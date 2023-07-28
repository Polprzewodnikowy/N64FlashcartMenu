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
    .load_emulator_rom = NULL,
    .load_save = NULL,
    .set_save_type = NULL,
    .set_save_writeback = NULL,
});


flashcart_error_t flashcart_init (void) {
    flashcart_error_t error;

    // HACK: Because libcart reads PI config from address 0x10000000 when initializing
    //       we need to write safe value before running any libcart function.
    //       Data in SDRAM can be undefined on some flashcarts at this point
    //       and might result in setting incorrect PI config.
    extern uint32_t cart_dom1;
    cart_dom1 = 0x80371240;

    if (!debug_init_sdfs("sd:/", -1)) {
        return FLASHCART_ERROR_NOT_DETECTED;
    }

    // NOTE: Flashcart model is extracted from libcart after debug_init_sdfs call is made
    extern int cart_type;

    switch (cart_type) {
        case CART_CI: // 64drive
        case CART_ED: // Original EverDrive-64
        case CART_EDX: // Series X EverDrive-64
            break;

        case CART_SC: // SC64
            flashcart = sc64_get_flashcart();
            break;

        default:
            return FLASHCART_ERROR_UNSUPPORTED;
    }

    if ((error = flashcart->init()) != FLASHCART_OK) {
        return error;
    }

#ifndef MENU_NO_USB_LOG
    // NOTE: Some flashcarts doesn't have USB port, can't throw error here
    debug_init_usblog();
#endif

    return FLASHCART_OK;
}

flashcart_error_t flashcart_deinit (void) {
    if (flashcart->deinit) {
        return flashcart->deinit();
    }
    return FLASHCART_OK;
}

flashcart_error_t flashcart_load_rom (char *rom_path, bool byte_swap) {
    flashcart_error_t error;

    if ((rom_path == NULL) || (!file_exists(rom_path)) || (file_get_size(rom_path) < KiB(4))) {
        return FLASHCART_ERROR_ARGS;
    }

    if (cart_card_byteswap(byte_swap)) {
        return FLASHCART_ERROR_INT;
    }

    error = flashcart->load_rom(rom_path);

    if (cart_card_byteswap(false)) {
        return FLASHCART_ERROR_INT;
    }

    return error;
}

flashcart_error_t flashcart_load_emulator_rom (char *rom_path) {
    if ((rom_path == NULL) || (!file_exists(rom_path))) {
        return FLASHCART_ERROR_ARGS;
    }
    return flashcart->load_emulator_rom(rom_path);
}

flashcart_error_t flashcart_load_emulator_rom (char *rom_path) {
    if ((rom_path == NULL) || (!file_exists(rom_path))) {
        return FLASHCART_ERROR_ARGS;
    }
    return flashcart->load_emulator_rom(rom_path);
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
