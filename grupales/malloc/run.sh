make clean
make libmalloc.so
make test-d
LD_PRELOAD=${PWD}/libmalloc.so ./test-d