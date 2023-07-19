#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <fatfs/ff.h>
#include <libdragon.h>

#include "utils/fs.h"
#include "utils/utils.h"

#include "ed64_internal.h"
#include "ed64.h"


// #define SRAM_FLASHRAM_ADDRESS       (0x08000000)
// #define ROM_ADDRESS                 (0x10000000)
// #define EXTENDED_ADDRESS            (0x14000000)
// #define SHADOW_ADDRESS              (0x1FFC0000)
// #define EEPROM_ADDRESS              (0x1FFE2000)

// #define SUPPORTED_MAJOR_VERSION     (2)
// #define SUPPORTED_MINOR_VERSION     (12)


// static flashcart_error_t load_to_flash (FIL *fil, void *address, size_t size, UINT *br) {
    // size_t erase_block_size;
    // UINT bp;

    // *br = 0;

    // if (ed64_flash_get_erase_block_size(&erase_block_size) != ED64_OK) {
    //     return FLASHCART_ERROR_INT;
    // }

    // while (size > 0) {
    //     size_t program_size = MIN(size, erase_block_size);
    //     if (ed64_flash_erase_block(address) != ED64_OK) {
    //         return FLASHCART_ERROR_INT;
    //     }
    //     if (f_read(fil, address, program_size, &bp) != FR_OK) {
    //         return FLASHCART_ERROR_LOAD;
    //     }
    //     if (ed64_flash_wait_busy() != ED64_OK) {
    //         return FLASHCART_ERROR_INT;
    //     }
    //     address += program_size;
    //     size -= program_size;
    //     *br += bp;
    // }

//     return FLASHCART_OK;
// }

// static void load_cleanup (FIL *fil) {
//     // ed64_sd_set_byte_swap(false);
//     f_close(fil);
// }


static flashcart_error_t ed64_init (void) {
    // uint16_t major;
    // uint16_t minor;

    // HACK: Because libcart reads PI config from address 0x10000000 when initializing
    //       we need to write safe value before running any libcart function.
    //       Data in SDRAM can be undefined at this point and result in setting incorrect PI config.
    // extern uint32_t cart_dom1;
    // cart_dom1 = 0x80371240;

    // ed64_bios_unlock_regs();

    ed64_bios_init();

    // if (!ed64_check_presence()) {
    //     return FLASHCART_ERROR_UNSUPPORTED;
    // }
    // if (ed64_get_version(&major, &minor) != ED64_OK) {
    //     return FLASHCART_ERROR_OUTDATED;
    // }
    // if (major != SUPPORTED_MAJOR_VERSION) {
    //     return FLASHCART_ERROR_OUTDATED;
    // }
    // if (minor < SUPPORTED_MINOR_VERSION) {
    //     return FLASHCART_ERROR_OUTDATED;
    // }

    // bool writeback_pending;
    // do {
    //     if (ed64_writeback_pending(&writeback_pending) != ED64_OK) {
    //         return FLASHCART_ERROR_INT;
    //     }
    // } while (writeback_pending);

    // const struct {
    //     ed64_cfg_t id;
    //     uint32_t value;
    // } default_config[] = {
    //     { CFG_BOOTLOADER_SWITCH, false },
    //     { CFG_ROM_WRITE_ENABLE, false },
    //     { CFG_ROM_SHADOW_ENABLE, false },
    //     { CFG_DD_MODE, DD_MODE_DISABLED },
    //     { CFG_ISV_ADDRESS, 0x00000000 },
    //     { CFG_BOOT_MODE, BOOT_MODE_MENU },
    //     { CFG_SAVE_TYPE, SAVE_TYPE_NONE },
    //     { CFG_CIC_SEED, CIC_SEED_AUTO },
    //     { CFG_TV_TYPE, TV_TYPE_PASSTHROUGH },
    //     { CFG_DD_SD_ENABLE, false },
    //     { CFG_DD_DRIVE_TYPE, DRIVE_TYPE_RETAIL },
    //     { CFG_DD_DISK_STATE, DISK_STATE_EJECTED },
    //     { CFG_BUTTON_MODE, BUTTON_MODE_NONE },
    //     { CFG_ROM_EXTENDED_ENABLE, false },
    // };

    // for (int i = 0; i < sizeof(default_config) / sizeof(default_config[0]); i++) {
    //     if (ed64_set_config(default_config[i].id, default_config[i].value) != ED64_OK) {
    //         return FLASHCART_ERROR_INT;
    //     }
    // }

    return FLASHCART_OK;
}

