#define  _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "filesystem.h"

void dentry_end(FILE* data, struct dentry* dentry) {
    if (fwrite(dentry, sizeof(struct dentry), 1, data) != 1) {
        perror(WRITING_FILE_ERROR"\n");
        exit(1);
    }
    for (int i = 0; i < dentry->numberOfInodes; i++) {
        inode_end(data, &(*dentry->inodes)[i]);
    }
}

void dentry_init(struct filesystem* fs, FILE* data, struct dentry* dentry) {
    if (fread(dentry, sizeof(struct dentry), 1, data) != 1) {
        LOG_READING_ERROR();
        exit(1);
    }
    dentry->inodes = calloc(MAX_FILES_IN_DIR, sizeof(struct inode));
    if (dentry->inodes == NULL) {
        perror(NOT_ENOUGH_MEM_ERROR);
        exit(1);
    }
    for (int i = 0; i < dentry->numberOfInodes; i++) {
        struct inode* inode = &(*dentry->inodes)[i];
        fs->size += inode->fileSize;
        inode_init(data, inode, dentry);
    }
}

void inode_end(FILE* data, struct inode* inode) {
    if (fwrite(inode, sizeof(struct inode), 1, data) != 1) {
        perror(WRITING_FILE_ERROR"\n");
        exit(1);
    }
}

void inode_init(FILE* data, struct inode* inode, struct dentry* parent) {
    if (fread(inode, sizeof(struct inode), 1, data) != 1) {
        LOG_READING_ERROR();
        exit(1);
    }
    inode->parent = parent;
    if ((inode->data = malloc(inode->fileSize)) == NULL) {
        perror(NOT_ENOUGH_MEM_ERROR);
        exit(1);
    }
}

void fs_unload(FILE* data, struct filesystem* fs, bool freeData) {
    for (int i = 0; i < fs->numberOfInodes; i++) {
        struct inode* inode = &(*fs->root_inodes)[i];
        if (fwrite(inode->data, inode->fileSize, 1, data) != 1) {
            perror(WRITING_FILE_ERROR"\n");
            exit(1);
        }
        if (freeData && inode->data) free(inode->data);
    }
    for (int i = 0; i < fs->numberOfDentries; i++) {
        struct dentry* dentry = &(*fs->dentries)[i];
        for (int j = 0; j < dentry->numberOfInodes; j++) {
            struct inode* inode = &(*dentry->inodes)[j];
            if (fwrite(inode->data, inode->fileSize, 1, data) != 1) {
                perror(WRITING_FILE_ERROR"\n");
                exit(1);
            }
            if (freeData && inode->data) free(inode->data);
        }
        if (freeData) free(dentry->inodes);
    }
}

void fs_load(FILE* data, struct filesystem* fs) {
    for (int i = 0; i < fs->numberOfInodes; i++) {
        struct inode* inode = &(*fs->root_inodes)[i];
        if (fread(inode->data, inode->fileSize, 1, data) != 1) {
            LOG_READING_ERROR();
            exit(1);
        }
    }
    for (int i = 0; i < fs->numberOfDentries; i++) {
        struct dentry* dentry = &(*fs->dentries)[i];
        for (int j = 0; j < dentry->numberOfInodes; j++) {
            struct inode* inode = &(*dentry->inodes)[j];
            if (fread(inode->data, inode->fileSize, 1, data) != 1) {
                LOG_READING_ERROR();
                exit(1);
            }
        }
    }
}

