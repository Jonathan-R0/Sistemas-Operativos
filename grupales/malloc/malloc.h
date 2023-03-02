#ifndef _MALLOC_H_
#define _MALLOC_H_

#define SMALL_SIZE 16000
#define MEDIUM_SIZE 1000000
#define BIG_SIZE 32000000

enum size {
    none = 0,
    small = SMALL_SIZE, 
    medium = MEDIUM_SIZE, 
    big = BIG_SIZE
};

struct region;

struct block;

struct region {
	bool free;
	size_t size;
	struct region *next;
	struct region *prev;
    struct block* block;
};

struct block {
	enum size size;
    struct region *region_list;
    struct block *next_block;
    struct block *prev_block;
};

void split(struct region* curr, size_t size);

void coalesce(struct region* curr);

void *malloc(size_t size);

void free(void *ptr);

void *calloc(size_t nmemb, size_t size);

void *realloc(void *ptr, size_t size);

#endif // _MALLOC_H_
