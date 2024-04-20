/**
 * @file flashcart_utils.h
 * @brief Flashcart utilities
 * @ingroup flashcart
 */

#ifndef FLASHCART_UTILS_H__
#define FLASHCART_UTILS_H__


#include <fatfs/ff.h>


void fix_file_size (FIL *fil);
void pi_dma_read_data (void *src, void *dst, size_t length);
void pi_dma_write_data (void *src, void *dst, size_t length);
bool file_get_sectors (char *path, void (*callback) (uint32_t sector_count, uint32_t file_sector, uint32_t cluster_sector, uint32_t cluster_size));


#endif
