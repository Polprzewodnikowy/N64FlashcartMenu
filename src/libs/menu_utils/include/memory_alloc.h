#include "mem_pool.h"

/**
 * @brief Used to allocate an object using either a MemZone or malloc according to the parameters.
 */
#define MEM_ALLOC(SIZE, MEMPOOL) ((MEMPOOL) ? mem_zone_alloc((MEMPOOL), (SIZE)) : malloc(SIZE))
