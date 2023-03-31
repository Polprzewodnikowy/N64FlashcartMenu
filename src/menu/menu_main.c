#include <stdio.h>
#include <stdlib.h> // TODO: does this work... will unlock qsort!
#include <string.h>

#include <libdragon.h>
#include <fatfs/ff.h>

#include "flashcart/flashcart.h"

#include "settings.h"
#include "menu_main.h"
#include "menu_info.h"
#include "menu_fileinfo.h"
#include "rom_database.h"

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
    char sfno[280];
    int counter = 0;
          
    res = f_opendir(&dir, path);
     
    if (res == FR_OK) {
        for (;;) {
            FILINFO fno;
     
            res = f_readdir(&dir, &fno);
     
            if ((res != FR_OK) || (fno.fname[0] == 0)) {
                break;
            }

            if (fno.fattrib & AM_SYS) {
                continue; // we do not want to show system files ever...
            }

            counter ++;
            
            // TODO: convert these to icons...
            sprintf(sfno, "| %c%c%c | %10d | %s",
                ((fno.fattrib & AM_DIR) ? 'D' : '-'),
                ((fno.fattrib & AM_RDO) ? 'R' : '-'),
                ((fno.fattrib & AM_HID) ? 'H' : '-'),
                (int)fno.fsize, fno.fname);
           
            if (scroll_menu_position == counter -1) {
                printf("-> %s\n", sfno);
                sprintf(current_filename, fno.fname);
            }
            else {
                printf("   %s\n", sfno);
            }
        }
    }
    items_in_dir = counter -1;
    f_closedir(&dir);
    return res;
}

void menu_main_refresh (char *dir_path) {
    console_clear();
    printf("SC64 Flashcart Menu Rev: 0.0.1\n\n");
    printf("SD Card Directory list:\n\n");
    printf("   | DRH | FILE SIZE  | FILE NAME\n");
    printf("   |-----|------------|----------\n");
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
            //console_clear();
            if (scroll_menu_position > 0) {
                scroll_menu_position --;
            }
            else {
                scroll_menu_position = items_in_dir;
            }
            menu_main_refresh(settings->last_state.current_directory);
		}

        if (joypad.c[0].down) {
            //console_clear();
            if ((scroll_menu_position >= 0) && (scroll_menu_position < items_in_dir)) {
                scroll_menu_position ++;
            }
            else {
                scroll_menu_position = 0;
            }
            menu_main_refresh(settings->last_state.current_directory);
		}

		if (joypad.c[0].A) {
            //console_clear();
            printf("Loading ROM...\n");
            printf("%s\n", current_filename);
            // TODO: move this to a function and check that the ROM is valid by checking the header...
            if (str_endswith(current_filename, ".z64") || str_endswith(current_filename, ".n64") || str_endswith(current_filename, ".v64") || str_endswith(current_filename, ".rom")) {
                assertf(flashcart_load_rom(current_filename) == FLASHCART_OK, "ROM load error");
                // FIXME: we now need the header ID and CRC HI...
                // ed64_dma_read_rom(buff, 0, 1);
                // crc_high = (buff[0x10] << 24) | (buff[0x11] << 16) | (buff[0x12] << 8) | (buff[0x13] << 0);
                // crc_low =  (buff[0x14] << 24) | (buff[0x15] << 16) | (buff[0x16] << 8) | (buff[0x17] << 0);
                // id = (buff[0x3c] << 8) | buff[0x3d];
                //assertf(flashcart_load_save("current_filename.sav", rom_db_match_save_type(id, crc), false) == FLASHCART_OK, "ROM load save error");
                break; //required!
            }
            else {
                // TODO: if this is a directory we need to transverse to it!
                printf("Failed... Returning to menu...\n");
                wait_ms(1000);
                menu_main_refresh(settings->last_state.current_directory);
            }            
		}
        if (joypad.c[0].start) { // FIXME: the START button on any controller!
            //console_clear();
            menu_info();
            menu_main_refresh(settings->last_state.current_directory);
		}
        if (joypad.c[0].Z) {
            //console_clear();
            menu_fileinfo();
            menu_main_refresh(settings->last_state.current_directory);
		}
    }
    // TODO: write menu state to SD card

}
