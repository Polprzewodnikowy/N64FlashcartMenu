#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <fatfs/ff.h>
#include <libdragon.h>

#include "utils/fs.h"
#include "utils/utils.h"

#include "../flashcart_utils.h"
#include "sc64_ll.h"
#include "sc64.h"


#define SRAM_FLASHRAM_ADDRESS       (0x08000000)
#define ROM_ADDRESS                 (0x10000000)
#define EXTENDED_ADDRESS            (0x14000000)
#define SHADOW_ADDRESS              (0x1FFC0000)
#define EEPROM_ADDRESS              (0x1FFE2000)

#define SUPPORTED_MAJOR_VERSION     (2)
#define SUPPORTED_MINOR_VERSION     (16)
#define SUPPORTED_REVISION          (0)


static flashcart_error_t load_to_flash (FIL *fil, void *address, size_t size, UINT *br, flashcart_progress_callback_t *progress) {
    size_t erase_block_size;
    UINT bp;

    *br = 0;

    if (sc64_ll_flash_get_erase_block_size(&erase_block_size) != SC64_OK) {
        return FLASHCART_ERROR_INT;
    }

    while (size > 0) {
        size_t program_size = MIN(size, erase_block_size);
        if (sc64_ll_flash_erase_block(address) != SC64_OK) {
            return FLASHCART_ERROR_INT;
        }
        if (f_read(fil, address, program_size, &bp) != FR_OK) {
            return FLASHCART_ERROR_LOAD;
        }
        if (sc64_ll_flash_wait_busy() != SC64_OK) {
            return FLASHCART_ERROR_INT;
        }
        if (progress) {
            progress(f_tell(fil) / (float) (f_size(fil)));
        }
        address += program_size;
        size -= program_size;
        *br += bp;
    }

    return FLASHCART_OK;
}

static flashcart_error_t sc64_init (void) {
    uint16_t major;
    uint16_t minor;
    uint32_t revision;

    if (sc64_ll_get_version(&major, &minor, &revision) != SC64_OK) {
        return FLASHCART_ERROR_OUTDATED;
    }
    if (major != SUPPORTED_MAJOR_VERSION) {
        return FLASHCART_ERROR_OUTDATED;
    }
    if (minor < SUPPORTED_MINOR_VERSION) {
        return FLASHCART_ERROR_OUTDATED;
    } else if (minor == SUPPORTED_MINOR_VERSION && revision < SUPPORTED_REVISION) {
        return FLASHCART_ERROR_OUTDATED;
    }

    bool writeback_pending;
    do {
        if (sc64_ll_writeback_pending(&writeback_pending) != SC64_OK) {
            return FLASHCART_ERROR_INT;
        }
    } while (writeback_pending);

    const struct {
        sc64_cfg_id_t id;
        uint32_t value;
    } default_config[] = {
        { CFG_ID_BOOTLOADER_SWITCH, false },
        { CFG_ID_ROM_WRITE_ENABLE, true },
        { CFG_ID_ROM_SHADOW_ENABLE, false },
        { CFG_ID_DD_MODE, DD_MODE_DISABLED },
        { CFG_ID_ISV_ADDRESS, 0x00000000 },
        { CFG_ID_BOOT_MODE, BOOT_MODE_MENU },
        { CFG_ID_SAVE_TYPE, SAVE_TYPE_NONE },
        { CFG_ID_CIC_SEED, CIC_SEED_AUTO },
        { CFG_ID_TV_TYPE, TV_TYPE_PASSTHROUGH },
        { CFG_ID_DD_SD_ENABLE, false },
        { CFG_ID_DD_DRIVE_TYPE, DRIVE_TYPE_RETAIL },
        { CFG_ID_DD_DISK_STATE, DISK_STATE_EJECTED },
        { CFG_ID_BUTTON_MODE, BUTTON_MODE_NONE },
        { CFG_ID_ROM_EXTENDED_ENABLE, false },
    };

    for (int i = 0; i < sizeof(default_config) / sizeof(default_config[0]); i++) {
        if (sc64_ll_set_config(default_config[i].id, default_config[i].value) != SC64_OK) {
            return FLASHCART_ERROR_INT;
        }
    }

    return FLASHCART_OK;
}

static flashcart_error_t sc64_deinit (void) {
    sc64_ll_set_config(CFG_ID_ROM_WRITE_ENABLE, false);

    sc64_ll_lock();

    return FLASHCART_OK;
}

