#include <libdragon.h>

#include <fatfs/ff.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/fs.h"
#include "cpak_handler.h"

#define PAK_BLOCKS 128
uint8_t pak_data[PAK_BLOCKS * MEMPAK_BLOCK_SIZE];

int clone_pak_content_to_file(char *path, uint8_t port) {
    int err;
    for (int i = 0; i < PAK_BLOCKS; i++) {
        err = read_mempak_sector(port, i, &pak_data[i * MEMPAK_BLOCK_SIZE]);
        if (err) {
            return err;
        }
    }

    FIL fil;
    UINT bytes_written;
    if (f_open(&fil, strip_sd_prefix(path), FA_WRITE | FA_CREATE_ALWAYS) != FR_OK) {
        return 1;
    }

    FRESULT fw_err = f_write(&fil, &pak_data, sizeof(pak_data), &bytes_written);

    f_close(&fil);

    if (fw_err) {
        return fw_err;
    }
    else {
        return 0;
    }
}
