#include <stdio.h>
#include <string.h>
// #include <sys/stat.h>
// #include <unistd.h>

#include <libdragon.h>
#include <fatfs/ff.h>

#include "flashcart/flashcart.h"

#include "settings.h"
#include "menu_main.h"

// FIXME: use newlib rather than fatfs to do this!
FRESULT scan_file_path (char* path) {

    FRESULT res;
    DIR dir;
    char sfno[273];
          
    res = f_opendir(&dir, path);
     
    if (res == FR_OK) {
        for (;;) {
            FILINFO fno;
     
            res = f_readdir(&dir, &fno);
     
            if ((res != FR_OK) || (fno.fname[0] == 0)) {
                break;
            }
     
            sprintf(sfno, "%c%c%c%c %10d %s/%s",
                ((fno.fattrib & AM_DIR) ? 'D' : '-'),
                ((fno.fattrib & AM_RDO) ? 'R' : '-'),
                ((fno.fattrib & AM_SYS) ? 'S' : '-'),
                ((fno.fattrib & AM_HID) ? 'H' : '-'),
                (int)fno.fsize, path, fno.fname);
     
            printf("%s\n", sfno);
        }
    }
    f_closedir(&dir);
    return res;
}

void menu_main_refresh (char *dir_path) {
    console_clear();
    printf("N64 Flashcart Menu\n\n");
    printf("SD Card Directory list:\n\n");
    scan_file_path(dir_path);
}

void menu_main_init (settings_t *settings) {
    // TODO: implement nice user interface here

    console_init();
    console_set_debug(true);

    controller_init();


    // TODO: wait for a key input

    if (settings->last_state.auto_load_last_rom) { // TODO: check if there is a button input to cancel.

        printf("Loading last ROM: %s\n", settings->last_rom.rom_path);
        assertf(flashcart_load_rom(settings->last_rom.rom_path) == FLASHCART_OK, "ROM load error");

        printf("Loading save: %s, type: %d, writeback: %d\n", settings->last_rom.save_path, settings->last_rom.save_type, settings->last_rom.save_writeback);
        assertf(flashcart_load_save(settings->last_rom.save_path, settings->last_rom.save_type, settings->last_rom.save_writeback) == FLASHCART_OK, "Save load error");
    }
    else {
        menu_main_refresh(settings->last_state.current_directory);
    }
    
    for (;;) {
        controller_scan();
		struct controller_data ckeys = get_keys_down();

		if (ckeys.c[0].A) {
            console_clear();
            printf("TODO: loading ROM...\n");
            wait_ms(1000);
            printf("Failed... Returning to menu...\n");
            wait_ms(1000);
            menu_main_refresh(settings->last_state.current_directory);
		}
    }
    // TODO: write menu state to SD card

}
