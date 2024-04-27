/**
 * @file flashcart_utils.h
 * @brief Flashcart utilities
 * @ingroup flashcart
 */

#ifndef FLASHCART_UTILS_H__
#define FLASHCART_UTILS_H__


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <fatfs/ff.h>


#define SAVE_WRITEBACK_MAX_SECTORS  (256)


typedef enum {
    ADDRESS_TYPE_MEM,
    ADDRESS_TYPE_PI,
} address_type_t;


void pi_dma_read_data (void *src, void *dst, size_t length);
void pi_dma_write_data (void *src, void *dst, size_t length);
void fatfs_fix_file_size (FIL *fil);
bool fatfs_get_file_sectors (char *path, uint32_t *address, address_type_t address_type, uint32_t max_sectors);


#endif
