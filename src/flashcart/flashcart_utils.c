/**
 * @file flashcart_utils.c
 * @brief Flashcart utility functions implementation
 * @ingroup flashcart
 */

#include <libdragon.h>

#include "flashcart_utils.h"
#include "utils/fs.h"
#include "utils/utils.h"

/**
 * @brief Perform a DMA read operation from the PI (Peripheral Interface).
 * 
 * @param src Source address.
 * @param dst Destination address.
 * @param length Length of data to read.
 */
void pi_dma_read_data (void *src, void *dst, size_t length) {
    data_cache_hit_writeback_invalidate(dst, length);
    dma_read_async(dst, (uint32_t) (src), length);
    dma_wait();
}

/**
 * @brief Perform a DMA write operation to the PI (Peripheral Interface).
 * 
 * @param src Source address.
 * @param dst Destination address.
 * @param length Length of data to write.
 */
void pi_dma_write_data (void *src, void *dst, size_t length) {
    assert((((uint32_t) (src)) & 0x07) == 0);
    assert((((uint32_t) (dst)) & 0x01) == 0);
    assert((length & 1) == 0);

    data_cache_hit_writeback(src, length);
    dma_write_raw_async(src, (uint32_t) (dst), length);
    dma_wait();
}

/**
 * @brief Align the file size to the SD sector size to prevent partial sector load.
 * 
 * @param fil Pointer to the file object.
 */
void fatfs_fix_file_size (FIL *fil) {
    // HACK: Align file size to the SD sector size to prevent FatFs from doing partial sector load.
    //       We are relying on direct transfer from SD to SDRAM without CPU intervention.
    //       Sending some extra bytes isn't an issue here.
    fil->obj.objsize = ALIGN(f_size(fil), FS_SECTOR_SIZE);
}

/**
 * @brief Get the file sectors in the FAT filesystem.
 * 
 * @param path Path to the file.
 * @param address Pointer to store the address of the file sectors.
 * @param type The type of address (memory or PI).
 * @param max_sectors Maximum number of sectors.
 * @return true if an error occurred, false otherwise.
 */
bool fatfs_get_file_sectors (char *path, uint32_t *address, address_type_t type, uint32_t max_sectors) {
    FATFS *fs;
    FIL fil;
    bool error = false;

    if (f_open(&fil, strip_fs_prefix(path), FA_READ) != FR_OK) {
        return true;
    }

    fatfs_fix_file_size(&fil);

    fs = fil.obj.fs;

    uint32_t sector_count = MIN(f_size(&fil) / FS_SECTOR_SIZE, max_sectors);

    for (uint32_t file_sector = 0; file_sector < sector_count; file_sector += fs->csize) {
        if ((f_lseek(&fil, (file_sector * FS_SECTOR_SIZE) + (FS_SECTOR_SIZE / 2))) != FR_OK) {
            error = true;
            break;
        }

        uint32_t cluster = fil.clust;

        if ((cluster < 2) || (cluster >= fs->n_fatent)) {
            error = true;
            break;
        }

        uint32_t cluster_sector = (fs->database + ((LBA_t) (fs->csize) * (cluster - 2)));

        for (uint32_t i = 0; i < fs->csize; i++) {
            uint32_t sector = (cluster_sector + i);

            if ((file_sector + i) >= sector_count) {
                break;
            }

            switch (type) {
                case ADDRESS_TYPE_MEM:
                    *address = sector;
                    break;

                case ADDRESS_TYPE_PI:
                    io_write((uint32_t) (address), sector);
                    break;
            }

            address++;
        }
    }

    if (f_close(&fil) != FR_OK) {
        error = true;
    }

    return error;
}
