#include <stddef.h>

#include <libcart/cart.h>
#include <libdragon.h>
#include <usb.h>

#include "utils/fs.h"
#include "utils/utils.h"

#include "flashcart.h"
#include "flashcart_utils.h"

#include "64drive/64drive.h"
#include "sc64/sc64.h"


static const size_t SAVE_SIZE[__FLASHCART_SAVE_TYPE_END] = {
    0,
    512,
    KiB(2),
    KiB(32),
    KiB(96),
    KiB(128),
    KiB(128),
    KiB(128),
};


static flashcart_err_t dummy_init (void) {
    return FLASHCART_OK;
}

static bool dummy_has_feature (flashcart_features_t feature) {
    switch (feature) {
        default:
            return false;
    }
}

static flashcart_err_t dummy_load_rom (char *rom_path, flashcart_progress_callback_t *progress) {
    return FLASHCART_OK;
}

static flashcart_err_t dummy_load_file (char *file_path, uint32_t rom_offset, uint32_t file_offset) {
    return FLASHCART_OK;
}

static flashcart_err_t dummy_load_save (char *save_path) {
    return FLASHCART_OK;
}

static flashcart_err_t dummy_set_save_type (flashcart_save_type_t save_type) {
    return FLASHCART_OK;
}

static flashcart_t *flashcart = &((flashcart_t) {
    .init = dummy_init,
    .deinit = NULL,
    .has_feature = dummy_has_feature,
    .load_rom = dummy_load_rom,
    .load_file = dummy_load_file,
    .load_save = dummy_load_save,
    .load_64dd_ipl = NULL,
    .load_64dd_disk = NULL,
    .set_save_type = dummy_set_save_type,
    .set_save_writeback = NULL,
});

#ifdef NDEBUG
    // HACK: libdragon mocks every debug function if NDEBUG flag is enabled.
    //       Code below reverts that and point to real function instead.
    #undef debug_init_sdfs
    bool debug_init_sdfs (const char *prefix, int npart);
#endif


char *flashcart_convert_error_message (flashcart_err_t err) {
    switch (err) {
        case FLASHCART_OK: return "No error";
        case FLASHCART_ERR_OUTDATED: return "Outdated flashcart firmware";
        case FLASHCART_ERR_SD_CARD: return "Error during SD card initialization";
        case FLASHCART_ERR_BBFS: return "Error during iQue NAND initialization";
        case FLASHCART_ERR_ARGS: return "Invalid argument passed to flashcart function";
        case FLASHCART_ERR_LOAD: return "Error during loading data into flashcart";
        case FLASHCART_ERR_INT: return "Internal flashcart error";
        case FLASHCART_ERR_FUNCTION_NOT_SUPPORTED: return "Flashcart doesn't support this function";
        default: return "Unknown flashcart error";
    }
}

flashcart_err_t flashcart_init (const char **storage_prefix) {
    flashcart_err_t err;

    if (sys_bbplayer()) {
        // TODO: Add iQue callbacks
        *storage_prefix = "bbfs:/";
        if (bbfs_init()) {
            return FLASHCART_ERR_BBFS;
        }
        return FLASHCART_OK;
    }

    *storage_prefix = "sd:/";
    bool sd_card_initialized = debug_init_sdfs(*storage_prefix, -1);

    switch (cart_type) {
        case CART_CI:   // 64drive
            flashcart = d64_get_flashcart();
            break;

        case CART_EDX:  // Series X EverDrive-64
            break;

        case CART_ED:   // Original EverDrive-64
            break;

        case CART_SC:   // SummerCart64
            flashcart = sc64_get_flashcart();
            break;

        default:        // Probably emulator
            *storage_prefix = "rom:/";
            debug_init_isviewer();
            break;
    }

#ifndef NDEBUG
    // NOTE: Some flashcarts doesn't have USB port, can't throw error here
    debug_init_usblog();
#endif

    if ((err = flashcart->init()) != FLASHCART_OK) {
        return err;
    }

    if ((cart_type != CART_NULL) && (!sd_card_initialized)) {
        return FLASHCART_ERR_SD_CARD;
    }

    return FLASHCART_OK;
}

flashcart_err_t flashcart_deinit (void) {
    if (flashcart->deinit) {
        return flashcart->deinit();
    }

    return FLASHCART_OK;
}

bool flashcart_has_feature (flashcart_features_t feature) {
    return flashcart->has_feature(feature);
}

flashcart_err_t flashcart_load_rom (char *rom_path, bool byte_swap, flashcart_progress_callback_t *progress) {
    flashcart_err_t err;

    if (rom_path == NULL) {
        return FLASHCART_ERR_ARGS;
    }

    cart_card_byteswap = byte_swap;
    err = flashcart->load_rom(rom_path, progress);
    cart_card_byteswap = false;

    return err;
}

flashcart_err_t flashcart_load_file (char *file_path, uint32_t rom_offset, uint32_t file_offset) {
    if ((file_path == NULL) || ((file_offset % FS_SECTOR_SIZE) != 0)) {
        return FLASHCART_ERR_ARGS;
    }

    return flashcart->load_file(file_path, rom_offset, file_offset);
}

flashcart_err_t flashcart_load_save (char *save_path, flashcart_save_type_t save_type) {
    flashcart_err_t err;

    if (save_type >= __FLASHCART_SAVE_TYPE_END) {
        return FLASHCART_ERR_ARGS;
    }

    if ((err = flashcart->set_save_type(save_type)) != FLASHCART_OK) {
        return err;
    }

    if ((save_path == NULL) || (save_type == FLASHCART_SAVE_TYPE_NONE)) {
        return FLASHCART_OK;
    }

    if (!file_exists(save_path)) {
        if (file_allocate(save_path, SAVE_SIZE[save_type])) {
            return FLASHCART_ERR_LOAD;
        }
        if (file_fill(save_path, 0xFF)) {
            return FLASHCART_ERR_LOAD;
        }
    }

    if (file_get_size(save_path) != SAVE_SIZE[save_type]) {
        return FLASHCART_ERR_LOAD;
    }

    if ((err = flashcart->load_save(save_path)) != FLASHCART_OK) {
        return err;
    }

    if (!flashcart->set_save_writeback) {
        return FLASHCART_OK;
    }

    return flashcart->set_save_writeback(save_path);
}

flashcart_err_t flashcart_load_64dd_ipl (char *ipl_path, flashcart_progress_callback_t *progress) {
    if (!flashcart->load_64dd_ipl) {
        return FLASHCART_ERR_FUNCTION_NOT_SUPPORTED;
    }

    if (ipl_path == NULL) {
        return FLASHCART_ERR_ARGS;
    }

    return flashcart->load_64dd_ipl(ipl_path, progress);
}

flashcart_err_t flashcart_load_64dd_disk (char *disk_path, flashcart_disk_parameters_t *disk_parameters) {
    if (!flashcart->load_64dd_disk) {
        return FLASHCART_ERR_FUNCTION_NOT_SUPPORTED;
    }

    if ((disk_path == NULL) || (disk_parameters == NULL)) {
        return FLASHCART_ERR_ARGS;
    }

    return flashcart->load_64dd_disk(disk_path, disk_parameters);
}
