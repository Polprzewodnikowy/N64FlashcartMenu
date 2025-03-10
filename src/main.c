#include <libdragon.h>

#include "boot/boot.h"
#include "menu/menu.h"


int main (void) {
    boot_params_t boot_params;

    menu_run(&boot_params);

    disable_interrupts();

    debug_init_isviewer();

    boot(&boot_params);

    assertf(false, "Unexpected return from 'boot' function");

    while (true) {
        // Shouldn't get here
    }
}
