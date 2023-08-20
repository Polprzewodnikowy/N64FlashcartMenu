#include <stddef.h>

#include <libcart/cart.h>
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


flashcart_error_t flashcart_init (void) {
    flashcart_error_t error;

    // NOTE: Explicitly support only these flashcarts in this specific initialization order.
    struct {
        int type;
        int (* libcart_init) (void);
        flashcart_t *(* get) (void);
    } flashcarts[CART_MAX] = {
        { CART_CI, ci_init, NULL },                 // 64drive
        { CART_SC, sc_init, sc64_get_flashcart },   // SC64
        { CART_EDX, edx_init, NULL },               // Series X EverDrive-64
        { CART_ED, ed_init, NULL },                 // Original EverDrive-64
    };

    for (int i = 0; i < CART_MAX; i++) {
        if (flashcarts[i].libcart_init() >= 0) {
            cart_type = flashcarts[i].type;
            if (flashcarts[i].get) {
                flashcart = flashcarts[i].get();
            }
            break;
        }
    }

    if (cart_type == CART_NULL) {
        return FLASHCART_ERROR_NOT_DETECTED;    
    }

#ifndef NDEBUG
    // NOTE: Some flashcarts doesn't have USB port, can't throw error here
    debug_init_usblog();
#endif

    if ((error = flashcart->init()) != FLASHCART_OK) {
        return error;
    }

    if (!debug_init_sdfs("sd:/", -1)) {
        return FLASHCART_ERROR_SD_CARD;
    }

    return FLASHCART_OK;
}

flashcart_error_t flashcart_deinit (void) {
    if (flashcart->deinit) {
        return flashcart->deinit();
    }
    return FLASHCART_OK;
}

flashcart_error_t flashcart_load_rom (char *rom_path, bool byte_swap, flashcart_progress_callback_t *progress) {
    flashcart_error_t error;

    if ((rom_path == NULL) || (!file_exists(rom_path)) || (file_get_size(rom_path) < KiB(4))) {
        return FLASHCART_ERROR_ARGS;
    }

    cart_card_byteswap = byte_swap;
    error = flashcart->load_rom(rom_path, progress);
    cart_card_byteswap = false;

    return error;
}

flashcart_error_t flashcart_load_file (char *file_path, uint32_t start_offset_address) {
    if ((file_path == NULL) || (!file_exists(file_path))) {
        return FLASHCART_ERROR_ARGS;
    }

    return flashcart->load_file(file_path, start_offset_address);
}

flashcart_error_t flashcart_load_save (char *save_path, flashcart_save_type_t save_type) {
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
        if (file_fill(save_path, 0xFF)) {
            return FLASHCART_ERROR_LOAD;
        }
    }

    if (file_get_size(save_path) != SAVE_SIZE[save_type]) {
        return FLASHCART_ERROR_LOAD;
    }

    if ((error = flashcart->load_save(save_path)) != FLASHCART_OK) {
        return error;
    }

    if (flashcart->set_save_writeback) {
        if (file_get_sectors(save_path, sectors, WRITEBACK_MAX_SECTORS)) {
            return FLASHCART_ERROR_LOAD;
        }
        if ((error = flashcart->set_save_writeback(sectors)) != FLASHCART_OK) {
            return error;
        }
    }

    return FLASHCART_OK;
}
