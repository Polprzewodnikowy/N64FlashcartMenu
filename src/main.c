#include <libdragon.h>
#include <fatfs/ff.h>
#include <stdio.h>
#include <string.h>

#include "boot/boot.h"
#include "menu/menu.h"


int main (void) {
    boot_params_t boot_params;
    // boot_params.device_type = BOOT_DEVICE_TYPE_ROM;
    // boot_params.tv_type = BOOT_TV_TYPE_PASSTHROUGH;
    // boot_params.cic_seed = 0;
    // boot_params.detect_cic_seed = true;
    // boot_params.cheat_list = NULL;
    // boot_params.clear_rdram = false;

    menu_run(&boot_params);

    // TEMPORARY DIAGNOSTIC: confirm menu_run() (and therefore flashcart_deinit()) returned
    // without hanging/crashing, so we know the failure is in boot() or the game itself.
    FIL dbg;
    if (f_open(&dbg, "/menu/debug_ed64.txt", FA_WRITE | FA_OPEN_APPEND) == FR_OK) {
        const char *line = "main: menu_run() returned, about to call boot()\n";
        UINT bw;
        f_write(&dbg, line, (UINT) (strlen(line)), &bw);
        f_close(&dbg);
    }

    // Disable interrupts before booting the game to prevent any interrupts from occurring during the boot process, 
    // which could cause instability or unexpected behavior, and helps act like a cold boot.
    disable_interrupts();

    boot(&boot_params);

    assertf(false, "Unexpected return from 'boot' function");
}
