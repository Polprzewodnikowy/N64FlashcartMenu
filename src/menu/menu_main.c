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


void load_n64_rom() {

    console_clear();

    char tmp_buffer[280];
    sprintf(tmp_buffer, "sd:/%s", current_fileinfo.fname);

    rom_header_t temp_header = file_read_rom_header(tmp_buffer);

    printf("ROM title: %s\n\n", temp_header.title);
    
    printf("ROM media type: %c\n", temp_header.metadata.media_type);
    printf("ROM unique id: %.2s\n", (char*)&(temp_header.metadata.unique_identifier));
    printf("uid as int: %d\n\n", temp_header.metadata.unique_identifier);

    printf("ROM destination market: %c\n\n", temp_header.metadata.destination_market);
    
    printf("ROM version: %hhu\n", temp_header.version);
    printf("ROM checksum: %llu\n\n", temp_header.checksum);

    
    uint8_t save_type = rom_db_match_save_type(temp_header);

    printf("save type: %d\n", save_type);
    sprintf(tmp_buffer, "%s.%llu.sav", current_fileinfo.fname, temp_header.checksum);
    wait_ms(5000); // wait used for debugging. Can be removed later.

    assertf(flashcart_load_save(tmp_buffer, (flashcart_save_type_t)save_type, true) == FLASHCART_OK, "ROM load save error");

    assertf(flashcart_load_rom(current_fileinfo.fname) == FLASHCART_OK, "ROM load error");

}

// FIXME: use newlib rather than fatfs to do this!
FRESULT scan_file_path (char *path) {

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

void menu_main_draw_header(surface_t *disp) {

    printf("           FILE MENU\n\n\n\n");

    // graphics_draw_text(disp, (disp->width / 2) - 36, vertical_start_position, "FILE MENU"); // centre = numchars * font_horizontal_pixels / 2
    // graphics_draw_line(disp,0,30,disp->width,30, 0xff);
}

void menu_main_draw_footer(char *dir_path, surface_t *disp) {
	
    // graphics_draw_line(disp,0,disp->height - overscan_vertical_pixels - font_vertical_pixels,disp->width,disp->height - overscan_vertical_pixels - font_vertical_pixels, 0xff);
    
    printf("\n\nCurrent Directory: %s\n", dir_path);
    printf("    File: %d of %d\n\n", scroll_menu_position, items_in_dir);
	
}

void menu_main_refresh (char *dir_path, surface_t *disp) {

    console_clear();

    menu_main_draw_header(disp);

    printf("   | DRH | FILE SIZE  | FILE NAME\n");
    printf("   |-----|------------|----------\n");
    
    scan_file_path(dir_path);

    menu_main_draw_footer(dir_path, disp);

}

void menu_main_init (settings_t *settings) {
    // TODO: implement nice user interface here
    surface_t *disp = display_try_get();



    console_init();
    console_set_debug(true);
    console_clear();

    

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
        menu_main_refresh(current_dir, disp);
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
            menu_main_refresh(current_dir, disp);
		}

        if (joypad.c[0].down) {
            if (scroll_menu_position < items_in_dir) {
                scroll_menu_position ++;
            }
            else {
                scroll_menu_position = 1;
            }
            menu_main_refresh(current_dir, disp);
		}

		if (joypad.c[0].A) {
            // TODO: move this to a function and check that the ROM is valid by checking the header...
            if (str_endswith(current_fileinfo.fname, ".z64") || str_endswith(current_fileinfo.fname, ".n64") || str_endswith(current_fileinfo.fname, ".v64") || str_endswith(current_fileinfo.fname, ".rom")) {
                printf("Loading N64 ROM type...\n");
                printf("%s\n", current_fileinfo.fname);

                load_n64_rom();

                break; //required!
            }
            else {
                if (current_fileinfo.fattrib & AM_DIR) {
                    // if this is a directory we need to transverse to it!
                    items_in_dir = 1;
                    scroll_menu_position = 1;
                    last_dir = current_dir;
                    current_dir = current_fileinfo.fname;
                    menu_main_refresh(current_dir, disp);
                }
                else {
                    printf("Failed... Returning to menu...\n");
                    wait_ms(1000);
                    menu_main_refresh(current_dir, disp);
                }
            }            
		}
        if (joypad.c[0].B) {
                menu_main_refresh(last_dir, disp);
                current_dir = last_dir;
        }
        if (joypad.c[0].start) { // FIXME: the START button on any controller!
            menu_info();
            menu_main_refresh(current_dir, disp);
		}
        if (joypad.c[0].Z) {
            menu_fileinfo(current_fileinfo);
            menu_main_refresh(current_dir, disp);
		}
        if (joypad.c[0].L) {
            //load the new File list GUI
            //menu_file_scroll(current_fileinfo);
        }
    }
    // TODO: write menu state to SD card

}