static flashcart_error_t sc64_load_rom (char *rom_path, flashcart_progress_callback_t *progress) {
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(rom_path), FA_READ) != FR_OK) {
        return FLASHCART_ERROR_LOAD;
    }

    fix_file_size(&fil);

    size_t rom_size = f_size(&fil);

    if (rom_size > MiB(78)) {
        f_close(&fil);
        return FLASHCART_ERROR_LOAD;
    }

    bool shadow_enabled = (rom_size > (MiB(64) - KiB(128)));
    bool extended_enabled = (rom_size > MiB(64));

    size_t sdram_size = shadow_enabled ? (MiB(64) - KiB(128)) : rom_size;
    size_t shadow_size = shadow_enabled ? MIN(rom_size - sdram_size, KiB(128)) : 0;
    size_t extended_size = extended_enabled ? rom_size - MiB(64) : 0;

    size_t chunk_size = MiB(1);
    for (int offset = 0; offset < sdram_size; offset += chunk_size) {
        size_t block_size = MIN(sdram_size - offset, chunk_size);
        if (f_read(&fil, (void *) (ROM_ADDRESS + offset), block_size, &br) != FR_OK) {
            f_close(&fil);
            return FLASHCART_ERROR_LOAD;
        }
        if (progress) {
            progress(f_tell(&fil) / (float) (f_size(&fil)));
        }
    }
    if (f_tell(&fil) != sdram_size) {
        f_close(&fil);
        return FLASHCART_ERROR_LOAD;
    }

    if (sc64_ll_set_config(CFG_ID_ROM_SHADOW_ENABLE, shadow_enabled) != SC64_OK) {
        f_close(&fil);
        return FLASHCART_ERROR_INT;
    }

    if (shadow_enabled) {
        flashcart_error_t error = load_to_flash(&fil, (void *) (SHADOW_ADDRESS), shadow_size, &br, progress);
        if (error != FLASHCART_OK) {
            f_close(&fil);
            return error;
        }
        if (br != shadow_size) {
            f_close(&fil);
            return FLASHCART_ERROR_LOAD;
        }
    }

    if (sc64_ll_set_config(CFG_ID_ROM_EXTENDED_ENABLE, extended_enabled) != SC64_OK) {
        f_close(&fil);
        return FLASHCART_ERROR_INT;
    }

    if (extended_enabled) {
        flashcart_error_t error = load_to_flash(&fil, (void *) (EXTENDED_ADDRESS), extended_size, &br, progress);
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

static flashcart_error_t sc64_load_file (char *file_path, uint32_t rom_offset, uint32_t file_offset) {
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_sd_prefix(file_path), FA_READ) != FR_OK) {
        return FLASHCART_ERROR_LOAD;
    }

    fix_file_size(&fil);

    size_t file_size = f_size(&fil) - file_offset;

    if (file_size > (MiB(64) - rom_offset)) {
        f_close(&fil);
        return FLASHCART_ERROR_ARGS;
    }

    if (f_lseek(&fil, file_offset) != FR_OK) {
        f_close(&fil);
        return FLASHCART_ERROR_LOAD;
    }

    if (f_read(&fil, (void *) (ROM_ADDRESS + rom_offset), file_size, &br) != FR_OK) {
        f_close(&fil);
        return FLASHCART_ERROR_LOAD;
    }
    if (br != file_size) {
        f_close(&fil);
        return FLASHCART_ERROR_LOAD;
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERROR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_error_t sc64_load_save (char *save_path) {
    void *address = NULL;
    uint32_t value;

    if (sc64_ll_get_config(CFG_ID_SAVE_TYPE, &value) != SC64_OK) {
        return FLASHCART_ERROR_INT;
    }

    sc64_save_type_t type = (sc64_save_type_t) (value);

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

    if (f_open(&fil, strip_sd_prefix(save_path), FA_READ) != FR_OK) {
        return FLASHCART_ERROR_LOAD;
    }

    size_t save_size = f_size(&fil);

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
            type = SAVE_TYPE_SRAM_128K;
            break;
        case FLASHCART_SAVE_TYPE_FLASHRAM:
            type = SAVE_TYPE_FLASHRAM;
            break;
        case FLASHCART_SAVE_TYPE_FLASHRAM_PKST2:
            type = SAVE_TYPE_FLASHRAM;
            break;
        default:
            return FLASHCART_ERROR_ARGS;
    }

    if (sc64_ll_set_config(CFG_ID_SAVE_TYPE, type) != SC64_OK) {
        return FLASHCART_ERROR_INT;
    }

    return FLASHCART_OK;
}

static flashcart_error_t sc64_set_save_writeback (uint32_t *sectors) {
    pi_dma_write_data(sectors, SC64_BUFFERS->BUFFER, 1024);

    if (sc64_ll_writeback_enable(SC64_BUFFERS->BUFFER) != SC64_OK) {
        return FLASHCART_ERROR_INT;
    }

    return FLASHCART_OK;
}


static flashcart_t flashcart_sc64 = {
    .init = sc64_init,
    .deinit = sc64_deinit,
    .load_rom = sc64_load_rom,
    .load_file = sc64_load_file,
    .load_save = sc64_load_save,
    .set_save_type = sc64_set_save_type,
    .set_save_writeback = sc64_set_save_writeback,
};


flashcart_t *sc64_get_flashcart (void) {
    return &flashcart_sc64;
}
