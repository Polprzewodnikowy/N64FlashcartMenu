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
        return found + strlen(prefix);
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

bool file_get_sectors (char *path, uint32_t *sectors, size_t entries) {
    FATFS *fs;
    FIL fil;
    bool error = false;

    for (int i = 0; i < entries; i++) {
        sectors[i] = 0;
    }

    if (f_open(&fil, strip_sd_prefix(path), FA_READ) != FR_OK) {
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

        if ((strlen(directory) > 0) && (!directory_exists(directory))) {
            if (f_mkdir(directory) != FR_OK) {
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
