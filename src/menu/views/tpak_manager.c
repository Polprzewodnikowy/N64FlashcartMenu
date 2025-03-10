#include <stdbool.h>
#include <stdio.h>
#include <libdragon.h>
#include "views.h"
#include "../sound.h"
#include "../fonts.h"
#include "../gb_game_info.h"
#include <fatfs/ff.h>

#define u8 unsigned char
#define u32 unsigned long

#define MAX_STRING_LENGTH_TPAK 50

short controller_selected_tpak;

bool has_transfert;
bool is_transfert_on;
bool status_ready;
bool status_removed;  //We focus on this flag to know if a Cartridge is in or not.
bool status_powered;  
bool status_resetting;
bool status_was_reset;
bool checksum_checked;
struct gameboy_cartridge_header header;   
bool buffer_read;
uint8_t *rom_data;

int error_tpak_header;
int result_tpak_header;
char tpak_header_text[50];

char tpak_cart_info[500];

const char * GB_PATH = "sd:/gb_saves";
const char * GB_PATH_NO_PRE = "/gb_saves";


u8 fmLoadDir(const TCHAR* path, FILINFO *inf, u32 max_items);

void reset_vars_tpak(){
    has_transfert = false;
    is_transfert_on = false;
    status_ready = false;
    status_removed = false;
    status_powered = false;
    status_resetting = false;
    status_was_reset = false;
    checksum_checked = false;
    buffer_read = false;
    error_tpak_header = 0;
    result_tpak_header = 0;
    header.title[0] = '\0';
    sprintf(tpak_header_text, " ");
    sprintf(tpak_cart_info, " ");
}

void create_directory_tpak(const char *dirpath) {
    FRESULT res = f_mkdir(dirpath);
    
    if (res == FR_OK) {
        printf("Directory created: %s\n", dirpath);
    } else if (res == FR_EXIST) {
        printf("Directory already exists: %s\n", dirpath);
    } else {
        printf("Failed to create directory: %s (Error Code: %d)\n", dirpath, res);
    }
}

bool check_tpak_detected(int port) {
    
    int val = joypad_get_accessory_type(port);

    has_transfert = val == JOYPAD_ACCESSORY_TYPE_TRANSFER_PAK;

    return has_transfert;
}

char * get_rom_size(gb_cart_rom_size_t value) {
    if (value == GB_ROM_32KB) return "32 KiB (no banks)";
    if (value == GB_ROM_64KB) return "64 KiB (4 banks)";
    if (value == GB_ROM_128KB) return "128 KiB (8 banks)";
    if (value == GB_ROM_256KB) return "256 KiB (16 banks)";
    if (value == GB_ROM_512KB) return "512 KiB (32 banks)";
    if (value == GB_ROM_1MB) return "1 MiB (64 banks)";
    if (value == GB_ROM_2MB) return "2 MiB (128 banks)";
    if (value == GB_ROM_4MB) return "4 MiB (256 banks)";
    if (value == GB_ROM_8MB) return "8 MiB (512 banks)";
    if (value == GB_ROM_1152KB) return "1.125 MiB (72 banks)";
    if (value == GB_ROM_1280KB) return "1.25 MiB (80 banks)";
    if (value == GB_ROM_1536KB) return "1.5 MiB (96 banks)";
    return "";
}

char * get_ram_size(gb_cart_ram_size_t value) {

    if (value == GB_RAM_NONE) return "not available";
    if (value == GB_RAM_2KB) return "2 KiB (no banks)";
    if (value == GB_RAM_8KB) return "8 KiB (no banks)";
    if (value == GB_RAM_32KB) return "32 KiB (4 banks)";
    if (value == GB_RAM_64KB) return "64 KiB (8 banks)";
    if (value == GB_RAM_128KB) return "128 KiB (16 banks)";
    return "";

}