static flashcart_error_t ed64_deinit (void) {
    // // NOTE: Necessary because libcart enables ROM write by default
    // ed64_set_config(CFG_ROM_WRITE_ENABLE, false);

    ed64_bios_lock_regs();

    return FLASHCART_OK;
}

static flashcart_error_t ed64_load_rom (char *rom_path, bool byte_swap) {
    FIL fil;
    // UINT br;

    if (f_open(&fil, rom_path, FA_READ) != FR_OK) {
        return FLASHCART_ERROR_LOAD;
    }

    // HACK: Align file size to the SD sector size to prevent FatFs from doing partial sector load.
    //       We are relying on direct transfer from SD to SDRAM without CPU intervention.
    //       Sending some extra bytes isn't an issue here.
    fil.obj.objsize = ALIGN(f_size(&fil), FS_SECTOR_SIZE);

    size_t rom_size = f_size(&fil);

    if (rom_size > MiB(78)) {
        f_close(&fil);
        return FLASHCART_ERROR_LOAD;
    }

    // if (ed64_sd_set_byte_swap(byte_swap) != ED64_OK) {
    //     load_cleanup(&fil);
    //     return FLASHCART_ERROR_INT;
    // }

    // bool shadow_enabled = (rom_size > (MiB(64) - KiB(128)));
    // bool extended_enabled = (rom_size > MiB(64));

    // size_t sdram_size = shadow_enabled ? (MiB(64) - KiB(128)) : rom_size;
    // size_t shadow_size = shadow_enabled ? MIN(rom_size - sdram_size, KiB(128)) : 0;
    // size_t extended_size = extended_enabled ? rom_size - MiB(64) : 0;

    // if (f_read(&fil, (void *) (ROM_ADDRESS), sdram_size, &br) != FR_OK) {
    //     load_cleanup(&fil);
    //     return FLASHCART_ERROR_LOAD;
    // }
    // if (br != sdram_size) {
    //     load_cleanup(&fil);
    //     return FLASHCART_ERROR_LOAD;
    // }

    // if (ed64_set_config(CFG_ROM_SHADOW_ENABLE, shadow_enabled) != ED64_OK) {
    //     load_cleanup(&fil);
    //     return FLASHCART_ERROR_INT;
    // }

    // if (shadow_enabled) {
    //     flashcart_error_t error = load_to_flash(&fil, (void *) (SHADOW_ADDRESS), shadow_size, &br);
    //     if (error != FLASHCART_OK) {
    //         load_cleanup(&fil);
    //         return error;
    //     }
    //     if (br != shadow_size) {
    //         load_cleanup(&fil);
    //         return FLASHCART_ERROR_LOAD;
    //     }
    // }

    // if (ed64_set_config(CFG_ROM_EXTENDED_ENABLE, extended_enabled) != ED64_OK) {
    //     load_cleanup(&fil);
    //     return FLASHCART_ERROR_INT;
    // }

    // if (extended_enabled) {
    //     flashcart_error_t error = load_to_flash(&fil, (void *) (EXTENDED_ADDRESS), extended_size, &br);
    //     if (error != FLASHCART_OK) {
    //         load_cleanup(&fil);
    //         return error;
    //     }
    //     if (br != extended_size) {
    //         load_cleanup(&fil);
    //         return FLASHCART_ERROR_LOAD;
    //     }
    // }

    // if (ed64_sd_set_byte_swap(false) != ED64_OK) {
    //     load_cleanup(&fil);
    //     return FLASHCART_ERROR_INT;
    // }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERROR_LOAD;
    }

    return FLASHCART_OK;
}

