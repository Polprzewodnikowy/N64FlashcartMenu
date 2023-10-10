#include <stddef.h>

#include <libcart/cart.h>
#include <libdragon.h>
#include <usb.h>

#include "utils/fs.h"
#include "utils/utils.h"

#include "flashcart.h"

#include "64drive/64drive.h"
#include "sc64/sc64.h"
#include "ed64/ed64.h"


#define SAVE_WRITEBACK_MAX_SECTORS  (256)


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

static uint32_t save_writeback_sectors[SAVE_WRITEBACK_MAX_SECTORS] __attribute__((aligned(8)));


static void save_writeback_sectors_callback (uint32_t sector_count, uint32_t file_sector, uint32_t cluster_sector, uint32_t cluster_size) {
    for (uint32_t i = 0; i < cluster_size; i++) {
        uint32_t offset = file_sector + i;
        uint32_t sector = cluster_sector + i;

        if ((offset > SAVE_WRITEBACK_MAX_SECTORS) || (offset > sector_count)) {
            return;
        }

        save_writeback_sectors[offset] = sector;
    }
}


static flashcart_err_t dummy_init (void) {
    return FLASHCART_OK;
}

static flashcart_t *flashcart = &((flashcart_t) {
    .init = dummy_init,
    .deinit = NULL,
    .load_rom = NULL,
    .load_file = NULL,
    .load_save = NULL,
    .set_save_type = NULL,
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
        case FLASHCART_ERR_NOT_DETECTED: return "No flashcart hardware was detected";
        case FLASHCART_ERR_OUTDATED: return "Outdated flashcart firmware";
        case FLASHCART_ERR_SD_CARD: return "Error during SD card initialization";
        case FLASHCART_ERR_ARGS: return "Invalid argument passed to flashcart function";
        case FLASHCART_ERR_LOAD: return "Error during loading data into flashcart";
        case FLASHCART_ERR_INT: return "Internal flashcart error";
        case FLASHCART_ERR_FUNCTION_NOT_SUPPORTED: return "Flashcart doesn't support this function";
        default: return "Unknown flashcart error";
    }
}

flashcart_err_t flashcart_init (void) {
    flashcart_err_t err;

    bool sd_card_initialized = debug_init_sdfs("sd:/", -1);

#ifndef NDEBUG
    // NOTE: Some flashcarts doesn't have USB port, can't throw error here
    debug_init_usblog();
#endif

    switch (cart_type) {
        case CART_CI:   // 64drive
            flashcart = d64_get_flashcart();
            break;

        case CART_EDX:  // Series X EverDrive-64
        case CART_ED:   // Original EverDrive-64
            flashcart = ed64_get_flashcart();
            break;

        case CART_SC:   // SummerCart64
            flashcart = sc64_get_flashcart();
            break;

        default:
            return FLASHCART_ERR_NOT_DETECTED;
    }

    if ((err = flashcart->init()) != FLASHCART_OK) {
        return err;
    }

    if (!sd_card_initialized) {
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

    if (flashcart->set_save_writeback) {
        for (int i = 0; i < SAVE_WRITEBACK_MAX_SECTORS; i++) {
            save_writeback_sectors[i] = 0;
        }
        if (file_get_sectors(save_path, save_writeback_sectors_callback)) {
            return FLASHCART_ERR_LOAD;
        }
        if ((err = flashcart->set_save_writeback(save_writeback_sectors)) != FLASHCART_OK) {
            return err;
        }
    }

    return FLASHCART_OK;
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
