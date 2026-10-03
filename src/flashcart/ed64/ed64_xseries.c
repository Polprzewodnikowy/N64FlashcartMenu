#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <fatfs/ff.h>
#include <libdragon.h>

#include "utils/fs.h"
#include "utils/utils.h"

#include "../flashcart_utils.h"
#include "ed64_bios_ll.h"
#include "ed64_xseries_ll.h"
#include "ed64_xseries.h"
#include "ed64_pseudo_state.h"

#define ROM_ADDRESS                 (0xB0000000)
#define ED64_XSERIES_MAX_SAVE_SIZE  (KiB(128))
#define ED64_XSERIES_STATE_FILE     "sd:/menu/ed64_xseries_state.ini"

static ed64_save_type_t current_save_type = ED64_SAVE_TYPE_NONE;
/** @brief Pending save-writeback state, persisted across the RESET button (see ed64_xseries_flush_pending_writeback). */
static ed64_pseudo_writeback_t pending_writeback;
/** @brief Whether the full (RTC/USB/save registers) personality is actually active this boot. */
static bool fpga_configured = false;

static flashcart_firmware_version_t ed64_xseries_get_firmware_version (void) {
    flashcart_firmware_version_t version_info = {
        .major = (uint16_t) (ed64_bios_get_cart_id()),
        .minor = 0,
        .revision = 0,
    };

    return version_info;
}

/**
 * @brief X series carts (like V series) can't monitor in-game save writes, so the menu only
 *        regains control after the RESET button is pressed. If the previous session left a
 *        save pending, read it back from the cart and write it to the SD card now.
 * 
 * @return flashcart_err_t Error code.
 */
static flashcart_err_t ed64_xseries_flush_pending_writeback (void) {
    if (!pending_writeback.is_expecting_save_writeback || !pending_writeback.last_save_path || !pending_writeback.last_save_path[0]) {
        return FLASHCART_OK;
    }

    ed64_save_type_t save_type = (ed64_save_type_t) ((int) (pending_writeback.save_type));
    int64_t save_size = file_get_size(pending_writeback.last_save_path);

    if ((save_size <= 0) || (save_size > ED64_XSERIES_MAX_SAVE_SIZE)) {
        pending_writeback.is_expecting_save_writeback = false;
        ed64_pseudo_state_save(&pending_writeback);
        return FLASHCART_OK;
    }

    uint8_t *buffer = ed64_bios_save_buffer;

    ed64_bios_read_save(save_type, buffer, (size_t) (save_size));

    // Matches the official OS's own bramBackup(): skip writing back a save the game never
    // actually touched this session, rather than needlessly rewriting an unchanged file.
    if (ed64_bios_save_is_blank(buffer, (size_t) (save_size))) {
        pending_writeback.is_expecting_save_writeback = false;
        ed64_pseudo_state_save(&pending_writeback);
        return FLASHCART_OK;
    }

    FIL fil;
    UINT bw;

    if (f_open(&fil, strip_fs_prefix(pending_writeback.last_save_path), FA_WRITE | FA_CREATE_ALWAYS) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }
    if ((f_write(&fil, buffer, (UINT) (save_size), &bw) != FR_OK) || (bw != (UINT) (save_size))) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }
    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    pending_writeback.is_expecting_save_writeback = false;
    ed64_pseudo_state_save(&pending_writeback);

    return FLASHCART_OK;
}

static flashcart_err_t ed64_xseries_init (void) {
    // X series carts (like V series) boot into a minimal personality; load the full one
    // (RTC/USB/save registers) every time, same as the official OS's edConfigureFpga()
    // does on a cold power-on. We always redo it rather than skipping it on a warm reset
    // the way the official OS does via its BOOTMOD/NCSTART boot-config bits, for simplicity.
    // A missing/corrupt firmware file shouldn't brick the whole menu: fall back to running
    // in the cart's basic cold-boot personality instead, same as the V1 handling below.
    fpga_configured = ed64_xseries_ll_configure_fpga("rom:/menu/firmware/ed64_xseries_fpga_ice.rle", "rom:/menu/firmware/ed64_xseries_fpga_main.rle");
    if (!fpga_configured) {
        debugf("ed64x: FPGA personality load failed, continuing in basic mode\n");
        return FLASHCART_OK;
    }

    directory_create("sd:/menu");
    ed64_pseudo_state_init(ED64_XSERIES_STATE_FILE);

    // The shared boot-config GAMEMOD bit survives RESET, so a true cold power-on can skip
    // the SD-card pending-writeback check entirely - only check it if we're actually
    // regaining control after a game ran, matching the official OS's own GAMEMOD check.
    if (!ed64_bios_take_game_mode_flag()) {
        return FLASHCART_OK;
    }

    ed64_pseudo_state_load(&pending_writeback);

    return ed64_xseries_flush_pending_writeback();
}

static flashcart_err_t ed64_xseries_deinit (void) {
    ed64_bios_set_game_mode();
    ed64_pseudo_state_free(&pending_writeback);
    return FLASHCART_OK;
}

