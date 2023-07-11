#include <libdragon.h>

#include "boot/boot.h"
#include "menu/menu.h"
#include "menu/settings.h"


int main (void) {
    settings_t settings;

    settings_load_default_state(&settings);

    menu_run(&settings);

    disable_interrupts();

    boot(&settings.boot_params);

    assertf(false, "Unexpected return from 'boot' function");

    while (true) {
        // Shouldn't get here
    }
}
