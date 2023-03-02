#define  _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "filesystem.h"

void fill_with_example_data(struct filesystem* fs) {
    fs->numberOfInodes = 3;
    fs->numberOfDentries = 2;
    fs->size = 0;
    for (int i = 0; i < fs->numberOfInodes; i++) {
        struct inode* inode = &(*fs->root_inodes)[i];
        inode->inumber = i + 1;
        inode->fileSize = i + 4;
        inode->data = calloc(1, inode->fileSize);
        for (int c = 0; c < inode->fileSize; c++) {
            inode->data[c] = 'a' + i;
        }
        inode->data[inode->fileSize] = 0;
        inode->type = FILE_CLOSED;
        inode->parent = NULL;
        inode->timesOpen = 0;
        inode->birth = 0;
        inode->lastAccess = 0;
        inode->lastMod = 0;
        sprintf(inode->name, "file %d", i);
    }
    for (int i = 0; i < fs->numberOfDentries; i++) {
        struct dentry* dentry = &(*fs->dentries)[i];
        dentry->numberOfInodes = 2;
        dentry->inodes = calloc(2, sizeof(struct inode));
        dentry->timesOpen = 0;
        sprintf(dentry->name, "dir %d", i);
        for (int j = 0; j < dentry->numberOfInodes; j++) {
            struct inode* inode = &(*dentry->inodes)[j];
            inode->inumber = j + 5;
            inode->fileSize = 6 + j;
            inode->data = calloc(1, inode->fileSize);
            for (int c = 0; c < inode->fileSize; c++) {
                inode->data[c] = 'a' + j;
            }
            inode->data[inode->fileSize + 1] = 0;
            inode->type = FILE_CLOSED;
            inode->parent = dentry;
            inode->timesOpen = 0;
            inode->birth = 0;
            inode->lastAccess = 0;
            inode->lastMod = 0;
            sprintf(inode->name, "file %d", i * 2 + j);
        }
    }
}

int main(int argc, char** argv) {
    struct filesystem fs = {0};
    char* filename = (argv[1] == NULL) ? DEFAULT_FILENAME : argv[1];
    fs_init(filename, &fs);
    fill_with_example_data(&fs);
    fs_print(&fs);
    fs_end(filename, &fs, true);
    return 0;
}