static bool ed64_xseries_has_feature (flashcart_features_t feature) {
    bool is_model_x7 = fpga_configured && (ed64_bios_get_cart_id() == ED64_CART_ID_X7);
    switch (feature) {
        case FLASHCART_FEATURE_RTC: return is_model_x7;
        case FLASHCART_FEATURE_USB: return is_model_x7;
        case FLASHCART_FEATURE_64DD: return false;
        case FLASHCART_FEATURE_AUTO_CIC: return true;
        case FLASHCART_FEATURE_AUTO_REGION: return true;
        // No hardware support for monitoring save writes; the pending-writeback flow above
        // flushes saves back to the SD card once the menu regains control (after RESET).
        case FLASHCART_FEATURE_SAVE_WRITEBACK: return true;
        // The FPGA/ICE40 personality bitstreams are embedded assets in this same menu ROM,
        // not separately-flashed cart firmware; updating the menu updates them too.
        case FLASHCART_FEATURE_BIOS_UPDATE_FROM_MENU: return true;
        default: return false;
    }
}

static flashcart_err_t ed64_xseries_load_rom (char *rom_path, flashcart_progress_callback_t *progress) {
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_fs_prefix(rom_path), FA_READ) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    fatfs_fix_file_size(&fil);

    size_t rom_size = f_size(&fil);

    if (rom_size > MiB(64)) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    size_t sdram_size = rom_size; // (MiB(64) - KiB(128));

    size_t chunk_size = KiB(128);
    for (unsigned int offset = 0; offset < sdram_size; offset += chunk_size) {
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

    return FLASHCART_OK;
}

static flashcart_err_t ed64_xseries_load_file (char *file_path, uint32_t rom_offset, uint32_t file_offset) {
    FIL fil;
    UINT br;

    if (f_open(&fil, strip_fs_prefix(file_path), FA_READ) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    fatfs_fix_file_size(&fil);

    size_t file_size = f_size(&fil) - file_offset;

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

static flashcart_err_t ed64_xseries_load_save (char *save_path) {
    if (current_save_type == ED64_SAVE_TYPE_NONE) {
        return FLASHCART_OK;
    }

    FIL fil;
    UINT br;
    uint8_t *buffer = ed64_bios_save_buffer;
    size_t buffer_size = ED64_BIOS_MAX_SAVE_SIZE;

    if (f_open(&fil, strip_fs_prefix(save_path), FA_READ) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    size_t save_size = f_size(&fil);

    if (save_size > buffer_size) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if ((f_read(&fil, buffer, save_size, &br) != FR_OK) || (br != save_size)) {
        f_close(&fil);
        return FLASHCART_ERR_LOAD;
    }

    if (f_close(&fil) != FR_OK) {
        return FLASHCART_ERR_LOAD;
    }

    ed64_bios_write_save(current_save_type, buffer, save_size);
    // ed64_bios_write_save() either never touches the save-type register (EEPROM) or
    // leaves it reset to NONE (SRAM/FlashRAM, see ed64_bios_ll.c) - apply the game's
    // actual save type now so it's correctly configured before it boots.
    ed64_bios_set_save_type(current_save_type);

    if (pending_writeback.last_save_path) {
        free(pending_writeback.last_save_path);
    }
    pending_writeback.last_save_path = strdup(save_path);
    pending_writeback.is_expecting_save_writeback = true;
    // NOTE: reusing this generic field to persist the low-level save type across resets.
    pending_writeback.save_type = (flashcart_save_type_t) ((int) (current_save_type));
    ed64_pseudo_state_save(&pending_writeback);

    return FLASHCART_OK;
}

static flashcart_err_t ed64_xseries_set_save_type (flashcart_save_type_t save_type) {
    ed64_save_type_t type;

    switch (save_type) {
        case FLASHCART_SAVE_TYPE_NONE:
            type = ED64_SAVE_TYPE_NONE;
            break;
        case FLASHCART_SAVE_TYPE_EEPROM_4KBIT:
            type = ED64_SAVE_TYPE_EEPROM_4K;
            break;
        case FLASHCART_SAVE_TYPE_EEPROM_16KBIT:
            type = ED64_SAVE_TYPE_EEPROM_16K;
            break;
        case FLASHCART_SAVE_TYPE_SRAM_256KBIT:
            type = ED64_SAVE_TYPE_SRAM_32K;
            break;
        case FLASHCART_SAVE_TYPE_SRAM_BANKED:
            type = ED64_SAVE_TYPE_SRAM_96K_BANKED;
            break;
        case FLASHCART_SAVE_TYPE_SRAM_1MBIT:
            type = ED64_SAVE_TYPE_SRAM_128K;
            break;
        case FLASHCART_SAVE_TYPE_FLASHRAM_1MBIT:
        case FLASHCART_SAVE_TYPE_FLASHRAM_PKST2:
            type = ED64_SAVE_TYPE_FLASHRAM;
            break;
        default:
            return FLASHCART_ERR_ARGS;
    }

    ed64_bios_set_save_type(type);
    current_save_type = type;

    return FLASHCART_OK;
}

static flashcart_t flashcart_ed64_xseries = {
    .init = ed64_xseries_init,
    .deinit = ed64_xseries_deinit,
    .has_feature = ed64_xseries_has_feature,
    .get_firmware_version = ed64_xseries_get_firmware_version,
    .load_rom = ed64_xseries_load_rom,
    .load_file = ed64_xseries_load_file,
    .load_save = ed64_xseries_load_save,
    .load_64dd_ipl = NULL,
    .load_64dd_disk = NULL,
    .load_64dd_disks = NULL,
    .get_button_state = NULL,
    .get_voltage_temperature = NULL,
    .set_save_type = ed64_xseries_set_save_type,
    .set_save_writeback = NULL,
    .set_next_boot_mode = NULL,
};


flashcart_t *ed64_xseries_get_flashcart (void) {
    return &flashcart_ed64_xseries;
}
