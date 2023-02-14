#include <stddef.h>

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

static flashcart_t *flashcart;


bool flashcart_init (void) {
    switch (usb_getcart()) {
        case CART_SC64:
            flashcart = sc64_get_flashcart();
            break;

        default:
            return false;
    }

    return flashcart->init();
}

void flashcart_deinit (void) {
    flashcart->deinit();
}

flashcart_error_t flashcart_load_rom (char *rom_path) {
    if ((rom_path == NULL) || (!file_exists(rom_path)) || (file_get_size(rom_path) < KiB(4))) {
        return FLASHCART_ERROR_ARGS;
    }
    return flashcart->load_rom(rom_path);
}

flashcart_error_t flashcart_load_save (char *save_path, flashcart_save_type_t save_type, bool save_writeback) {
    flashcart_error_t error;
    uint32_t sectors[WRITEBACK_MAX_SECTORS] __attribute__((aligned(8)));

    if (save_type >= __FLASHCART_SAVE_TYPE_END) {
        return FLASHCART_ERROR_ARGS;
    }

    flashcart->set_save_type(save_type);

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
