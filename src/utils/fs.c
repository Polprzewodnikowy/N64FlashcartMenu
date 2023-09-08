#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include <fatfs/ff.h>

#include "fs.h"
#include "utils.h"


char *strip_sd_prefix (char *path) {
    const char *prefix = "sd:/";

    char *found = strstr(path, prefix);
    if (found) {
        return found + strlen(prefix) - 1;
    }

    return path;
}


bool file_exists (char *path) {
    FRESULT fr;
    FILINFO fno;

    fr = f_stat(strip_sd_prefix(path), &fno);

    return ((fr == FR_OK) && (!(fno.fattrib & AM_DIR)));
}


size_t file_get_size (char *path) {
    FILINFO fno;

    if (f_stat(strip_sd_prefix(path), &fno) != FR_OK) {
        return 0;
    }

    return (size_t) (fno.fsize);
}

bool file_delete (char *path) {
    if (file_exists(path)) {
        return (f_unlink(strip_sd_prefix(path)) != FR_OK);
    }
    return false;
}

bool file_allocate (char *path, size_t size) {
    FIL fil;
    bool error = false;

    if (f_open(&fil, strip_sd_prefix(path), FA_WRITE | FA_CREATE_NEW) != FR_OK) {
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

bool file_fill (char *path, uint8_t value) {
    FIL fil;
    bool error = false;
    uint8_t buffer[FS_SECTOR_SIZE * 8];
    FRESULT res;
    UINT bytes_to_write;
    UINT bytes_written;

    for (int i = 0; i < sizeof(buffer); i++) {
        buffer[i] = value;
    }

    if (f_open(&fil, strip_sd_prefix(path), FA_WRITE) != FR_OK) {
        return true;
    }

    for (int i = 0; i < f_size(&fil); i += sizeof(buffer)) {
        bytes_to_write = MIN(f_size(&fil) - f_tell(&fil), sizeof(buffer));
        res = f_write(&fil, buffer, bytes_to_write, &bytes_written);
        if ((res != FR_OK) || (bytes_to_write != bytes_written)) {
            error = true;
            break;
        }
    }

    if (f_tell(&fil) != f_size(&fil)) {
        error = true;
    }

    if (f_close(&fil) != FR_OK) {
        error = true;
    }

    return error;
}

bool file_get_sectors (char *path, void (*callback) (uint32_t sector_count, uint32_t file_sector, uint32_t cluster_sector, uint32_t cluster_size)) {
    FATFS *fs;
    FIL fil;
    bool error = false;

    if (!callback) {
        return true;
    }

    if (f_open(&fil, strip_sd_prefix(path), FA_READ) != FR_OK) {
        return true;
    }

    fs = fil.obj.fs;

    uint32_t sector_count = (ALIGN(f_size(&fil), FS_SECTOR_SIZE) / FS_SECTOR_SIZE);

    uint32_t cluster_sector = 0;

    for (int file_sector = 0; file_sector < sector_count; file_sector += fs->csize) {
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
        callback(sector_count, file_sector, cluster_sector, fs->csize);
    }

    if (f_close(&fil) != FR_OK) {
        error = true;
    }

    return error;
}

bool file_has_extensions (char *path, const char *extensions[]) {
    char *ext = strrchr(path, '.');

    if (ext == NULL) {
        return false;
    }

    while (*extensions != NULL) {
        if (strcasecmp(ext + 1, *extensions) == 0) {
            return true;
        }
        extensions++;
    }

    return false;
}


bool directory_exists (char *path) {
    FRESULT fr;
    FILINFO fno;

    fr = f_stat(strip_sd_prefix(path), &fno);

    return ((fr == FR_OK) && (fno.fattrib & AM_DIR));
}

bool directory_delete (char *path) {
    if (directory_exists(path)) {
        return (f_unlink(strip_sd_prefix(path)) != FR_OK);
    }

    return false;
}

bool directory_create (char *path) {
    bool error = false;

    if (directory_exists(path)) {
        return false;
    }

    char *directory = strdup(strip_sd_prefix(path));
    char *separator = directory;

    do {
        separator = strchr(separator, '/');

        if (separator != NULL) {
            *separator++ = '\0';
        }

        if (directory[0] != '\0') {
            FRESULT res = f_mkdir(directory);
            if ((res != FR_OK) && (res != FR_EXIST)) {
                error = true;
                break;
            }
        }

        if (separator != NULL) {
            *(separator - 1) = '/';
        }
    } while (separator != NULL);

    free(directory);

    return error;
}
