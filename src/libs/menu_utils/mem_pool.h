#pragma once

#include <malloc.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief A contiguous zone where memory can be allocated.
 */
typedef struct {
	/// Pointer to current free space position.
	char *pos;
	/// Pointer to start of zone.
	char *start;
	/// Pointer to end of zone.
	char *end;
} MemZone;

/**
 * @brief Allocate a memory zone with the given size.
 *
 * @param memory_zone MemZone to use.
 * @param size Size in bytes that the MemZone will have. (eg.: '1*1024*1024' for 1MB)
 */
void mem_zone_init(MemZone *memory_zone, size_t size);

/**
 * @brief Allocate memory from the zone.
 *
 * @param memory_zone MemZone to use.
 * @param size Size in bytes of the memory. (eg.: sizeof(int))
 *
 * @return the memory allocated.
 */
void *mem_zone_alloc(MemZone *memory_zone, size_t size);

/**
 * @brief Free all objects in the zone.
 *
 * @param memory_zone MemZone to use.
 */
void mem_zone_free_all(MemZone *memory_zone);

#ifdef __cplusplus
}
#endif
