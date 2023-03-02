#define FUSE_USE_VERSION 30

#include <fuse.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "fisopfs.h"
#include "filesystem.h"
#include "colors.h"

static struct filesystem *fs;

static char *filename;

static struct inode *
find_file_in_fs(const char *path)
{
	char pathCopy[MAX_FISOP_PATH_SIZE];
	strncpy(pathCopy, path, MAX_FISOP_PATH_SIZE - 1);
	strtok(pathCopy, "/");
	char *upperFile = strtok(NULL, "/");
	if (upperFile == NULL) {
		for (int i = 0; i < fs->numberOfInodes; i++) {
			struct inode *inode = &(*fs->root_inodes)[i];
			if (strcmp(path, inode->name) == 0) {
				return inode;
			}
		}
	} else {
		for (int i = 0; i < fs->numberOfDentries; i++) {
			struct dentry *dentry = &(*fs->dentries)[i];
			for (int j = 0; j < dentry->numberOfInodes; j++) {
				struct inode *inode = &(*dentry->inodes)[j];
				if (strcmp(upperFile, inode->name) == 0) {
					return inode;
				}
			}
		}
	}
	return NULL;
}

static int
find_file_idx_in_fs(const char *path)
{
	char pathCopy[MAX_FISOP_PATH_SIZE];
	strncpy(pathCopy, path, MAX_FISOP_PATH_SIZE - 1);
	strtok(pathCopy, "/");
	char *upperFile = strtok(NULL, "/");
	if (upperFile == NULL) {
		for (int i = 0; i < fs->numberOfInodes; i++) {
			struct inode *inode = &(*fs->root_inodes)[i];
			if (strcmp(path, inode->name) == 0) {
				return i;
			}
		}
	} else {
		for (int i = 0; i < fs->numberOfDentries; i++) {
			struct dentry *dentry = &(*fs->dentries)[i];
			for (int j = 0; j < dentry->numberOfInodes; j++) {
				struct inode *inode = &(*dentry->inodes)[j];
				if (strcmp(upperFile, inode->name) == 0) {
					return j;
				}
			}
		}
	}
	return -1;
}

static int
find_dentry_idx(const char *path)
{
	for (int i = 0; i < fs->numberOfDentries; i++) {
		struct dentry *dentry = &(*fs->dentries)[i];
		if (strcmp(path, dentry->name) == 0) {
			return i;
		}
	}
	return -1;
}

static struct dentry *
find_dentry(const char *path)
{
	for (int i = 0; i < fs->numberOfDentries; i++) {
		struct dentry *dentry = &(*fs->dentries)[i];
		if (strcmp(path, dentry->name) == 0) {
			return dentry;
		}
	}
	return NULL;
}

static int
fisopfs_getattr(const char *path, struct stat *st)
{
	/*
	 * Si hago touch root/file.txt -> path = "/file.txt"
	 * Si hago touch root/a/file.txt -> path = "/a"
	 */
	printf(CYAN "[debug] fisopfs_getattr(%s)\n" RESET, path);

	if (strcmp(path, "/") == 0) {
		st->st_uid = getuid();
		st->st_gid = getgid();
		st->st_mode = __S_IFDIR | 0755;
		st->st_nlink = 2;
		return 0;
	}
	path = &path[1];  // Me deshago del primer caracter que es un '/'

	char pathCopy[MAX_FISOP_PATH_SIZE];
	strncpy(pathCopy, path, MAX_FISOP_PATH_SIZE - 1);
	char *str = strtok(pathCopy, "/");
	char *upperFile = strtok(NULL, "/");

	if (upperFile == NULL) {
		for (int i = 0; i < fs->numberOfInodes; i++) {
			struct inode *inode = &(*fs->root_inodes)[i];
			if (strcmp(path, inode->name) == 0) {
				st->st_uid = getuid();
				st->st_gid = getgid();
				st->st_mode = __S_IFREG | 0644;
				st->st_size = inode->fileSize;
				st->st_nlink = 1;
				st->st_mtimensec = inode->lastMod;
				return 0;
			}
		}
	}
	for (int i = 0; i < fs->numberOfDentries; i++) {
		struct dentry *dentry = &(*fs->dentries)[i];
		if (strcmp(str, dentry->name) == 0) {
			st->st_uid = getuid();
			st->st_gid = getgid();
			st->st_mode = __S_IFDIR | 0755;
			st->st_nlink = 2;
			if (upperFile != NULL) {
				for (int j = 0; j < dentry->numberOfInodes; j++) {
					struct inode *inode =
					        &(*dentry->inodes)[j];
					if (strcmp(upperFile, inode->name) == 0) {
						st->st_uid = getuid();
						st->st_gid = getgid();
						st->st_mode = __S_IFREG | 0644;
						st->st_size = inode->fileSize;
						st->st_mtimensec = inode->lastMod;
						st->st_nlink = 1;
						return 0;
					}
				}
				return -ENOENT;
			}
			return 0;
		}
	}

	return -ENOENT;
}

