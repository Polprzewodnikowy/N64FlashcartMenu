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


extern int ed_exit(void);

static flashcart_err_t ed64_init (void) {

    // TODO: partly already done, see https://github.com/DragonMinded/libdragon/blob/4ec469d26b6dc4e308caf3d5b86c2b340b708bbd/src/libcart/cart.c#L1064

    //ed64_ll_unlock_regs();

    // ed64_ll_init(); // FIXME: this causes a crash!
    // ed64_ll_reset_spx(); // only required if V2+ FW is updating!

    // ed64_ll_set_ram_bank(1); // Seemingly bank 0 (for SRAM 128k) is only supported on V3 and it is not the default.
    
    // If a V3 or X7 cart is detected (has a battery?), enable RTC, 
    // ed64_ll_gpio_mode_rtc();
    // otherwise:
    // ed64_ll_gpio_mode_io();
    
    // if (fpga_revision < SUPPORTED_FPGA_REVISION) {
    //     return FLASHCART_ERR_OUTDATED;
    // }


    return FLASHCART_OK;
}

static flashcart_err_t ed64_deinit (void) {

    // ed64_ll_gpio_mode_off(); // On V3 or X7, this should be ed64_bios_gpio_mode_rtc() if it is required.

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

    // FIXME: if the cart is not V3 or X5 or X7, we probably need to - 128KiB
    if (rom_size > MiB(64)) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }


    size_t sdram_size = MiB(64);

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
    if (f_tell(&fil) != rom_size) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }


    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

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

    // FIXME: if the cart is not V3 or X5 or X7, we probably need to - 128KiB
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
    // TODO: look at d64_load_save() and adjust.
    uint8_t eeprom_contents[KiB(2)] __attribute__((aligned(8)));
    void *address =  NULL;
    ed64_save_type_t current_save_type = ed64_ll_get_save_type();

    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(save_path), FA_CREATE_ALWAYS | FA_READ) != FR_OK) { // FIXME: create always is a hack to ensure we always have a valid save.
        return FLASHCART_ERR_LOAD;
    }

    size_t save_size = f_size(&fil);

    switch (current_save_type) {
        case SAVE_TYPE_EEPROM_4K:
        case SAVE_TYPE_EEPROM_16K:
            address = eeprom_contents;
            break;
        case SAVE_TYPE_SRAM:
        case SAVE_TYPE_SRAM_128K:
        case SAVE_TYPE_FLASHRAM:
            address = (void *) (SRAM_ADDRESS);
            break;
        case SAVE_TYPE_NONE:
        default:
            return FLASHCART_ERR_ARGS;
    }

    if(f_read(&fil, address, save_size, &br) != FR_OK) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (eeprom_present()) {
        if (current_save_type == SAVE_TYPE_EEPROM_16K) { // FIXME: could use save_size instead?!
            eeprom_write_bytes(eeprom_contents, 0, KiB(2));
        }
        else {
            eeprom_write_bytes(eeprom_contents, 0, KiB(1));
        }
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

    // TODO: set pesudo save writeback.
    // as a start, create the file?!

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
