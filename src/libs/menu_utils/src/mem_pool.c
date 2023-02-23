#include "../include/mem_pool.h"

#include <string.h>

#include <libdragon.h>

void mem_zone_init(MemZone *z, size_t size) {
	disable_interrupts();

	void *ptr = malloc(size);
	if (ptr == NULL) {
		abort();  // Put your error handling here.
	}
	z->pos = (char *)ptr;
	z->start = z->pos;
	z->end = z->start + size;

	memset(z->start, 0, size);

	enable_interrupts();
}

void *mem_zone_alloc(MemZone *z, size_t size) {
	if (size == 0) {
		return NULL;
	}
	// Round up to multiple of 16 bytes.
	size = (size + 15) & ~(size_t)15;
	// How much free space remaining in zone?
	size_t rem = z->end - z->pos;
	if (rem < size) {
		return NULL;  // Out of memory. Put your error handling here.
	}

	void *ptr = (void *)z->pos;
	z->pos += size;
	return ptr;
}

void mem_zone_free_all(MemZone *z) {
	z->pos = z->start;
}
