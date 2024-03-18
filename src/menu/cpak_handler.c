#include <libdragon.h>

#include <fatfs/ff.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/fs.h"
#include "cpak_handler.h"

#define CPAK_BLOCKS 128
#define CPAK_BLOCK_SIZE MEMPAK_BLOCK_SIZE

int clone_pak_content_to_file(char *path, uint8_t port) {
    uint8_t cpak_data[CPAK_BLOCKS * CPAK_BLOCK_SIZE];
    int err;
    for (int i = 0; i < CPAK_BLOCKS; i++) {
        err = read_mempak_sector(port, i, &cpak_data[i * CPAK_BLOCK_SIZE]);
        if (err) {
            return err;
        }
    }

    FIL fil;
    UINT bytes_written;
    if (f_open(&fil, strip_sd_prefix(path), FA_WRITE | FA_CREATE_ALWAYS) != FR_OK) {
        return 1;
    }

    FRESULT fw_err = f_write(&fil, &cpak_data, sizeof(cpak_data), &bytes_written);

    f_close(&fil);

    if (fw_err) {
        return fw_err;
    }
    else {
        return 0;
    }
}

// void overwite_pak_content_from_file(char *path, uint8_t port) {

// }

// void try_repair_pak() {

// }