static int
fisopfs_readdir(const char *path,
                void *buf,
                fuse_fill_dir_t filler,
                off_t offset,
                struct fuse_file_info *fi)
{
	printf(CYAN "[debug] fisopfs_readdir(%s)" RESET, path);

	struct dentry *dentry;
	filler(buf, ".", NULL, 0);
	filler(buf, "..", NULL, 0);
	if (strcmp(path, "/") == 0) {
		for (int i = 0; i < fs->numberOfInodes; i++) {
			struct inode *inode = &(*fs->root_inodes)[i];
			filler(buf, inode->name, NULL, 0);
		}
		for (int i = 0; i < fs->numberOfDentries; i++) {
			struct dentry *dentry = &(*fs->dentries)[i];
			filler(buf, dentry->name, NULL, 0);
		}
	} else {
		path = &path[1];  // Me deshago del primer caracter que es un '/'
		dentry = find_dentry(path);
		if (!dentry || dentry->timesOpen < 0)
			return -ENOENT;
		for (int i = 0; i < dentry->numberOfInodes; i++) {
			struct inode *inode = &(*dentry->inodes)[i];
			if (inode->fileSize > 0 || inode->name[0] != 0) {
				filler(buf, inode->name, NULL, 0);
			}
		}
	}

	return 0;
}

static int
fisopfs_read(const char *path,
             char *buffer,
             size_t size,
             off_t offset,
             struct fuse_file_info *fi)
{
	path = &path[1];  // Me deshago del primer caracter que es un '/'
	printf(CYAN "[debug] fisopfs_read(%s, %lu, %lu)\n" RESET, path, offset, size);

	struct inode *nodeFound = find_file_in_fs(path);
	if (!nodeFound || nodeFound->timesOpen < 1 || nodeFound->inumber != fi->fh)
		return -ENOENT;

	int copiedSize =
	        size + offset > nodeFound->fileSize ? nodeFound->fileSize : size;
	strncpy(buffer, nodeFound->data + offset, copiedSize);

	nodeFound->lastAccess = time(0);

	return copiedSize;
}

static int
fisopfs_opendir(const char *path, struct fuse_file_info *fi)
{
	printf(CYAN "[debug] fisopfs_opendir(%s)\n" RESET, path);

	if (strcmp(path, "/") == 0)
		return 0;

	path = &path[1];  // Me deshago del primer caracter que es un '/'

	int idx = find_dentry_idx(path);
	if (idx == -1)
		return -ENOENT;
	(&(*fs->dentries)[idx])->timesOpen++;

	fi->fh = idx;

	return 0;
}

static int
fisopfs_closedir(const char *path, struct fuse_file_info *fi)
{
	path = &path[1];  // Me deshago del primer caracter que es un '/'
	printf(CYAN "[debug] fisopfs_closedir(%s)\n" RESET, path);

	int idx = find_dentry_idx(path);
	struct dentry *dir = (&(*fs->dentries)[idx]);
	if (idx == -1 || fi->fh != idx || dir->timesOpen < 1)
		return -ENOENT;
	dir->timesOpen--;

	return 0;
}

