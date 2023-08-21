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


#endif
