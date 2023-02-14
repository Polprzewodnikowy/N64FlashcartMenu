#include <fatfs/ff.h>

#include "fs.h"
#include "utils.h"


bool file_exists (char *path) {
    FRESULT fr;
    FILINFO fno;

    fr = f_stat(path, &fno);

    return (fr == FR_OK);
}

size_t file_get_size (char *path) {
    FILINFO fno;

    if (f_stat(path, &fno) != FR_OK) {
        return 0;
    }

    return (size_t) (fno.fsize);
}

bool file_allocate (char *path, size_t size) {
    FIL fil;
    bool error = false;

    if (f_open(&fil, path, FA_WRITE | FA_CREATE_NEW) != FR_OK) {
        return true;
    }

    if (f_lseek(&fil, size) != FR_OK) {
        error = true;
    }

    if (f_tell(&fil) != size) {
        error = true;
    }

    if (f_close(&fil) != FR_OK) {
        error = true;
    }

    return error;
}

bool file_get_sectors (char *path, uint32_t *sectors, size_t entries) {
    FATFS *fs;
    FIL fil;
    bool error = false;

    for (int i = 0; i < entries; i++) {
        sectors[i] = 0;
    }

    if (f_open(&fil, path, FA_READ) != FR_OK) {
        return true;
    }

    fs = fil.obj.fs;

    uint32_t file_sector_entries = (ALIGN(f_size(&fil), FS_SECTOR_SIZE) / FS_SECTOR_SIZE);
    file_sector_entries = (file_sector_entries > entries) ? entries : file_sector_entries;

    uint32_t cluster_sector = 0;

    for (int file_sector = 0; file_sector < file_sector_entries; file_sector++) {
        if ((file_sector % fs->csize) == 0) {
            if ((f_lseek(&fil, (file_sector * FS_SECTOR_SIZE) + (FS_SECTOR_SIZE / 2))) != FR_OK) {
                error = true;
                break;
            }
            uint32_t cluster = fil.clust;
            if (cluster >= fs->n_fatent) {
                error = true;
                break;
            }
            cluster_sector = (fs->database + ((LBA_t) (fs->csize) * (cluster - 2)));
        }
        *sectors++ = (cluster_sector + (file_sector % fs->csize));
    }

    if (f_close(&fil) != FR_OK) {
        error = true;
    }

    return error;
}