static int
fisopfs_mkdir(const char *path, mode_t mode)
{
	path = &path[1];  // Me deshago del primer caracter que es un '/'
	printf(CYAN "[debug] fisopfs_mkdir(%s)\n" RESET, path);

	if (fs->numberOfDentries >= MAX_ROOT_DIR) {
		perror(RED "Cannot create more root entries" RESET);
		return -ENOSPC;
	}

	struct dentry *dentry = &(*fs->dentries)[fs->numberOfDentries];
	dentry->numberOfInodes = 0;
	dentry->timesOpen = 0;
	strncpy(dentry->name, path, DENTRY_MAX_NAME_LENGTH - 1);
	dentry->inodes = calloc(MAX_FILES_IN_DIR, sizeof(struct inode));
	if (dentry->inodes == NULL) {
		perror(NOT_ENOUGH_MEM_ERROR);
		exit(1);
	}
	for (int i = 0; i < MAX_FILES_IN_DIR; i++) {
		struct inode *inode = &(*dentry->inodes)[i];
		inode->parent = dentry;
	}

	fs->numberOfDentries++;
	return 0;
}

static int
fisopfs_mknod(const char *path, mode_t mode, dev_t dev)
{
	path = &path[1];  // Me deshago del primer caracter que es un '/'
	printf(CYAN "[debug] fisopfs_mknod(%s)\n" RESET, path);

	if (fs->numberOfInodes >= MAX_ROOT_FILES) {
		perror(RED "Cannot create more root files" RESET);
		return -ENOSPC;
	}

	char pathCopy[MAX_FISOP_PATH_SIZE];
	strncpy(pathCopy, path, MAX_FISOP_PATH_SIZE - 1);
	char *dir = strtok(pathCopy, "/");
	char *upperFile = strtok(NULL, "/");

	if (upperFile == NULL) {
		// Estoy en root
		if (fs->numberOfInodes >= MAX_ROOT_FILES) {
			perror(RED "Cannot create more root files" RESET);
			return -ENOSPC;
		}

		struct inode *inode = &(*fs->root_inodes)[fs->numberOfInodes];
		inode->inumber = fs->numberOfInodes;
		inode->fileSize = 0;
		inode->type = FILE_CLOSED;
		strncpy(inode->name, path, INODE_MAX_NAME_LENGTH - 1);
		inode->data = NULL;
		inode->timesOpen = 0;
		inode->parent = NULL;
		inode->birth = time(0);
		inode->lastAccess = time(0);
		inode->lastMod = time(0);
		fs->numberOfInodes++;
	} else {
		// Estoy en un directorio
		int idx = find_dentry_idx(dir);
		if (idx == -1)
			return -ENOENT;
		struct dentry *dentry = &(*fs->dentries)[idx];
		if (dentry->numberOfInodes >= MAX_FILES_IN_DIR) {
			perror(RED "Cannot create more files in this "
			           "directory" RESET);
			return -ENOSPC;
		}

		struct inode *inode = &(*dentry->inodes)[dentry->numberOfInodes];
		struct inode *last =
		        &(*dentry->inodes)[dentry->numberOfInodes - 1];
		inode->inumber = last->inumber + 1;
		inode->fileSize = 0;
		inode->type = FILE_CLOSED;
		strncpy(inode->name, upperFile, INODE_MAX_NAME_LENGTH - 1);
		inode->data = NULL;
		inode->timesOpen = 0;
		inode->parent = dentry;
		inode->birth = time(0);
		inode->lastAccess = time(0);
		inode->lastMod = time(0);
		dentry->numberOfInodes++;
	}

	return 0;
}

static int
fisopfs_write(const char *path,
              const char *buffer,
              size_t size,
              off_t offset,
              struct fuse_file_info *fi)
{
	path = &path[1];  // Me deshago del primer caracter que es un '/'
	printf(CYAN "[debug] fisopfs_write(%s)\n" RESET, path);

	struct inode *nodeFound = find_file_in_fs(path);
	if (!nodeFound || nodeFound->timesOpen < 1 || nodeFound->inumber != fi->fh)
		return -ENOENT;


	if ((fs->size + size - nodeFound->fileSize + offset > MAX_FILE_SIZE) ||
	    (nodeFound->data = realloc(nodeFound->data,
	                               nodeFound->fileSize + size)) == NULL) {
		perror(RED "Cannot allocate memory" RESET);
		return -ENOMEM;
	}

