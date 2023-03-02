#define _DEFAULT_SOURCE

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

#include "malloc.h"
#include "printfmt.h"

#define ALIGN4(s) (((((s) -1) >> 2) << 2) + 4)
#define REGION2PTR(r) ((r) + 1)
#define PTR2REGION(ptr) ((struct region *) (ptr) -1)
#define HARD_DEBUG

#define BEST_FIT

struct block *block = NULL;

int amount_of_mallocs = 0;
int amount_of_frees = 0;
int amount_of_invalid_frees = 0;
int requested_memory = 0;

static void
print_statistics(void)
{
	printfmt("\tsize of struct region:   %ld\n", sizeof(struct region));
	printfmt("\tsize of struct block:    %ld\n", sizeof(struct block));
	printfmt("\tmallocs:           %d\n", amount_of_mallocs);
	printfmt("\tfrees:             %d\n", amount_of_frees);
	printfmt("\tinvalid frees:     %d\n", amount_of_invalid_frees);
	printfmt("\trequested:         %d\n", requested_memory);
}

static void
check_heap(void)
{
	for (struct block *b = block; b != NULL; b = b->next_block) {
		int total_size = (int) sizeof(struct region) * -1;
		for (struct region *r = b->region_list; r != NULL; r = r->next) {
			total_size += r->size;
			total_size += sizeof(struct region);
		}
		if (total_size != (int) b->size) {
			printfmt(RED "Heap corrupted! %ld != %ld" RESET_COLOR "\n",
			         total_size,
			         b->size);
		}
	}
}

static void
print_heap(void)
{
	printfmt(RED "Heap:" RESET_COLOR "\n");
	for (struct block *b = block; b != NULL; b = b->next_block) {
		printfmt("  " STRINGIFY_BLOCK(b));
		for (struct region *r = b->region_list; r != NULL; r = r->next) {
			printfmt("  " STRINGIFY_REGION(r));
		}
	}
	check_heap();
}

static void
unmap()
{
	struct block *b = block;
	while (b != NULL) {
		struct block *next = b->next_block;
		if (munmap(b, b->size + sizeof(struct block)) == -1) {
			printfmt(RED "Munmap failed: %s" RESET_COLOR "\n",
			         strerror(errno));
		}
		b = next;
	}
}

static void
end(void)
{
	printfmt(EXIT_LINE);
	print_statistics();
	printfmt("\n");
#ifdef HARD_DEBUG
	print_heap();
#endif
	unmap();
	/*
	for (struct block *iter_b = block->next_block; iter_b != NULL;
	     iter_b = block->next_block) {
	        block->next_block = iter_b->next_block;
	        iter_b->next_block = NULL;
	        iter_b->prev_block = NULL;
	        for(struct region *iter_r = iter_b->region_list; iter_r != NULL;
	            iter_r = iter_b->region_list) {
	                iter_b->region_list = iter_r->next;
	                iter_r->next = NULL;
	                iter_r->prev = NULL;
	                iter_r->block = NULL;
	                iter_r->free = true;
	                iter_r->size = 0;
	        }
	        if (munmap(iter_b, iter_b->size) == -1) {
	                perror("munmap");
	        }
	}
	*/
}

static struct region *
first_fit(size_t size)
{
	for (struct block *iter_b = block; iter_b != NULL;
	     iter_b = iter_b->next_block) {
		for (struct region *iter_r = iter_b->region_list; iter_r != NULL;
		     iter_r = iter_r->next) {
			if (iter_r->size >= size && iter_r->free) {
				return iter_r;
			}
		}
	}

	return NULL;
}

static struct region *
best_fit(size_t size)
{
	if (block == NULL)
		return NULL;
	struct region *best = first_fit(size);
	for (struct block *iter_b = block; iter_b != NULL;
	     iter_b = iter_b->next_block) {
		for (struct region *iter_r = iter_b->region_list->next;
		     iter_r != NULL;
		     iter_r = iter_r->next) {
			if (best && iter_r->free && iter_r->size >= size &&
			    iter_r->size < best->size) {
				best = iter_r;
			}
		}
	}
	return best;
}

static struct region *
find_free_region(size_t size)
{
#ifdef FIRST_FIT
	return first_fit(size);
#endif

#ifdef BEST_FIT
	return best_fit(size);
#endif
}

static struct region *
grow_heap(size_t size)
{
	struct block *curr =
	        (struct block *) mmap(NULL,
	                              sizeof(struct region) +
	                                      sizeof(struct block) + size,
	                              PROT_READ | PROT_WRITE,
	                              MAP_PRIVATE | MAP_ANONYMOUS,
	                              -1,
	                              0);

	if (curr == (struct block *) -1) {
		return NULL;
	}

	// first time here
	if (block == NULL) {
		block = curr;
		block->next_block = NULL;
		block->prev_block = NULL;
		block->region_list = (struct region *) (curr + 1);
		block->size = size;
		atexit(end);
	} else {
		for (struct block *iter = block; iter != NULL;
		     iter = iter->next_block) {
			if (iter->next_block == NULL) {
				iter->next_block = curr;
				curr->prev_block = iter;
				curr->next_block = NULL;
				curr->region_list = (struct region *) (curr + 1);
				curr->size = size;
				break;
			}
		}
	}

	struct region *region = (struct region *) (curr + 1);

	region->free = false;
	region->size = size;
	region->next = NULL;
	region->prev = NULL;
	region->block = curr;

	return region;
}

