#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <fatfs/ff.h>
#include <libdragon.h>

#include "utils/fs.h"
#include "utils/utils.h"

#include "../flashcart_utils.h"
#include "ed64_ll.h"
#include "ed64.h"


// #include "../menu/settings.h"

// // This is a trial hack before using the settings API.
// #ifndef LAST_ROM_FILE_PATH
// #define LAST_ROM_FILE_PATH "/ed64/last_rom.txt"
// #endif


extern int ed_exit(void);

static flashcart_err_t ed64_init (void) {

    // TODO: partly already done, see https://github.com/DragonMinded/libdragon/blob/4ec469d26b6dc4e308caf3d5b86c2b340b708bbd/src/libcart/cart.c#L1064

    // FIXME: Update firmware if needed.
    // FIXME: Enable RTC if available.

    // FIXME: Retrive a config file from the SD card that might have been set.
    // This should include the location of the ROM and its save type.
    // Then, if it is valid, perform a save.
    // FIL lrp_fil;
    // UINT lrp_br;

    // if (f_open(&lrp_fil, LAST_ROM_PATH_FILE, FA_READ) != FR_OK) {
    //     return FLASHCART_ERR_LOAD;
    // }

    // if (f_read(&lrp_fil, lrp_path, 1024, &lrp_br) != FR_OK) {
    //     f_close(&lrp_fil);
    //     return FLASHCART_ERR_LOAD;
    // }

    // if (f_close(&lrp_fil) != FR_OK) {
    //     return FLASHCART_ERR_LOAD;
    // }

    // Now save the content back to the SD!
    
    return FLASHCART_OK;
}

static flashcart_err_t ed64_deinit (void) {

    // For the moment, just use libCart exit.
    ed_exit();

    return FLASHCART_OK;
}

static bool ed64_has_feature (flashcart_features_t feature) {
    switch (feature) {
        case FLASHCART_FEATURE_64DD: return false;
        default: return false;
    }
}

static flashcart_err_t ed64_load_rom (char *rom_path, flashcart_progress_callback_t *progress) {
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(rom_path), FA_READ) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    fix_file_size(&fil);

    size_t rom_size = f_size(&fil);

    // FIXME: if the cart is not V3 or X5 or X7, we need probably need to - 128KiB for save compatibility.
    // Or somehow warn that certain ROM's will have corruption due to the address space being used for saves.
    if (rom_size > MiB(64)) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    size_t sdram_size = rom_size;

    size_t chunk_size = MiB(1);
    for (int offset = 0; offset < sdram_size; offset += chunk_size) {
        size_t block_size = MIN(sdram_size - offset, chunk_size);
        if (f_read(&fil, (void *) (ROM_ADDRESS + offset), block_size, &br) != FR_OK) {
            f_close(&fil);
            return FLASHCART_ERR_LOAD;
        }
        if (progress) {
            progress(f_tell(&fil) / (float) (f_size(&fil)));
        }
    }
    if (f_tell(&fil) != sdram_size) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }


    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    
    // Set the required actions for retriving the save file later.
    // Given there is no good place in RAM...
    // This would involve creating some content to a file on the SD card that includes:
    // the ROM name and location and possibly its save type. This information will be used on init to perform a "save writeback".
    // Actually, we should be using the settings API, so this is just a trial hack.
    // FIL lrp_fil;
    // UINT lrp_bw;
    
    // if (f_open(&lrp_fil, LAST_ROM_FILE_PATH, FA_CREATE_ALWAYS) != FR_OK) {
    //     return FLASHCART_ERR_LOAD;
    // }
    // if (f_write(&lrp_fil, rom_path, strlen(rom_path) + 1, &lrp_bw) != FR_OK) {
    //     f_close(&lrp_fil);
    //     return FLASHCART_ERR_LOAD;
    // }
    // if (f_close(&lrp_fil) != FR_OK) {
    //     return FLASHCART_ERR_LOAD;
    // }

    return FLASHCART_OK;
}

static flashcart_err_t ed64_load_file (char *file_path, uint32_t rom_offset, uint32_t file_offset) {
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(file_path), FA_READ) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    fix_file_size(&fil);

    size_t file_size = f_size(&fil) - file_offset;

    // FIXME: if the cart is not V3 or X5 or X7, we need probably need to - 128KiB for save compatibility.
    // Or somehow warn that certain ROM's will have corruption due to the address space being used for saves.
    if (file_size > (MiB(64) - rom_offset)) {
        f_close(&fil);
        return FLASHCART_ERR_ARGS;
    }

    if (f_lseek(&fil, file_offset) != FR_OK) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (f_read(&fil, (void *) (ROM_ADDRESS + rom_offset), file_size, &br) != FR_OK) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }
    if (br != file_size) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_err_t ed64_load_save (char *save_path) {
    void *address =  NULL;
    ed64_save_type_t type = ed64_ll_get_save_type();

    switch (type) {
        case SAVE_TYPE_EEPROM_4K:
        case SAVE_TYPE_EEPROM_16K:
        case SAVE_TYPE_SRAM:
        case SAVE_TYPE_SRAM_128K:
        case SAVE_TYPE_FLASHRAM:
            address = (void *) (SRAM_ADDRESS);
            break;
        case SAVE_TYPE_NONE:
        default:
            return FLASHCART_ERR_ARGS;
    }

    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(save_path), FA_READ) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    size_t save_size = f_size(&fil);

    if (f_read(&fil, address, save_size, &br) != FR_OK) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    if (br != save_size) {
        return FLASHCART_ERR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_err_t ed64_set_save_type (flashcart_save_type_t save_type) {
    ed64_save_type_t type;

    switch (save_type) {
        case FLASHCART_SAVE_TYPE_NONE:
            type = SAVE_TYPE_NONE;
            break;
        case FLASHCART_SAVE_TYPE_EEPROM_4K:
            type = SAVE_TYPE_EEPROM_4K;
            break;
        case FLASHCART_SAVE_TYPE_EEPROM_16K:
            type = SAVE_TYPE_EEPROM_16K;
            break;
        case FLASHCART_SAVE_TYPE_SRAM:
            type = SAVE_TYPE_SRAM;
            break;
        case FLASHCART_SAVE_TYPE_SRAM_BANKED:
        case FLASHCART_SAVE_TYPE_SRAM_128K:
            type = SAVE_TYPE_SRAM_128K;
            break;
        case FLASHCART_SAVE_TYPE_FLASHRAM_PKST2:
        case FLASHCART_SAVE_TYPE_FLASHRAM:
            type = SAVE_TYPE_FLASHRAM;
            break;
        default:
            return FLASHCART_ERR_ARGS;
    }

    ed64_ll_set_save_type(type);

    return FLASHCART_OK;
}


static flashcart_t flashcart_ed64 = {
    .init = ed64_init,
    .deinit = ed64_deinit,
    .has_feature = ed64_has_feature,
    .load_rom = ed64_load_rom,
    .load_file = ed64_load_file,
    .load_save = ed64_load_save,
    .load_64dd_ipl = NULL,
    .load_64dd_disk = NULL,
    .set_save_type = ed64_set_save_type,
    .set_save_writeback = NULL,
};


flashcart_t *ed64_get_flashcart (void) {
    return &flashcart_ed64;
}