	fs->size += size - nodeFound->fileSize + offset;

	nodeFound->lastMod = time(0);
	nodeFound->fileSize += size;
	strncpy(nodeFound->data + offset, buffer, size);

	return size;
}

static int
fisopfs_open(const char *path, struct fuse_file_info *fi)
{
	path = &path[1];  // Me deshago del primer caracter que es un '/'
	printf(CYAN "[debug] fisopfs_open(%s)\n" RESET, path);

	struct inode *nodeFound = find_file_in_fs(path);
	if (!nodeFound)
		return -ENOENT;

	fi->fh = nodeFound->inumber;
	nodeFound->type = FILE_OPENED;
	nodeFound->timesOpen++;

	return 0;
}

static int
fisopfs_close(const char *path, struct fuse_file_info *fi)
{
	path = &path[1];  // Me deshago del primer caracter que es un '/'
	printf(CYAN "[debug] fisopfs_close(%s)\n" RESET, path);

	struct inode *nodeFound = find_file_in_fs(path);
	if (!nodeFound || nodeFound->timesOpen < 1 || nodeFound->inumber != fi->fh)
		return -ENOENT;

	nodeFound->timesOpen--;
	if (nodeFound->timesOpen == 0)
		nodeFound->type = FILE_CLOSED;

	return 0;
}

static int
fisopfs_rmdir(const char *path)
{
	path = &path[1];  // Me deshago del primer caracter que es un '/'
	printf(CYAN "[debug] fisopfs_rmdir(%s)\n" RESET, path);

	if (fs->numberOfDentries == 0)
		return -ENOENT;

	int idx = find_dentry_idx(path);
	if (idx == -1)
		return -ENOENT;
	struct dentry *dir = (&(*fs->dentries)[idx]);

	if (dir->timesOpen > 0)
		return -EBUSY;

	for (int i = 0; i < dir->numberOfInodes; i++) {
		struct inode *node = (&(*dir->inodes)[i]);
		if (node->timesOpen > 0)
			return -EBUSY;
	}

	for (int i = 0; i < dir->numberOfInodes; i++) {
		struct inode *node = (&(*dir->inodes)[i]);
		free(node->data);
	}

	free(dir->inodes);

	for (int i = idx; i < fs->numberOfDentries - 1; i++) {
		struct dentry *this = (&(*fs->dentries)[i]);
		struct dentry *next = (&(*fs->dentries)[i + 1]);
		memcpy(this, next, sizeof(struct dentry));
	}

	struct dentry *new = (&(*fs->dentries)[fs->numberOfDentries - 1]);
	if ((new->inodes = malloc(sizeof(struct inode) * MAX_FILES_IN_DIR)) ==
	    NULL) {
		perror(RED "Cannot allocate memory" RESET);
		exit(1);
	}

	fs->numberOfDentries--;

	return 0;
}

static struct inode *
find_file_in_dir(struct dentry *dir, char *filename)
{
	for (int i = 0; i < dir->numberOfInodes; i++) {
		struct inode *node = (&(*dir->inodes)[i]);
		if (strcmp(node->name, filename) == 0)
			return node;
	}
	return NULL;
}

