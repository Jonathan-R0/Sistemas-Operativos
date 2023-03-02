#ifndef _FILESYSTEM_H_
#define _FILESYSTEM_H_
#include <stdbool.h>
#include "colors.h"

#define MAX_ROOT_DIR 100
#define MAX_ROOT_FILES 100
#define MAX_FILES_IN_DIR 10
#define INODE_MAX_NAME_LENGTH 256
#define DENTRY_MAX_NAME_LENGTH 256
#define MAX_PATH_LENGTH 256
#define MAX_FILE_SIZE 1153433600 // 1024 * 1024 * 1100 = ~1.1GB
#define FS_FILE_EXTENSION ".fisopfs"
#define DEFAULT_FILENAME "data"FS_FILE_EXTENSION
#define OPENING_FILE_ERROR RED"File could not be opened. "RESET
#define READING_FILE_ERROR RED"File could not be read. "RESET
#define WRITING_FILE_ERROR RED"File could not be written. "RESET
#define CLOSING_FILE_ERROR RED"Error when closing file. "RESET
#define NEW_FS_MSG GREEN"File is empty, creating new filesystem."RESET
#define NOT_ENOUGH_MEM_ERROR RED"Error allocating memory for filesystem."RESET
#define FILE_OPENED 'o'
#define FILE_CLOSED 'c'

#define LOG_READING_ERROR() \
    fprintf(stderr, "Error reading on line %d\n", __LINE__)

struct inode {
    int inumber;
    int fileSize;
    char type;
    char name[INODE_MAX_NAME_LENGTH];
    char* data;
    int timesOpen;
    time_t birth;
    time_t lastMod;
    time_t lastAccess;
    struct dentry* parent;
};

struct dentry {
    int numberOfInodes;
    int timesOpen;
    char name[DENTRY_MAX_NAME_LENGTH];
    struct inode (*inodes)[MAX_FILES_IN_DIR];
};

struct filesystem {
    int numberOfInodes;
    int numberOfDentries;
    int size;
    struct dentry (*dentries)[MAX_ROOT_DIR];
    struct inode (*root_inodes)[MAX_ROOT_FILES];
};

void inode_end(FILE* data, struct inode* inode);

void inode_init(FILE* data, struct inode* inode, struct dentry* parent);

void dentry_end(FILE* data, struct dentry* dentry);

void dentry_init(struct filesystem* fs, FILE* data, struct dentry* dentry);

void fs_load(FILE* data, struct filesystem* fs);

void fs_unload(FILE* data, struct filesystem* fs,  bool freeData);

void fs_init(char* filename, struct filesystem* fs);

void fs_end(char* filename, struct filesystem* fs, bool freeData);

void fs_print(struct filesystem* fs);

#endif // _FILESYSTEM_H_