static bool
is_invalid_or_already_free_region(void *ptr)
{
	if (ptr == NULL || block == NULL || block->region_list == NULL)
		return true;

	for (struct block *iter_b = block; iter_b != NULL;
	     iter_b = iter_b->next_block) {
		for (struct region *iter_r = iter_b->region_list; iter_r != NULL;
		     iter_r = iter_r->next) {
			if (REGION2PTR(iter_r) == ptr) {
				return (iter_r->free);
			}
		}
	}

	return true;
}

void
split(struct region *curr, size_t size)
{
	if (curr->size - size > sizeof(struct region)) {
		struct region *new_region =
		        (struct region *) ((char *) curr +
		                           sizeof(struct region) + size);
		new_region->size = curr->size - size - sizeof(struct region);
		new_region->next = curr->next;
		new_region->prev = curr;
		new_region->free = true;
		new_region->block = curr->block;
		curr->size = size;
		curr->next = new_region;
	}
}

void
coalesce(struct region *curr)
{
	if ((curr->prev == NULL || !curr->prev->free) &&
	    (curr->next == NULL || !curr->next->free)) {
		return;
	}
	if (curr->next != NULL && curr->next->free) {
		curr->size += curr->next->size + sizeof(struct region);
		curr->next = curr->next->next;
		if (curr->next != NULL) {
			curr->next->prev = curr;
		}
	}
	if (curr->prev != NULL && curr->prev->free) {
		curr->prev->size += curr->size + sizeof(struct region);
		curr->prev->next = curr->next;
		if (curr->next != NULL) {
			curr->next->prev = curr->prev;
		}
	}
	if (curr->prev != NULL && curr->prev->free && curr->next != NULL &&
	    curr->next->free) {
		curr->prev->size += curr->size + curr->next->size +
		                    2 * sizeof(struct region);
		curr->prev->next = curr->next->next;
		if (curr->next->next != NULL) {
			curr->next->next->prev = curr->prev;
		}
	}
}

void *
malloc(size_t size)
{
	if (size == 0) {
		return NULL;
	}

	size = ALIGN4(size);
	size = (size < 256) ? 256 : size;

	struct region *next = find_free_region(size);

	if (!next) {
		if (size <= SMALL_SIZE) {
			next = grow_heap(SMALL_SIZE);
		} else if (size > SMALL_SIZE && size <= MEDIUM_SIZE) {
			next = grow_heap(MEDIUM_SIZE);
		} else if (size > MEDIUM_SIZE && size <= BIG_SIZE) {
			next = grow_heap(BIG_SIZE);
		} else if (size > BIG_SIZE) {
			errno = ENOMEM;
			return NULL;
		}
	}

	amount_of_mallocs++;
	requested_memory += size;

	next->free = false;
	split(next, size);

#ifdef HARD_DEBUG
	print_heap();
#endif

	return REGION2PTR(next);
}

void
free(void *ptr)
{
	if (ptr == NULL) {
		return;
	}
	if (is_invalid_or_already_free_region(ptr)) {
		amount_of_invalid_frees++;
		return;
	}

	struct region *curr = PTR2REGION(ptr);
	amount_of_frees++;

	assert(curr->free == 0);

	curr->free = true;

	coalesce(curr);

#ifdef HARD_DEBUG
	print_heap();
#endif
}

void *
calloc(size_t nmemb, size_t size)
{
	if (nmemb == 0 || size == 0)
		return NULL;
	size_t total_size = ALIGN4(nmemb * size);
	total_size = (total_size < 256) ? 256 : total_size;
	char *ptr = malloc(total_size);
	if (ptr == NULL)
		return NULL;
	explicit_bzero(ptr, total_size);
	return ptr;
}

void *
realloc(void *ptr, size_t size)
{
	if (ptr == NULL) {
		return malloc(size);
	} else if (size == 0) {
		free(ptr);
		return NULL;
	}

	size = ALIGN4(size);
	size = (size < 256) ? 256 : size;

	if (is_invalid_or_already_free_region(ptr))
		return NULL;

	struct region *curr = PTR2REGION(ptr);
	if (curr->size == size) {
		return ptr;
	} else if (curr->size > size && curr->size - size > sizeof(struct region)) {
		split(curr, size);
		coalesce(curr->next);
		return REGION2PTR(curr);
	} else {
		void *new = malloc(size);
		if (new == NULL)
			return NULL;
		memcpy(new, ptr, curr->size);
		free(ptr);
		return new;
	}
	return NULL;
}
