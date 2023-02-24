#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <libdragon.h>
#include <fatfs/ff.h>

#include "flashcart/flashcart.h"

#include "settings.h"
#include "menu.h"

void menu_restore (settings_t *settings) {
    // TODO: restore last menu state from SD card
}

FRESULT scan_files (
    char* path        /* Start node to be scanned (***also used as work area***) */
)
{
    FRESULT res;
    DIR dir;
    UINT i;
    static FILINFO fno;


    res = f_opendir(&dir, path);                       /* Open the directory */
    if (res == FR_OK) {
        for (;;) {
            res = f_readdir(&dir, &fno);                   /* Read a directory item */
            if (res != FR_OK || fno.fname[0] == 0) break;  /* Break on error or end of dir */
            if (fno.fattrib & AM_DIR) {                    /* It is a directory */
                i = strlen(path);
                sprintf(&path[i], "/%s", fno.fname);
                res = scan_files(path);                    /* Enter the directory */
                if (res != FR_OK) break;
                path[i] = 0;
            } else {                                       /* It is a file. */
                printf("%s/%s\n", path, fno.fname);
            }
        }
        f_closedir(&dir);
    }

    return res;
}

void menu_run (settings_t *settings) {
    // TODO: implement nice user interface here

    console_init();
    console_set_debug(true);

    if (settings->last_state.auto_load_last_rom) { // TODO: check if there is a button input to cancel.

        printf("Loading last ROM: %s\n", settings->last_rom.rom_path);
        assertf(flashcart_load_rom(settings->last_rom.rom_path) == FLASHCART_OK, "ROM load error");

        printf("Loading save: %s, type: %d, writeback: %d\n", settings->last_rom.save_path, settings->last_rom.save_type, settings->last_rom.save_writeback);
        assertf(flashcart_load_save(settings->last_rom.save_path, settings->last_rom.save_type, settings->last_rom.save_writeback) == FLASHCART_OK, "Save load error");
    }
    else {
        printf("N64 Flashcart Menu\n\n");
        printf("File list:\n");
        char buff[256];
        strcpy(buff, "/");
        scan_files(buff); // TODO: use current_directory
    
        // TODO: wait for a key input
        for (;;) {
            wait_ms(1000);
        }
    }
    // TODO: write menu state to SD card

}
