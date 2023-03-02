make clean
gcc -ggdb3 -O0 -Wall -std=c11 -Wno-unused-function -Wvla fisopfs.c filesystem.c -D_FILE_OFFSET_BITS=64 -I/usr/include/fuse -lfuse -pthread -o fisopfs
sudo umount prueba
sudo gdb --args fisopfs -d prueba/