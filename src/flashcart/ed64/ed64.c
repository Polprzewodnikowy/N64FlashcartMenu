#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <fatfs/ff.h>
#include <libdragon.h>
#include <libcart/cart.h>

#include "utils/fs.h"
#include "utils/utils.h"

#include "../flashcart_utils.h"
#include "ed64_ll.h"
#include "ed64.h"
#include "ed64_state.h"


static ed64_pseudo_writeback_t current_state;

extern int ed_exit (void);

static flashcart_err_t ed64_init (void) {

    // TODO: partly already done, see https://github.com/DragonMinded/libdragon/blob/4ec469d26b6dc4e308caf3d5b86c2b340b708bbd/src/libcart/cart.c#L1064

    // FIXME: Update firmware if needed.
    // FIXME: Enable RTC if available.

    // older everdrives cannot save during gameplay so we need to the reset method.
    // works by checking if a file exists.

    ed64_state_load(&current_state);

    if (current_state.is_expecting_save_writeback == true) {

        // make sure next boot doesnt trigger the check changing its state.
        current_state.is_expecting_save_writeback = false;
        ed64_state_save(&current_state);

        // Now save the content back to the SD card!
        FIL fil;
        UINT bw;
        uint8_t cartsave_data[KiB(128)];

        // find the path to last save
        if (file_exists(strip_sd_prefix(current_state.last_save_path))) {

            int save_size = file_get_size(strip_sd_prefix(current_state.last_save_path));

            if ((f_open(&fil, strip_sd_prefix(current_state.last_save_path), FA_CREATE_ALWAYS | FA_READ | FA_WRITE)) != FR_OK) {
                return FLASHCART_ERR_LOAD;
            }

            // everdrive doesn't care about the save type other than flash sram and eeprom
            // so minus flashram we can just check the size
            if (current_state.is_fram_save_type == true) { // flashram is bugged atm
               ed64_ll_get_fram(cartsave_data, save_size);
              // deletes flag
              current_state.is_fram_save_type = false;
              ed64_state_save(&current_state);
            }
            else if (save_size > KiB(32)) { // sram 128
               ed64_ll_get_sram(cartsave_data, save_size);
            }
            else if (save_size > KiB(2)) { // sram
               ed64_ll_get_sram(cartsave_data, save_size);
            }
            else { // eeprom
               ed64_ll_get_eeprom(cartsave_data, save_size);
            }

            if (f_write(&fil, cartsave_data, save_size, &bw) != FR_OK) {
                return FLASHCART_ERR_LOAD;
            }

            if (f_close(&fil) != FR_OK) {
                return FLASHCART_ERR_LOAD;
            }
        }
        else {
            current_state.is_expecting_save_writeback = false;
            current_state.is_fram_save_type = false;
            current_state.last_save_path = "";
            ed64_state_save(&current_state);
        }
    }
    return FLASHCART_OK;
}

static flashcart_err_t ed64_deinit (void) {

    // For the moment, just use libCart exit.
    ed_exit();

    return FLASHCART_OK;
}

static bool ed64_has_feature (flashcart_features_t feature) {
    switch (feature) {
        case FLASHCART_FEATURE_64DD:
            return false;
        default:
            return false;
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
    // Conker's Bad Fur Day doesn't have this issue because eeprom data is at a fixed address in pif ram.
    if (rom_size > MiB(64)) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (rom_size == MiB(64)) {
        ed64_save_type_t type = ed64_ll_get_save_type();
        switch (type) {
            case SAVE_TYPE_SRAM:
                rom_size -= KiB(32);
                break;
            case SAVE_TYPE_SRAM_128K:
            case SAVE_TYPE_FLASHRAM:
                rom_size -= KiB(128);
                break;
            case SAVE_TYPE_DD64_CART_PORT:
                break;
            default:
                break;
        }
    }
    
    size_t sdram_size = rom_size;

    size_t chunk_size = MiB(1);
    for (int offset = 0; offset < sdram_size; offset += chunk_size)
    {
        size_t block_size = MIN(sdram_size - offset, chunk_size);
        if (f_read(&fil, (void *)(ROM_ADDRESS + offset), block_size, &br) != FR_OK) {
            f_close(&fil);
            return FLASHCART_ERR_LOAD;
        }
        if (progress) {
            progress(f_tell(&fil) / (float)(f_size(&fil)));
        }
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_err_t ed64_load_file (char *file_path, uint32_t rom_offset, uint32_t file_offset)
{
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

    if (f_read(&fil, (void *)(ROM_ADDRESS + rom_offset), file_size, &br) != FR_OK) {
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
    
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(save_path), FA_READ) != FR_OK) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    size_t save_size = file_get_size(strip_sd_prefix(save_path));
    uint8_t cartsave_data[save_size];

    if (f_read(&fil, cartsave_data, save_size, &br) != FR_OK) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    current_state.is_fram_save_type = false;

    ed64_save_type_t type = ed64_ll_get_save_type();
    switch (type) {
        case SAVE_TYPE_EEPROM_4K:
        case SAVE_TYPE_EEPROM_16K:
            ed64_ll_set_eeprom(cartsave_data, save_size);
            break;
        case SAVE_TYPE_SRAM:
            ed64_ll_set_sram(cartsave_data, save_size);
        case SAVE_TYPE_SRAM_128K:
            ed64_ll_set_sram_128(cartsave_data, KiB(128));
            break;
        case SAVE_TYPE_FLASHRAM:
            ed64_ll_set_fram(cartsave_data, KiB(128));
            // a cold and warm boot has no way of seeing save types and most types can be determined by size
            // this tells the cart to use flash instead of sram 128 since they are the same size
            current_state.is_fram_save_type = true;
            ed64_state_save(&current_state);
            break;
        case SAVE_TYPE_DD64_CART_PORT:
            break;
        default:
            break;
    }


    current_state.last_save_path = save_path;
    current_state.is_expecting_save_writeback = true;
    ed64_state_save(&current_state);

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