const char *get_cart_type(gb_cart_type_t value) {
    if (value == GB_ROM_ONLY) {
        return "ROM only (32 KiB ROM)";
    } else if (value == GB_MBC1) {
        return "MBC1 (max 2 MiB ROM)";
    } else if (value == GB_MBC1_RAM) {
        return "MBC1 (max 2 MiB ROM)\n+ RAM (32 KiB)";
    } else if (value == GB_MBC1_RAM_BATTERY) {
        return "MBC1 (max 2 MiB ROM)\n+ RAM (32 KiB) + Battery";
    } else if (value == GB_MBC2) {
        return "MBC2 (max 256 KiB ROM;\n512x4 bits RAM built-in)";
    } else if (value == GB_MBC2_BATTERY) {
        return "MBC2 (max 256 KiB ROM;\n 512x4 bits RAM built-in) + Battery";
    } else if (value == GB_ROM_RAM) {
        return "ROM (32 KiB) + RAM\n(max 8 KiB)";
    } else if (value == GB_ROM_RAM_BATTERY) {
        return "ROM (32 KiB) + RAM\n(max 8 KiB) + Battery";
    } else if (value == GB_MMM01) {
        return "MMM01 (\"Meta-mapper\")";
    } else if (value == GB_MMM01_RAM) {
        return "MMM01 (\"Meta-mapper\")\n+ RAM";
    } else if (value == GB_MMM01_RAM_BATTERY) {
        return "MMM01 (\"Meta-mapper\")\n+ RAM + Battery";
    } else if (value == GB_MBC3) {
        return "MBC3 (max 2 MiB ROM)";
    } else if (value == GB_MBC3_RAM) {
        return "MBC3 (max 2 MiB ROM)\n+ RAM (64 KiB)";
    } else if (value == GB_MBC3_RAM_BATTERY) {
        return "MBC3 (max 2 MiB ROM)\n+ RAM (64 KiB) + Battery";
    } else if (value == GB_MBC3_TIMER_BATTERY) {
        return "MBC3 (max 2 MiB ROM)+\n RTC + Battery";
    } else if (value == GB_MBC3_TIMER_RAM_BATTERY) {
        return "MBC3 (max 2 MiB ROM) +\n RTC + RAM (64 KiB) + Battery";
    } else if (value == GB_MBC5) {
        return "MBC5 (max 8 MiB ROM)";
    } else if (value == GB_MBC5_RAM) {
        return "MBC5 (max 8 MiB ROM)\n+ RAM (128 KiB)";
    } else if (value == GB_MBC5_RAM_BATTERY) {
        return "MBC5 (max 8 MiB ROM)\n+ RAM (128 KiB) + Battery";
    } else if (value == GB_MBC5_RUMBLE) {
        return "MBC5 (max 8 MiB ROM)\n+ Rumble";
    } else if (value == GB_MBC5_RUMBLE_RAM) {
        return "MBC5 (max 8 MiB ROM)\n+ Rumble + RAM (128 KiB)";
    } else if (value == GB_MBC5_RUMBLE_RAM_BATTERY) {
        return "MBC5 (max 8 MiB ROM)\n+ Rumble + RAM (128 KiB) + Battery";
    } else if (value == GB_MBC6) {
        return "MBC6";
    } else if (value == GB_MBC7_SENSOR_RUMBLE_RAM_BATTERY) {
        return "MBC7 + Tilt Sensor\n+ Rumble + RAM + Battery";
    } else if (value == GB_POCKET_CAMERA) {
        return "Game Boy Camera";
    } else if (value == GB_BANDAI_TAMA5) {
        return "Bandai TAMA5";
    } else if (value == GB_HUC3) {
        return "Hudson HuC3";
    } else if (value == GB_HUC1_RAM_BATTERY) {
        return "Hudson HuC1 + RAM\n+ Battery";
    } else {
        return "";
    }
}


