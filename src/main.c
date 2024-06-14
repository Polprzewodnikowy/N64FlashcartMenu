#include <libdragon.h>

#include "boot/boot.h"
#include "menu/menu.h"

// #ifdef DEBUG_GDB
// #include "libs/unfloader/debug.h"
// #endif


int main (void) {
    boot_params_t boot_params;

// #ifdef DEBUG_GDB
//     debug_initialize();
// #endif

    menu_run(&boot_params);

    disable_interrupts();

    boot(&boot_params);

    assertf(false, "Unexpected return from 'boot' function");

    while (true) {
        // Shouldn't get here
    }
}
