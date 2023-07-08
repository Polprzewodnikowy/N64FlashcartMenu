#include <stdio.h>

#include <libdragon.h>
#include <usb.h>

#include "boot/boot.h"
#include "flashcart/flashcart.h"
#include "menu/menu.h"
#include "menu/settings.h"


static void hw_init (void) {
    assertf(usb_initialize() != CART_NONE, "No flashcart was detected");

    flashcart_error_t error = flashcart_init();
    assertf(error != FLASHCART_ERROR_OUTDATED, "Outdated flashcart firmware");
    assertf(error != FLASHCART_ERROR_UNSUPPORTED, "Unsupported flashcart");
    assertf(error == FLASHCART_OK, "Unknown error while initializing flashcart");

    assertf(debug_init_sdfs("sd:/", -1), "Couldn't initialize SD card");

#ifdef DEBUG
    debug_init_usblog();
#endif
}

static void hw_deinit (void) {
    flashcart_deinit();
    disable_interrupts();
}


int main (void) {
    settings_t settings;

    hw_init();

    settings_load_default_state(&settings);
    settings_load_from_file(&settings);

    menu_run(&settings);

    hw_deinit();

    boot(&settings.boot_params);

    return 1;
}
