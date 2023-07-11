#include <stddef.h>

#include <libdragon.h>
#include <usb.h>

#include "utils/fs.h"
#include "utils/utils.h"

#include "flashcart.h"
#include "sc64/sc64.h"


#define WRITEBACK_MAX_SECTORS   (256)


static const size_t SAVE_SIZE[__FLASHCART_SAVE_TYPE_END] = {
    0,
    512,
    KiB(2),
    KiB(32),
    KiB(96),
    KiB(128),
    KiB(128),
};

static flashcart_error_t dummy_init (void) {
    return FLASHCART_OK;
}

static flashcart_t *flashcart = &((flashcart_t) {
    .init = dummy_init,
    .deinit = NULL,
    .load_rom = NULL,
    .load_save = NULL,
    .set_save_type = NULL,
    .set_save_writeback = NULL,
});


flashcart_error_t flashcart_init (void) {
    if (usb_initialize() == CART_NONE) {
        return FLASHCART_ERROR_NOT_DETECTED;
    }

#ifndef NDEBUG
    assertf(debug_init_usblog(), "Couldn't initialize USB debugging");
#endif

    if (!debug_init_sdfs("sd:/", -1)) {
        return FLASHCART_ERROR_SD_CARD_ERROR;
    }

    switch (usb_getcart()) {
        case CART_64DRIVE:
        case CART_EVERDRIVE:
            break;

        case CART_SC64:
            flashcart = sc64_get_flashcart();
            break;

        default:
            return FLASHCART_ERROR_UNSUPPORTED;
    }

    return flashcart->init();
}

flashcart_error_t flashcart_deinit (void) {
    if (flashcart->deinit) {
        return flashcart->deinit();
    }
    return FLASHCART_OK;
}

flashcart_error_t flashcart_load_rom (char *rom_path, bool byte_swap) {
    if ((rom_path == NULL) || (!file_exists(rom_path)) || (file_get_size(rom_path) < KiB(4))) {
        return FLASHCART_ERROR_ARGS;
    }
    return flashcart->load_rom(rom_path, byte_swap);
}

flashcart_error_t flashcart_load_save (char *save_path, flashcart_save_type_t save_type, bool save_writeback) {
    flashcart_error_t error;
    uint32_t sectors[WRITEBACK_MAX_SECTORS] __attribute__((aligned(8)));

    if (save_type >= __FLASHCART_SAVE_TYPE_END) {
        return FLASHCART_ERROR_ARGS;
    }

    if ((error = flashcart->set_save_type(save_type)) != FLASHCART_OK) {
        return error;
    }

    if ((save_path == NULL) || (save_type == FLASHCART_SAVE_TYPE_NONE)) {
        return FLASHCART_OK;
    }

    if (!file_exists(save_path)) {
        if (file_allocate(save_path, SAVE_SIZE[save_type])) {
            return FLASHCART_ERROR_LOAD;
        }
    }

    if (file_get_size(save_path) != SAVE_SIZE[save_type]) {
        return FLASHCART_ERROR_LOAD;
    }

    if ((error = flashcart->load_save(save_path)) != FLASHCART_OK) {
        return error;
    }

    if (save_writeback) {
        if (file_get_sectors(save_path, sectors, WRITEBACK_MAX_SECTORS)) {
            return FLASHCART_ERROR_LOAD;
        }
        if ((error = flashcart->set_save_writeback(sectors)) != FLASHCART_OK) {
            return error;
        }
    }

    return FLASHCART_OK;
}
