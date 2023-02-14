#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <fatfs/ff.h>
#include <libdragon.h>

#include "utils/fs.h"
#include "utils/utils.h"

#include "sc64_internal.h"
#include "sc64.h"


#define SRAM_FLASHRAM_ADDRESS   (0x08000000)
#define ROM_ADDRESS             (0x10000000)
#define EXTENDED_ADDRESS        (0x14000000)
#define SHADOW_ADDRESS          (0x1FFC0000)
#define EEPROM_ADDRESS          (0x1FFE2000)


static flashcart_error_t load_to_flash (FIL *fil, void *address, size_t size, UINT *br) {
    size_t erase_block_size;
    UINT bp;

    *br = 0;

    if (sc64_flash_get_erase_block_size(&erase_block_size) != SC64_OK) {
        return FLASHCART_ERROR_INT;
    }

    while (size > 0) {
        size_t program_size = MIN(size, erase_block_size);
        if (sc64_flash_erase_block(address) != SC64_OK) {
            return FLASHCART_ERROR_INT;
        }
        if (f_read(fil, address, program_size, &bp) != FR_OK) {
            return FLASHCART_ERROR_LOAD;
        }
        if (sc64_flash_wait_busy() != SC64_OK) {
            return FLASHCART_ERROR_INT;
        }
        address += program_size;
        size -= program_size;
        *br += bp;
    }

    return FLASHCART_OK;
}


static bool sc64_init (void) {
    sc64_unlock();
    return sc64_check_presence();
}

static void sc64_deinit (void) {
    sc64_lock();
}

static flashcart_error_t sc64_load_rom (char *rom_path) {
    FIL fil;
    UINT br;

    if (f_open(&fil, rom_path, FA_READ) != FR_OK) {
        return FLASHCART_ERROR_LOAD;
    }

    size_t rom_size = ALIGN(f_size(&fil), FS_SECTOR_SIZE);

    if (rom_size > MiB(78)) {
        f_close(&fil);
        return FLASHCART_ERROR_LOAD;
    }

    bool shadow_enabled = (rom_size > (MiB(64) - KiB(128)));
    bool extended_enabled = (rom_size > MiB(64));

    size_t sdram_size = shadow_enabled ? (MiB(64) - KiB(128)) : rom_size;
    size_t shadow_size = shadow_enabled ? MIN(rom_size - sdram_size, KiB(128)) : 0;
    size_t extended_size = extended_enabled ? rom_size - MiB(64) : 0;

    if (f_read(&fil, (void *) (ROM_ADDRESS), sdram_size, &br) != FR_OK) {
        f_close(&fil);
        return FLASHCART_ERROR_LOAD;
    }
    if (br != sdram_size) {
        f_close(&fil);
        return FLASHCART_ERROR_LOAD;
    }

    if (sc64_set_config(CFG_ROM_SHADOW_ENABLE, shadow_enabled) != SC64_OK) {
        f_close(&fil);
        return FLASHCART_ERROR_INT;
    }

    if (shadow_enabled) {
        flashcart_error_t error = load_to_flash(&fil, (void *) (SHADOW_ADDRESS), shadow_size, &br);
        if (error != FLASHCART_OK) {
            f_close(&fil);
            return error;
        }
        if (br != shadow_size) {
            f_close(&fil);
            return FLASHCART_ERROR_LOAD;
        }
    }

    if (sc64_set_config(CFG_ROM_EXTENDED_ENABLE, extended_enabled) != SC64_OK) {
        f_close(&fil);
        return FLASHCART_ERROR_INT;
    }

    if (extended_enabled) {
        flashcart_error_t error = load_to_flash(&fil, (void *) (EXTENDED_ADDRESS), extended_size, &br);
        if (error != FLASHCART_OK) {
            f_close(&fil);
            return error;
        }
        if (br != extended_size) {
            f_close(&fil);
            return FLASHCART_ERROR_LOAD;
        }
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERROR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_error_t sc64_load_save (char *save_path) {
    void *address = NULL;
    sc64_save_type_t type;

    if (sc64_get_config(CFG_SAVE_TYPE, &type) != SC64_OK) {
        return FLASHCART_ERROR_INT;
    }

    switch (type) {
        case SAVE_TYPE_EEPROM_4K:
        case SAVE_TYPE_EEPROM_16K:
            address = (void *) (EEPROM_ADDRESS);
            break;
        case SAVE_TYPE_SRAM:
        case SAVE_TYPE_FLASHRAM:
        case SAVE_TYPE_SRAM_BANKED:
            address = (void *) (SRAM_FLASHRAM_ADDRESS);
            break;
        case SAVE_TYPE_NONE:
        default:
            return FLASHCART_ERROR_ARGS;
    }

    FIL fil;
    UINT br;

    if (f_open(&fil, save_path, FA_READ) != FR_OK) {
        return FLASHCART_ERROR_LOAD;
    }

    size_t save_size = ALIGN(f_size(&fil), FS_SECTOR_SIZE);

    if (f_read(&fil, address, save_size, &br) != FR_OK) {
        f_close(&fil);
        return FLASHCART_ERROR_LOAD;
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERROR_LOAD;
    }

    if (br != save_size) {
        return FLASHCART_ERROR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_error_t sc64_set_save_type (flashcart_save_type_t save_type) {
    sc64_save_type_t type;

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
        default:
            return FLASHCART_ERROR_ARGS;
    }

    if (sc64_set_config(CFG_SAVE_TYPE, type) != SC64_OK) {
        return FLASHCART_ERROR_INT;
    }

    return FLASHCART_OK;
}

static flashcart_error_t sc64_set_save_writeback (uint32_t *sectors) {
    sc64_write_data(sectors, SC64_BUFFERS->BUFFER, 1024);

    if (sc64_writeback_enable(SC64_BUFFERS->BUFFER) != SC64_OK) {
        return FLASHCART_ERROR_INT;
    }

    return FLASHCART_OK;
}


static flashcart_t flashcart_sc64 = {
    .init = sc64_init,
    .deinit = sc64_deinit,
    .load_rom = sc64_load_rom,
    .load_save = sc64_load_save,
    .set_save_type = sc64_set_save_type,
    .set_save_writeback = sc64_set_save_writeback,
};


flashcart_t *sc64_get_flashcart (void) {
    return &flashcart_sc64;
}