static flashcart_error_t ed64_load_save (char *save_path) {
//     void *address = NULL;
//     ed64_save_type_t type;

//     if (ed64_get_config(CFG_SAVE_TYPE, &type) != ED64_OK) {
//         return FLASHCART_ERROR_INT;
//     }

//     switch (type) {
//         case SAVE_TYPE_EEPROM_4K:
//         case SAVE_TYPE_EEPROM_16K:
//             address = (void *) (EEPROM_ADDRESS);
//             break;
//         case SAVE_TYPE_SRAM:
//         case SAVE_TYPE_FLASHRAM:
//         case SAVE_TYPE_SRAM_BANKED:
//             address = (void *) (SRAM_FLASHRAM_ADDRESS);
//             break;
//         case SAVE_TYPE_NONE:
//         default:
//             return FLASHCART_ERROR_ARGS;
//     }

//     FIL fil;
//     UINT br;

//     if (f_open(&fil, save_path, FA_READ) != FR_OK) {
//         return FLASHCART_ERROR_LOAD;
//     }

//     size_t save_size = f_size(&fil);

//     if (f_read(&fil, address, save_size, &br) != FR_OK) {
//         f_close(&fil);
//         return FLASHCART_ERROR_LOAD;
//     }

//     if (f_close(&fil) != FR_OK) {
//         return FLASHCART_ERROR_LOAD;
//     }

//     if (br != save_size) {
//         return FLASHCART_ERROR_LOAD;
//     }

    return FLASHCART_OK;
}

static flashcart_error_t ed64_set_save_type (flashcart_save_type_t save_type) {
//     ed64_save_type_t type;

//     switch (save_type) {
//         case FLASHCART_SAVE_TYPE_NONE:
//             type = SAVE_TYPE_NONE;
//             break;
//         case FLASHCART_SAVE_TYPE_EEPROM_4K:
//             type = SAVE_TYPE_EEPROM_4K;
//             break;
//         case FLASHCART_SAVE_TYPE_EEPROM_16K:
//             type = SAVE_TYPE_EEPROM_16K;
//             break;
//         case FLASHCART_SAVE_TYPE_SRAM:
//             type = SAVE_TYPE_SRAM;
//             break;
//         case FLASHCART_SAVE_TYPE_SRAM_BANKED:
//             type = SAVE_TYPE_SRAM_BANKED;
//             break;
//         case FLASHCART_SAVE_TYPE_SRAM_128K:
//             type = SAVE_TYPE_SRAM_128K;
//             break;
//         case FLASHCART_SAVE_TYPE_FLASHRAM:
//             type = SAVE_TYPE_FLASHRAM;
//             break;
//         default:
//             return FLASHCART_ERROR_ARGS;
//     }

//     if (ed64_set_config(CFG_SAVE_TYPE, type) != ED64_OK) {
//         return FLASHCART_ERROR_INT;
//     }

    return FLASHCART_OK;
}

static flashcart_error_t ed64_set_save_writeback (uint32_t *sectors) {
//     ed64_write_data(sectors, ED64_BUFFERS->BUFFER, 1024);

//     if (ed64_writeback_enable(ED64_BUFFERS->BUFFER) != ED64_OK) {
//         return FLASHCART_ERROR_INT;
//     }

    return FLASHCART_OK;
}


static flashcart_t flashcart_ed64 = {
    .init = ed64_init,
    .deinit = ed64_deinit,
    .load_rom = ed64_load_rom,
    .load_save = ed64_load_save,
    .set_save_type = ed64_set_save_type,
    .set_save_writeback = ed64_set_save_writeback,
};


flashcart_t *ed64_get_flashcart (void) {
    return &flashcart_ed64;
}
