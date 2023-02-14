#include <stdio.h>

#include <libdragon.h>
#include <usb.h>

#include "boot/boot.h"
#include "flashcart/flashcart.h"
#include "menu/menu.h"


static void init (void) {
    assertf(usb_initialize() != CART_NONE, "No flashcart was detected");
    switch (flashcart_init()) {
        case FLASHCART_OK:
            break;
        case FLASHCART_ERROR_OUTDATED:
            assertf(false, "Outdated flashcart firmware");
            break;
        case FLASHCART_ERROR_UNSUPPORTED:
            assertf(false, "Unsupported flashcart");
            break;
        default:
            assertf(false, "Unknown error while initializing flashcart");
            break;
    }
    assertf(debug_init_sdfs("sd:/", -1), "Couldn't initialize SD card");
}

static void deinit (void) {
    flashcart_deinit();
    rdp_close();
    rspq_close();
    audio_close();
    display_close();
    disable_interrupts();
}


int main (void) {
    menu_t menu;

    init();

    if (boot_is_warm()) {
        menu_restore(&menu);
    }

    menu_run(&menu);

    deinit();

    boot(&menu.boot_params);

    return 1;
}
