#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <fatfs/ff.h>
#include <libdragon.h>

#include "utils/fs.h"
#include "utils/utils.h"

#include "../flashcart_utils.h"
#include "64drive_ll.h"
#include "64drive.h"


#define ROM_ADDRESS                 (0x10000000)
#define SAVE_ADDRESS_DEV_A          (0x11FF0000)
#define SAVE_ADDRESS_DEV_A_PKST2    (0x10B032B0)
#define SAVE_ADDRESS_DEV_B          (0x17FE0000)

#define SUPPORTED_FPGA_REVISION     (205)


static size_t sdram_size = 0;
static d64_device_variant_t device_variant = DEVICE_VARIANT_UNKNOWN;
static d64_save_type_t current_save_type = SAVE_TYPE_NONE;
static bool enable_extended_mode_on_exit = false;


static flashcart_error_t d64_init (void) {
    uint16_t fpga_revision;
    uint32_t bootloader_version;

    if (d64_ll_enable_extended_mode(false)) {
        return FLASHCART_ERROR_INT;
    }

    if (d64_ll_get_version(&device_variant, &fpga_revision, &bootloader_version)) {
        return FLASHCART_ERROR_INT;
    }

    if (fpga_revision < SUPPORTED_FPGA_REVISION) {
        return FLASHCART_ERROR_OUTDATED;
    }

    if (d64_ll_get_sdram_size(&sdram_size)) {
        return FLASHCART_ERROR_INT;
    }

    if (d64_ll_enable_save_writeback(false)) {
        return FLASHCART_ERROR_INT;
    }

    if (d64_ll_set_save_type(SAVE_TYPE_NONE)) {
        return FLASHCART_ERROR_INT;
    }

    current_save_type = SAVE_TYPE_NONE;

    if (d64_ll_enable_cartrom_writes(false)) {
        return FLASHCART_ERROR_INT;
    }

    return FLASHCART_OK;
}

static flashcart_error_t d64_deinit (void) {
    if (enable_extended_mode_on_exit) {
        if (d64_ll_enable_extended_mode(true)) {
            return FLASHCART_ERROR_INT;
        }
    }

    return FLASHCART_OK;
}

static flashcart_error_t d64_load_rom (char *rom_path) {
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(rom_path), FA_READ) != FR_OK) {
        return FLASHCART_ERROR_LOAD;
    }

    fix_file_size(&fil);

    size_t rom_size = f_size(&fil);

    if (rom_size > sdram_size) {
        f_close(&fil);
        return FLASHCART_ERROR_LOAD;
    }

    if (f_read(&fil, (void *) (ROM_ADDRESS), rom_size, &br) != FR_OK) {
        f_close(&fil);
        return FLASHCART_ERROR_LOAD;
    }
    if (br != rom_size) {
        f_close(&fil);
        return FLASHCART_ERROR_LOAD;
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERROR_LOAD;
    }

    if (rom_size > MiB(64)) {
        enable_extended_mode_on_exit = true;
    }

    return FLASHCART_OK;
}

static flashcart_error_t d64_load_save (char *save_path) {
    uint8_t eeprom_contents[2048] __attribute__((aligned(8)));
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(save_path), FA_READ) != FR_OK) {
        return FLASHCART_ERROR_LOAD;
    }

    size_t save_size = f_size(&fil);

    bool is_eeprom_save = (current_save_type == SAVE_TYPE_EEPROM_4K || current_save_type == SAVE_TYPE_EEPROM_16K);

    void *address = (void *) (SAVE_ADDRESS_DEV_B);
    if (is_eeprom_save) {
        address = eeprom_contents;
    } else if (device_variant == DEVICE_VARIANT_A) {
        address = (void *) (SAVE_ADDRESS_DEV_A);
        if (current_save_type == SAVE_TYPE_FLASHRAM_PKST2) {
            address = (void *) (SAVE_ADDRESS_DEV_A_PKST2);
        }
    }

    if (f_read(&fil, address, save_size, &br) != FR_OK) {
        f_close(&fil);
        return FLASHCART_ERROR_LOAD;
    }

    if (is_eeprom_save) {
        if (d64_ll_write_eeprom_contents(eeprom_contents)) {
            f_close(&fil);
            return FLASHCART_ERROR_LOAD;
        }
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERROR_LOAD;
    }

    if (br != save_size) {
        return FLASHCART_ERROR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_error_t d64_set_save_type (flashcart_save_type_t save_type) {
    d64_save_type_t type;

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
            type = SAVE_TYPE_SRAM_BANKED;
            break;
        case FLASHCART_SAVE_TYPE_SRAM_128K:
            type = SAVE_TYPE_SRAM;
            break;
        case FLASHCART_SAVE_TYPE_FLASHRAM:
            type = SAVE_TYPE_FLASHRAM;
            break;
        case FLASHCART_SAVE_TYPE_FLASHRAM_PKST2:
            type = (device_variant == DEVICE_VARIANT_A) ? SAVE_TYPE_FLASHRAM_PKST2 : SAVE_TYPE_FLASHRAM;
            break;
        default:
            return FLASHCART_ERROR_ARGS;
    }

    if (d64_ll_enable_save_writeback(false)) {
        return FLASHCART_ERROR_INT;
    }

    if (d64_ll_set_save_type(type)) {
        return FLASHCART_ERROR_INT;
    }

    current_save_type = type;

    return FLASHCART_OK;
}

static flashcart_error_t d64_set_save_writeback (uint32_t *sectors) {
    if (d64_ll_write_save_writeback_lba_list(sectors)) {
        return FLASHCART_ERROR_INT;
    }

    if (d64_ll_enable_save_writeback(true)) {
        return FLASHCART_ERROR_INT;
    }

    return FLASHCART_OK;
}


static flashcart_t flashcart_d64 = {
    .init = d64_init,
    .deinit = d64_deinit,
    .load_rom = d64_load_rom,
    .load_save = d64_load_save,
    .set_save_type = d64_set_save_type,
    .set_save_writeback = d64_set_save_writeback,
};


flashcart_t *d64_get_flashcart (void) {
    return &flashcart_d64;
}