static void process (menu_t *menu){
    if(menu->actions.go_left) {
        sound_play_effect(SFX_SETTING);
        controller_selected_tpak = ((controller_selected_tpak - 1) + 4) % 4;
        reset_vars_tpak();
    } else if (menu->actions.go_right) {
        sound_play_effect(SFX_SETTING);
        controller_selected_tpak = ((controller_selected_tpak + 1) + 4) % 4;
        reset_vars_tpak();
    } else if (menu->actions.back) {
        sound_play_effect(SFX_EXIT);
        menu->next_mode = MENU_MODE_BROWSER;
    }

    check_tpak_detected(controller_selected_tpak);

    if (has_transfert) {
        if (is_transfert_on == 0) {
            int init = tpak_init(controller_selected_tpak);
            wait_ms(200);

            if (init == 0) {
                is_transfert_on = true;
                init = tpak_set_power(controller_selected_tpak, true);
                wait_ms(200);
            } else {
                is_transfert_on = false;
            }
        }

        uint8_t status = tpak_get_status(controller_selected_tpak);

        status_ready     = status & TPAK_STATUS_READY;
        status_removed   = status & TPAK_STATUS_REMOVED;
        status_powered   = status & TPAK_STATUS_POWERED;
        status_resetting = status & TPAK_STATUS_IS_RESETTING;
        status_was_reset = status & TPAK_STATUS_WAS_RESET;

        if (!status_removed) {
            is_transfert_on = true;
            //sprintf(tpak_header_text, " ");
            //sprintf(tpak_cart_info, " ");

            if (!checksum_checked) {
                error_tpak_header = tpak_get_cartridge_header(controller_selected_tpak, &header);

                if (error_tpak_header == 0) {
                    result_tpak_header = tpak_check_header(&header);

                    if (!result_tpak_header) {
                        sprintf(tpak_header_text, "Transfert Pak is empty");
                        tpak_set_power(controller_selected_tpak, false);
                        header.title[0] = '\0';
                    } else {
                        sprintf(tpak_header_text, "Transfert Pak has a cartridge");
                        checksum_checked = true;
                        buffer_read = false;
                    }

                } else {
                   sprintf(tpak_header_text, "ERROR on reading the header of the GB cart");
                    checksum_checked = false;
                    buffer_read = false;
                    tpak_set_power(controller_selected_tpak, false);
                    header.title[0] = '\0';
                }
            }

            if (header.title[0] != '\0') {
                char temp_DMG[MAX_STRING_LENGTH_TPAK];
                    
                int length_DMG = snprintf(temp_DMG, sizeof(temp_DMG), "%d-%d", header.header_checksum, header.global_checksum);
                char* code = (char*)malloc((length_DMG + 1) * sizeof(char));
                printf("Code retrieved : %s", code);
                
                sprintf(code, "%d-%d", header.header_checksum, header.global_checksum);
                GameInfo index_value = search_gb_info_by_game_code(code);
                free(code);
                code = NULL;

                if (index_value.rom_size != 0) {

                    //TODO : define if use or not gb assets

                    /*

                    if (strlen(index_value.image_path) > 2) {

                        //IN-ROM directly load  --> for dev purposes (easier)
                        //comment the IN-ROM part & uncomment the SD part to load the sprites from the SD card
                        //Also check "gb_dumper" file !!

                        utils_for_file_opening = dfs_open(index_value.image_path);
                        sprite_t *gb_image = malloc(dfs_size(utils_for_file_opening));
                        dfs_read(gb_image, 1, dfs_size(utils_for_file_opening), utils_for_file_opening);
                        dfs_close(utils_for_file_opening);

                        //Load from SD

                        //FILE *fp = fopen(index_value.image_path, "r");
                        //const int size = filesize(fp);
                        //sprite_t *gb_image = malloc(size);
                        //fread(gb_image, 1, size, fp);
                        //fclose(fp);

                        
                        graphics_draw_sprite(utils_disp, 250, 0, gb_image);
                        free(gb_image);

                    }

                    */

                    sprintf(tpak_cart_info, "%s", index_value.game_name);
                    sprintf(tpak_cart_info, "%s \n\nGame code  : %s", tpak_cart_info, index_value.game_code);
                    sprintf(tpak_cart_info, "%s \nNotes      : %s", tpak_cart_info, index_value.notes);
                    sprintf(tpak_cart_info, "%s \nLanguage   : %s", tpak_cart_info, index_value.language);
                    sprintf(tpak_cart_info, "%s \nRegion     : %s", tpak_cart_info,index_value.region);
                    //printf("Rev./Ver.  : %c \n", header.version_number);
                    sprintf(tpak_cart_info, "%s \nCart. type : %s", tpak_cart_info, get_cart_type(header.cartridge_type));
                    sprintf(tpak_cart_info, "%s \nSGB comp.  : %s", tpak_cart_info, header.is_sgb_supported & SGB_ENHANCED ? "Yes" : "No");
                    sprintf(tpak_cart_info, "%s \nROM size   : %s", tpak_cart_info, get_rom_size(header.rom_size_code));
                    sprintf(tpak_cart_info, "%s \nRAM size   : %s", tpak_cart_info, get_ram_size(header.ram_size_code));
                    sprintf(tpak_cart_info, "%s \nNew title  : %s", tpak_cart_info, header.new_title.title);
                } else {
                    sprintf(tpak_cart_info, "%s", header.title);
                    sprintf(tpak_cart_info, "%s \nGame code : %.4s", tpak_cart_info, &header.title[11]);
                    //printf("Rev./Ver. : %c \n", header.version_number);
                    sprintf(tpak_cart_info, "%s \nCart. type: %s", tpak_cart_info, get_cart_type(header.cartridge_type));
                    sprintf(tpak_cart_info, "%s \nSGB comp. : %s", tpak_cart_info, header.is_sgb_supported & SGB_ENHANCED ? "Yes" : "No");
                    sprintf(tpak_cart_info, "%s \nROM size  : %s", tpak_cart_info, get_rom_size(header.rom_size_code));
                    sprintf(tpak_cart_info, "%s \nRAM size  : %s", tpak_cart_info,get_ram_size(header.ram_size_code));
                    //printf("Destination code : %c \n", header.destination_code);
                    sprintf(tpak_cart_info, "%s \nNew title : %s", tpak_cart_info, header.new_title.title);
                    sprintf(tpak_cart_info, "%s \nOld title : %s", tpak_cart_info ,header.old_title.title);
                    sprintf(tpak_cart_info, "%s \nCHECKSUM header : %d", tpak_cart_info, header.header_checksum);
                    sprintf(tpak_cart_info, "%s \nCHECKSUM complete : %d", tpak_cart_info, header.global_checksum);
                    sprintf(tpak_cart_info, "%s \nCHECKSUM tot : %d", tpak_cart_info, header.header_checksum + header.global_checksum);
                }
            }

        }

    }
}

