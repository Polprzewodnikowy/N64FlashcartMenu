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

/**
 * @brief Address types for DMA operations.
 */
typedef enum {
    ADDRESS_TYPE_MEM, /**< Memory address type. */
    ADDRESS_TYPE_PI,  /**< Peripheral Interface address type. */
} address_type_t;

/**
 * @brief Perform a DMA read operation from the PI (Peripheral Interface).
 * 
 * @param src Source address.
 * @param dst Destination address.
 * @param length Length of data to read.
 */
void pi_dma_read_data (void *src, void *dst, size_t length);

/**
 * @brief Perform a DMA write operation to the PI (Peripheral Interface).
 * 
 * @param src Source address.
 * @param dst Destination address.
 * @param length Length of data to write.
 */
void pi_dma_write_data (void *src, void *dst, size_t length);

/**
 * @brief Fix the file size in the FAT filesystem.
 * 
 * @param fil Pointer to the file object.
 */
void fatfs_fix_file_size (FIL *fil);

/**
 * @brief Get the file sectors in the FAT filesystem.
 * 
 * @param path Path to the file.
 * @param address Pointer to store the address of the file sectors.
 * @param address_type The type of address (memory or PI).
 * @param max_sectors Maximum number of sectors.
 * @return true if successful, false otherwise.
 */
bool fatfs_get_file_sectors (char *path, uint32_t *address, address_type_t address_type, uint32_t max_sectors);

#endif /* FLASHCART_UTILS_H__ */
