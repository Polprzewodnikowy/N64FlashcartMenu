#include <stdio.h>
#include <string.h>

#include <libdragon.h>
#include <fatfs/ff.h>

#include "flashcart/flashcart.h"

#include "settings.h"
#include "menu_main.h"
#include "menu_info.h"
#include "menu_fileinfo.h"

static int scroll_menu_position = 0;
static int items_in_dir = 0;
static char current_filename[256];

// e.g. if (str_endswith(cur_rom, ".z64") || str_endswith(cur_rom, ".n64"))
static bool str_endswith(const char *str, const char *suffix) {
	char *p = strstr(str, suffix);
	return p && p[strlen(suffix)] == '\0';
}

// FIXME: use newlib rather than fatfs to do this!
FRESULT scan_file_path (char* path) {

    FRESULT res;
    DIR dir;
    char sfno[273];
    int counter = 0;
          
    res = f_opendir(&dir, path);
     
    if (res == FR_OK) {
        for (;;) {
            FILINFO fno;
     
            res = f_readdir(&dir, &fno);
     
            if ((res != FR_OK) || (fno.fname[0] == 0)) {
                break;
            }

            counter ++;
     
            sprintf(sfno, "%c%c%c%c %10d %s/%s",
                ((fno.fattrib & AM_DIR) ? 'D' : '-'),
                ((fno.fattrib & AM_RDO) ? 'R' : '-'),
                ((fno.fattrib & AM_SYS) ? 'S' : '-'),
                ((fno.fattrib & AM_HID) ? 'H' : '-'),
                (int)fno.fsize, path, fno.fname);
           
            if (scroll_menu_position == counter -1) {
                printf("> %s\n", sfno);
                sprintf(current_filename, "%s", fno.fname);
            }
            else {
                    printf("  %s\n", sfno);
            }
        }
    }
    items_in_dir = counter -1;
    f_closedir(&dir);
    return res;
}

void menu_main_refresh (char *dir_path) {
    console_clear();
    printf("N64 Flashcart Menu V0.0.0\n\n");
    printf("SD Card Directory list:\n\n");
    scan_file_path(dir_path);
}

void menu_main_init (settings_t *settings) {
    // TODO: implement nice user interface here

    console_init();
    console_set_debug(true);

    controller_init();

    controller_scan();
	struct controller_data joypad = get_keys_down();

    if ((settings->last_state.auto_load_last_rom) && !(joypad.c[0].B)) { // FIXME: the B button on any controller!
            printf("Loading last ROM: %s\n", settings->last_rom.rom_path);
            assertf(flashcart_load_rom(settings->last_rom.rom_path) == FLASHCART_OK, "ROM load error");

            printf("Loading save: %s, type: %d, writeback: %d\n", settings->last_rom.save_path, settings->last_rom.save_type, settings->last_rom.save_writeback);
            assertf(flashcart_load_save(settings->last_rom.save_path, settings->last_rom.save_type, settings->last_rom.save_writeback) == FLASHCART_OK, "Save load error");
    }
    else {
        settings->last_state.auto_load_last_rom = false;
        menu_main_refresh(settings->last_state.current_directory);
    }
    
    for (;;) {
        controller_scan();
		joypad = get_keys_down();

		if (joypad.c[0].up) {
            console_clear();
            if (scroll_menu_position > 0) {
                scroll_menu_position --;
            }
            else {
                scroll_menu_position = items_in_dir;
            }
            menu_main_refresh(settings->last_state.current_directory);
		}

        if (joypad.c[0].down) {
            console_clear();
            if ((scroll_menu_position >= 0) && (scroll_menu_position < items_in_dir)) {
                scroll_menu_position ++;
            }
            else {
                scroll_menu_position = 0;
            }
            menu_main_refresh(settings->last_state.current_directory);
		}

		if (joypad.c[0].A) {
            console_clear();
            printf("Loading ROM...\n");
            printf("%s\n", current_filename);
            if (str_endswith(current_filename, ".z64") || str_endswith(current_filename, ".n64") || str_endswith(current_filename, ".v64")) {
                assertf(flashcart_load_rom(current_filename) == FLASHCART_OK, "ROM load error");
                break; //required!
            }
            else {
                printf("Failed... Returning to menu...\n");
                wait_ms(1000);
                menu_main_refresh(settings->last_state.current_directory);
            }            
		}
        if (joypad.c[0].start) { // FIXME: the START button on any controller!
            console_clear();
            menu_info();
            menu_main_refresh(settings->last_state.current_directory);
		}
        if (joypad.c[0].B) {
            console_clear();
            menu_fileinfo();
            menu_main_refresh(settings->last_state.current_directory);
		}
    }
    // TODO: write menu state to SD card

}
