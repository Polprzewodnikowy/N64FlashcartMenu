#include <libdragon.h>

#include "boot/boot.h"
#include "hdmi/hdmi.h"
#include "menu/menu.h"


int main (void) {
    boot_params_t boot_params;

    hdmi_reset_game_id();

    menu_run(&boot_params);

    hdmi_broadcast_game_id(&boot_params);

    disable_interrupts();

    boot(&boot_params);

    assertf(false, "Unexpected return from 'boot' function");

    while (true) {
        // Shouldn't get here
    }
}