static void draw (menu_t *menu, surface_t *d){
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    ui_components_layout_draw();

    char has_mem_text_tpak[64];
    menu_font_type_t style;
    menu_font_type_t style_2;



    if (has_transfert) {
        sprintf(has_mem_text_tpak, "TPAK detected");
        style = STL_GREEN;

        if (is_transfert_on) {
            style_2 = STL_GREEN;
            sprintf(has_mem_text_tpak, "%s %s", has_mem_text_tpak, " (is init)");
        } else { 
            style_2 = STL_YELLOW;
            sprintf(has_mem_text_tpak, "%s %s", has_mem_text_tpak, " (ERROR on pow/init)");
        }




    } else {
        sprintf(has_mem_text_tpak, "NO TPAK detected");
        style = STL_ORANGE;
        reset_vars_tpak();
    }



    ui_components_main_text_draw(STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "TRANSFER PAK MANAGEMENT\n"
    );

    ui_components_main_text_draw(STL_DEFAULT,
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "Controller selected: < %d >\n",
        controller_selected_tpak + 1
    );

    ui_components_main_text_draw(style,
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "                            %s\n",
        has_mem_text_tpak
    );


    ui_components_main_text_draw(STL_DEFAULT,
        ALIGN_CENTER, VALIGN_TOP,
        "\n"
        "\n"
        "%s\n",
        tpak_header_text
    );

    ui_components_main_text_draw(STL_YELLOW,
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "\n"
        "\n"
        "\n"
        "\n"
        "%s\n",
        tpak_cart_info
    );


    ui_components_actions_bar_text_draw(STL_DEFAULT,
        ALIGN_LEFT, VALIGN_TOP,
        "A: Dump Savefile\n"
        "L: Dump ROM\n"
    );
    ui_components_actions_bar_text_draw(STL_DEFAULT,
        ALIGN_RIGHT, VALIGN_TOP,
        "START: Restore Savefile\n"
        "\n"
    );

    ui_components_actions_bar_text_draw(STL_GRAY,
        ALIGN_RIGHT, VALIGN_TOP,
        "\n"
        "R: Options\n"
    );

    rdpq_detach_show();
}




void view_transfer_pak_init (menu_t *menu){
    controller_selected_tpak = 0;
    reset_vars_tpak();

    create_directory_tpak(GB_PATH_NO_PRE);
}


void view_transfer_pak_display (menu_t *menu, surface_t *display){
    process(menu);
    draw(menu, display);
}