static int
fisopfs_unlink(const char *path)
{
	path = &path[1];  // Me deshago del primer caracter que es un '/'
	printf(CYAN "[debug] fisopfs_unlink(%s)\n" RESET, path);

	char pathCopy[MAX_FISOP_PATH_SIZE];
	strncpy(pathCopy, path, MAX_FISOP_PATH_SIZE - 1);
	char *dir = strtok(pathCopy, "/");
	char *upperFile = strtok(NULL, "/");

	if (upperFile == NULL) {
		// Estoy en root
		if (fs->numberOfInodes == 0)
			return -ENOENT;

		int idx = find_file_idx_in_fs(path);

		if (idx == -1)
			return -ENOENT;

		struct inode *nodeFound = (&(*fs->root_inodes)[idx]);
		if (nodeFound->timesOpen > 0)
			return -EBUSY;

		free(nodeFound->data);

		int lastInumber = -1;
		for (int i = idx; i < fs->numberOfInodes - 1; i++) {
			struct inode *this = (&(*fs->root_inodes)[i]);
			struct inode *next = (&(*fs->root_inodes)[i + 1]);
			memcpy(this, next, sizeof(struct inode));
			lastInumber = next->inumber;
		}

		struct inode *new = (&(*fs->root_inodes)[fs->numberOfInodes - 1]);
		memset(new, 0, sizeof(struct inode));
		new->inumber = lastInumber;
		fs->numberOfInodes--;

		return 0;
	} else {
		// Estoy en un directorio
		if (fs->numberOfDentries == 0)
			return -ENOENT;

		int idx = find_dentry_idx(dir);

		if (idx == -1)
			return -ENOENT;

		struct dentry *dirFound = (&(*fs->dentries)[idx]);
		struct inode *nodeFound = find_file_in_dir(dirFound, upperFile);
		if (nodeFound->timesOpen > 0)
			return -EBUSY;

		free(nodeFound->data);

		int lastInumber = -1;
		for (int i = idx; i < dirFound->numberOfInodes - 1; i++) {
			struct inode *this = (&(*dirFound->inodes)[i]);
			struct inode *next = (&(*dirFound->inodes)[i + 1]);
			memcpy(this, next, sizeof(struct inode));
			lastInumber = next->inumber;
		}

		struct inode *new =
		        (&(*dirFound->inodes)[dirFound->numberOfInodes - 1]);
		memset(new, 0, sizeof(struct inode));
		new->inumber = lastInumber;
		dirFound->numberOfInodes--;

		return 0;
	}

	return -ENOENT;
}

static int
fisopfs_utimes(const char *path, const struct timespec tv[2])
{
	path = &path[1];  // Me deshago del primer caracter que es un '/'
	printf(RED "[debug] fisopfs_utimes(%s)\n" RESET, path);

	struct inode *nodeFound = find_file_in_fs(path);
	if (!nodeFound || nodeFound->timesOpen < 1)
		return -ENOENT;

	nodeFound->lastAccess = tv[0].tv_nsec;
	nodeFound->lastMod = tv[1].tv_nsec;

	return 0;
}

static int
fisopfs_truncate(const char *path, off_t size)
{
	path = &path[1];  // Me deshago del primer caracter que es un '/'
	printf(CYAN "[debug] fisopfs_truncate(%s)\n" RESET, path);

	struct inode *nodeFound = find_file_in_fs(path);
	if (!nodeFound || nodeFound->timesOpen < 1)
		return -ENOENT;

	if (nodeFound->type == 1)
		return -EISDIR;

	nodeFound->fileSize = size;
	nodeFound->data = realloc(nodeFound->data, size);
	nodeFound->lastMod = time(0);

	return 0;
}

void *
fisopfs_init()
{
	if ((fs = malloc(sizeof(struct filesystem))) == NULL) {
		perror(NOT_ENOUGH_MEM_ERROR);
		exit(1);
	}
	fs_init(filename, fs);
	printf(BLUE "starting...\n" RESET);
	return NULL;
}

void
fisopfs_destroy(void *param)
{
	fs_print(fs);
	fs_end(filename, fs, true);
	if (fs)
		free(fs);
	printf(BLUE "\ndone!\n" RESET);
}

static struct fuse_operations operations = {
	.getattr = fisopfs_getattr,
	.readdir = fisopfs_readdir,
	.read = fisopfs_read,
	.mkdir = fisopfs_mkdir,
	.mknod = fisopfs_mknod,
	.write = fisopfs_write,
	.utimens = fisopfs_utimes,
	.opendir = fisopfs_opendir,
	.releasedir = fisopfs_closedir,
	.open = fisopfs_open,
	.unlink = fisopfs_unlink,
	.release = fisopfs_close,
	.rmdir = fisopfs_rmdir,
	.truncate = fisopfs_truncate,
	.destroy = fisopfs_destroy,
	.init = fisopfs_init,
};

int
main(int argc, char *argv[])
{
	filename = (argv[3] == NULL) ? DEFAULT_FILENAME : argv[3];
	return fuse_main(argc, argv, &operations, NULL);
}
