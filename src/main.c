#include <stdio.h>

#include <libdragon.h>
#include <usb.h>

#include "boot/boot.h"
#include "flashcart/flashcart.h"
#include "menu/settings.h"
#include "menu/menu_main.h"


static void init (void) {
    assertf(usb_initialize() != CART_NONE, "No flashcart was detected");
    //debug_init_usblog();
    assertf(debug_init_sdfs("sd:/", -1), "Couldn't initialize SD card");
    flashcart_error_t error = flashcart_init();
    assertf(error != FLASHCART_ERROR_OUTDATED, "Outdated flashcart firmware");
    assertf(error != FLASHCART_ERROR_UNSUPPORTED, "Unsupported flashcart");
    assertf(error == FLASHCART_OK, "Unknown error while initializing flashcart");
}

static void deinit (void) {
    flashcart_deinit();
    rdpq_close();
    rspq_close();
    audio_close();
    display_close();
    disable_interrupts();
}


int main (void) {

    init();

    settings_t settings;
    settings_load_default_state(&settings);
    //settings_load_from_file(&settings); // FIXME: this needs a rethink.

    // if (boot_is_warm()) {
    //     menu_restore(&settings);
    // }

    menu_main_init(&settings);

    deinit();

    boot(&settings.boot_params);

    return 1;
}
