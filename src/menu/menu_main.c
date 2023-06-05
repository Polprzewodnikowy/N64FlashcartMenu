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
#include "menu_res_setup.h"

#include "../utils/str_utils.h"


static int scroll_menu_position = 1;
static int items_in_dir = 1;

static FILINFO current_fileinfo;

// FIXME: use newlib rather than fatfs to do this!
FRESULT scan_file_path (char* path) {

    FRESULT res;
    DIR dir;
    char sfno[280];
    int counter = 0; //FIXME: we dont account for an empty dir or only one valid file!

    res = f_opendir(&dir, path);
     
    if (res == FR_OK) {
        for (;;) {
            FILINFO file_info;
     
            res = f_readdir(&dir, &file_info);
     
            if ((res != FR_OK) || (file_info.fname[0] == 0)) {
                break;
            }

            if (file_info.fattrib & AM_SYS) {
                continue; // we do not want to show system files ever...
            }

            counter ++;
            
            // TODO: convert these to icons...
            sprintf(sfno, "| %c%c%c | %10d | %s",
                ((file_info.fattrib & AM_DIR) ? 'D' : '-'),
                ((file_info.fattrib & AM_RDO) ? 'R' : '-'),
                ((file_info.fattrib & AM_HID) ? 'H' : '-'),
                (int)file_info.fsize, file_info.fname);
           
            if (scroll_menu_position == counter) {
                printf("-> %s\n", sfno);
                current_fileinfo = file_info;
            }
            else {
                printf("   %s\n", sfno);
            }
        }
    }
    items_in_dir = counter;
    f_closedir(&dir);
    return res;
}

void menu_main_draw_header() {

        printf("SC64 Flashcart Menu Rev: 0.0.2\n\n");
}

void menu_main_draw_footer(char *dir_path) {
	
    printf("\n\nDirectory: %s\n\n", dir_path);
    printf("    File: %d of %d\n\n", scroll_menu_position, items_in_dir);
	
}

void menu_main_refresh (char *dir_path) {

    console_clear();

    menu_main_draw_header();

    printf("   | DRH | FILE SIZE  | FILE NAME\n");
    printf("   |-----|------------|----------\n");
    
    scan_file_path(dir_path);

    menu_main_draw_footer(dir_path);

}

void menu_main_init (settings_t *settings) {
    // TODO: implement nice user interface here

    console_init();
    console_set_debug(true);
    console_clear();

    controller_init();

    char *current_dir = settings->last_state.current_directory;
    char *last_dir = current_dir;
	struct controller_data joypad = get_keys_down();

    if ((settings->last_state.auto_load_last_rom) && !(joypad.c[0].B)) { // FIXME: the B button on any controller!
            printf("Loading last ROM: %s\n", settings->last_rom.rom_path);
            assertf(flashcart_load_rom(settings->last_rom.rom_path) == FLASHCART_OK, "ROM load error");

            printf("Loading save: %s, type: %d, writeback: %d\n", settings->last_rom.save_path, settings->last_rom.save_type, settings->last_rom.save_writeback);
            assertf(flashcart_load_save(settings->last_rom.save_path, settings->last_rom.save_type, settings->last_rom.save_writeback) == FLASHCART_OK, "Save load error");
    }
    else {
        settings->last_state.auto_load_last_rom = false;
        menu_main_refresh(current_dir);
    }
    
    for (;;) {
        controller_scan();
		joypad = get_keys_down();

		if (joypad.c[0].up) {
            if (scroll_menu_position > 1 && scroll_menu_position <= items_in_dir) {
                scroll_menu_position --;
            }
            else {
                scroll_menu_position = items_in_dir;
            }
            menu_main_refresh(current_dir);
		}

        if (joypad.c[0].down) {
            if (scroll_menu_position < items_in_dir) {
                scroll_menu_position ++;
            }
            else {
                scroll_menu_position = 1;
            }
            menu_main_refresh(current_dir);
		}

		if (joypad.c[0].A) {
            // TODO: move this to a function and check that the ROM is valid by checking the header...
            if (str_endswith(current_fileinfo.fname, ".z64") || str_endswith(current_fileinfo.fname, ".n64") || str_endswith(current_fileinfo.fname, ".v64") || str_endswith(current_fileinfo.fname, ".rom")) {
                printf("Loading N64 ROM type...\n");
                printf("%s\n", current_fileinfo.fname);
                assertf(flashcart_load_rom(current_fileinfo.fname) == FLASHCART_OK, "ROM load error");
                // FIXME: we now need the header ID and CRC HI...
                // ed64_dma_read_rom(buff, 0, 1);
                // crc_high = (buff[0x10] << 24) | (buff[0x11] << 16) | (buff[0x12] << 8) | (buff[0x13] << 0);
                // crc_low =  (buff[0x14] << 24) | (buff[0x15] << 16) | (buff[0x16] << 8) | (buff[0x17] << 0);
                // id = (buff[0x3c] << 8) | buff[0x3d];
                //assertf(flashcart_load_save("current_filename.sav", rom_db_match_save_type(id, crc), false) == FLASHCART_OK, "ROM load save error");
                break; //required!
            }
            else {
                if (current_fileinfo.fattrib & AM_DIR) {
                    // if this is a directory we need to transverse to it!
                    items_in_dir = 1;
                    scroll_menu_position = 1;
                    last_dir = current_dir;
                    current_dir = current_fileinfo.fname;
                    menu_main_refresh(current_dir);
                }
                else {
                    printf("Failed... Returning to menu...\n");
                    wait_ms(1000);
                    menu_main_refresh(current_dir);
                }
            }            
		}
        if (joypad.c[0].B) {
                menu_main_refresh(last_dir);
                current_dir = last_dir;
        }
        if (joypad.c[0].start) { // FIXME: the START button on any controller!
            menu_info();
            menu_main_refresh(current_dir);
		}
        if (joypad.c[0].Z) {
            menu_fileinfo(current_fileinfo);
            menu_main_refresh(current_dir);
		}
    }
    // TODO: write menu state to SD card

}