void fs_init(char* filename, struct filesystem* fs) {
    FILE* data = fopen(filename, "ab+");
    if (data < 0) {
        perror(OPENING_FILE_ERROR"\n");
        exit(1);
    }
    size_t itemsRead = fread(fs, sizeof(struct filesystem), 1, data);
    if (itemsRead == 0) {
        printf(NEW_FS_MSG"\n");
        fs->numberOfInodes = 0;
        fs->numberOfDentries = 0;
        fs->size = 0;
    } else if (itemsRead != 1) {
        perror(READING_FILE_ERROR"\n");
    }
    fs->dentries = calloc(MAX_ROOT_DIR, sizeof(struct dentry));
    fs->root_inodes = calloc(MAX_ROOT_FILES, sizeof(struct inode));
    if (fs->dentries == NULL || fs->root_inodes == NULL) {
        perror(NOT_ENOUGH_MEM_ERROR"\n");
        exit(1);
    }
    if (fs->numberOfInodes > MAX_ROOT_FILES || fs->numberOfDentries > MAX_ROOT_DIR) {
        perror(READING_FILE_ERROR"\n");
        exit(1);
    }
    for (int i = 0; i < fs->numberOfInodes; i++) {
        struct inode* inode = &(*fs->root_inodes)[i];
        fs->size += inode->fileSize;
        inode_init(data, inode, NULL);
    }
    for (int i = 0; i < fs->numberOfDentries; i++) {
        struct dentry* dentry = &(*fs->dentries)[i];
        dentry_init(fs, data, dentry);
    }
    fs_load(data, fs);
    if (fclose(data) == EOF) {
        perror(CLOSING_FILE_ERROR"\n");
        exit(1);
    }
}

void fs_end(char* filename, struct filesystem* fs, bool freeData) {
    FILE* data = fopen(filename, "wb");
    if (data < 0) {
        perror(OPENING_FILE_ERROR"\n");
        exit(1);
    }
    if (fwrite(fs, sizeof(struct filesystem), 1, data) != 1) {
        perror(WRITING_FILE_ERROR"\n");
        exit(1);
    }
    for (int i = 0; i < fs->numberOfInodes; i++) {
        struct inode* inode = &(*fs->root_inodes)[i];
        inode_end(data, inode);
    }
    for (int i = 0; i < fs->numberOfDentries; i++) {
        struct dentry* dentry = &(*fs->dentries)[i];
        dentry_end(data, dentry);
    }
    fs_unload(data, fs, freeData);
    if (fclose(data) == EOF) {
        perror(CLOSING_FILE_ERROR"\n");
        exit(1);
    }
    if (freeData) {
        if (fs->root_inodes) free(fs->root_inodes);
        if (fs->dentries) free(fs->dentries);
    }
}

void fs_print(struct filesystem* fs) {
    printf("-> Filesystem:\n");
    printf("   Filesystem size: %d\n", fs->size);
    printf("   Number of inodes: %d\n", fs->numberOfInodes);
    printf("   Number of dentries: %d\n", fs->numberOfDentries);
    printf("-> Inodes:\n");
    for (int i = 0; i < fs->numberOfInodes; i++) {
        struct inode* inode = &(*fs->root_inodes)[i];
        printf("\t- inode %d:\n", inode->inumber);
        printf("\tFile size: %d\n", inode->fileSize);
        printf("\tFile type: %c\n", inode->type);
        printf("\tFile name: %s\n", inode->name);
        printf("\tTimes Open: %d\n", inode->timesOpen);
        printf("\tData: ");
        for (int c = 0; c < inode->fileSize; c++) {
            printf("%c", inode->data[c]);
        }
        printf("\n");
    }
    printf("-> Dentries:\n");
    for (int i = 0; i < fs->numberOfDentries; i++) {
        struct dentry* dentry = &(*fs->dentries)[i];
        printf("\tDir name: %s with %d inodes\n", dentry->name, dentry->numberOfInodes);
        for (int j = 0; j < dentry->numberOfInodes; j++) {
            struct inode* inode = &(*dentry->inodes)[j];
            printf("\t\t- inode %d\n", inode->inumber);
            printf("\t\tFile size: %d\n", inode->fileSize);
            printf("\t\tFile type: %c\n", inode->type);
            printf("\t\tFile name: %s\n", inode->name);
            printf("\t\tTimes Open: %d\n", inode->timesOpen);
            printf("\t\tData: ");
            for (int c = 0; c < inode->fileSize; c++) {
                printf("%c", inode->data[c]);
            }
            printf("\n");
        }
        printf("\n");
    }
}
