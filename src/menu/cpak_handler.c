#include <libdragon.h>

#include <fatfs/ff.h>
#include "../utils/fs.h"
#include "cpak_handler.h"

#define CPAK_BLOCKS 128
#define CPAK_BLOCK_SIZE MEMPAK_BLOCK_SIZE

cpak_err_t cpak_info_load(uint8_t port, cpak_info_t *cpak_info)
{
    int res = validate_mempak(port);
    if (res != CONTROLLER_PAK_OK) {
        return CONTROLLER_PAK_ERR_INVALID;
    }

    for (int j = 0; j < 16; j++)
    {
        entry_structure_t entry;

        get_mempak_entry(port, j, &entry);

        cpak_info->entries[j] = entry;
    }

    cpak_info->free_space = get_mempak_free_space(port);

    return CONTROLLER_PAK_OK;
}

int cpak_clone_contents_to_file(char *path, uint8_t port) {
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
        return CONTROLLER_PAK_ERR_IO;
    }

    FRESULT fwrite_err = f_write(&fil, &cpak_data, sizeof(cpak_data), &bytes_written);

    f_close(&fil);

    if (fwrite_err) {
        return fwrite_err;
    }
    else {
        return CONTROLLER_PAK_OK;
    }
}

cpak_err_t cpak_overwrite_contents_from_file(char *path, uint8_t port) {
    // TODO: implementation.
    return CONTROLLER_PAK_ERR_IO;
}

cpak_err_t cpak_attempt_repair(uint8_t port, bool allow_formatting) {
    int result = validate_mempak(port);
    if (result == CONTROLLER_PAK_OK) {
        return CONTROLLER_PAK_OK;
    }

    // TODO: repair implementation.

    // last resort would be to format it completely!
    if (result == -3 && allow_formatting) {
        result = format_mempak(port);
    }
    return result;
}
