#include <fatfs/ff.h>
#include <libdragon.h>
#include "../menu.h"
#include "../menu_res_setup.h"
#include "../../utils/str_utils.h"
#include "../rom_database.h"


static FILINFO info;

static char *get_rom_endian_s (uint32_t endian) {
    switch (endian)
    {
    case ROM_BIG_ENDIAN:
        return "Big (default)"; // expected
        break;
    case ROM_LITTLE_ENDIAN:
        return "Little";
        break;
    case ROM_MID_LITTLE_ENDIAN:
        return "Middle Little";
        break;
    case ROM_MID_BIG_ENDIAN:
        return "Middle Big";
        break;
    default:
        return "Unknown";
        break;
    }
}

static char *get_rom_mediatype_s (uint8_t type) {
    switch (type)
    {
    case N64_CART:
        return "N - Cartridge";
        break;
    case N64_DISK:
        return "D - Disk";
        break;
    case N64_CART_EXPANDABLE:
        return "C - Cartridge Exp";
        break;
    case N64_DISK_EXPANDABLE:
        return "E - Disk Exp";
        break;
    case N64_ALECK64:
        return "Z - Aleck64";
        break;
    default:
        return "Unknown";
        break;
    }
}

static char *get_rom_savetype_s (uint8_t type) {
    switch (type)
    {
    case DB_SAVE_TYPE_EEPROM_4K:
        return "EEPROM 4K";
        break;
    case DB_SAVE_TYPE_EEPROM_16K:
        return "EEPROM 16K";
        break;
    case DB_SAVE_TYPE_SRAM:
        return "SRAM";
        break;
    case DB_SAVE_TYPE_SRAM_BANKED:
        return "SRAM Banked";
        break;
    case DB_SAVE_TYPE_SRAM_128K:
        return "SRAM 128K [ED64]";
        break;
    case DB_SAVE_TYPE_FLASHRAM:
        return "FLASH RAM";
        break;
    case DB_SAVE_TYPE_CPAK:
        return "Controller PAK";
        break;
    default:
        return "Unknown";
        break;
    }
}

static char *get_file_type_s (void) {
    // TODO: should be at least a switch statement!
    if (str_endswith(info.fname, ".z64") || str_endswith(info.fname, ".n64") || str_endswith(info.fname, ".v64") || str_endswith(info.fname, ".rom")) {
        // TODO: check the necessary bytes in the header to ensure!
        return "N64 ROM";
    }
    else if (str_endswith(info.fname, ".txt")) {
        return "Text File";
    }
    else if (str_endswith(info.fname, ".ini")) {
        return "INI File";
    }
    else if (str_endswith(info.fname, ".yml") || str_endswith(info.fname, ".yaml")) {
        return "YAML File";
    }
    else if (str_endswith(info.fname, ".toml")) {
        return "TOML File";
    }
    else if (str_endswith(info.fname, ".sav") || str_endswith(info.fname, ".eep") || str_endswith(info.fname, ".sra") || str_endswith(info.fname, ".srm")|| str_endswith(info.fname, ".fla")) {
        return "ROM Save File";
    }
    else if (str_endswith(info.fname, ".mpk")) {
        return "ControllerPak Backup File";
    }
    else if (str_endswith(info.fname, ".emu")) {
        return "Emulator File";
    }
    else {
        return "Unknown File";
    }
}

static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    char str_buffer[1024];

	graphics_fill_screen(d, 0x00);

    graphics_draw_text(d, (d->width / 2) - 64, vertical_start_position, "FILE INFORMATION"); // centre = numchars * font_horizontal_pixels / 2
	graphics_draw_line(d, 0, 30, d->width, 30, 0xff);

    int16_t vertical_position = 40;

    graphics_draw_text(d, horizontal_start_position, vertical_position, "Name:");
    graphics_draw_text(d, horizontal_indent, vertical_position += font_vertical_pixels, info.fname);
    vertical_position += (font_vertical_pixels * 2);

    graphics_draw_text(d, horizontal_start_position, vertical_position, "Size:");
    sprintf(str_buffer, "%d %s", (int)info.fsize, "Bytes");
    graphics_draw_text(d, horizontal_indent, vertical_position += font_vertical_pixels, str_buffer);
    vertical_position += (font_vertical_pixels * 2);

    graphics_draw_text(d, horizontal_start_position, vertical_position, "Attributes:");
    sprintf(str_buffer, "%s%s%s%s%s\n",
                ((info.fattrib & AM_DIR) ? "Directory" : "File"),
                ((info.fattrib & AM_RDO) ? " | ReadOnly" : ""),
                ((info.fattrib & AM_SYS) ? " | System" : ""),
                ((info.fattrib & AM_ARC) ? " | Archive" : ""),
                ((info.fattrib & AM_HID) ? " | Hidden" : "")
                );
           
    graphics_draw_text(d, horizontal_indent, vertical_position += font_vertical_pixels, str_buffer);
    vertical_position += (font_vertical_pixels * 2);

    graphics_draw_text(d, horizontal_start_position, vertical_position, "Modified Timestamp:");
    sprintf(str_buffer, "%u-%02u-%02u, %02u:%02u", (info.fdate >> 9) + 1980, info.fdate >> 5 & 15, info.fdate & 31, info.ftime >> 11, info.ftime >> 5 & 63);
    graphics_draw_text(d, horizontal_indent, vertical_position += font_vertical_pixels, str_buffer);
    vertical_position += (font_vertical_pixels * 2);

    graphics_draw_text(d, horizontal_start_position, vertical_position, "Type:");
    graphics_draw_text(d, horizontal_indent, vertical_position += font_vertical_pixels, get_file_type_s());

    // TODO: split into a seperate menu item.
    if (strcmp(get_file_type_s(), "N64 ROM") == 0) {
        graphics_draw_line(d, d->width / 2, 67, d->width / 2, d->height - 45, 0xff);
        int x_start_position = (d->width / 2) + horizontal_start_position;
        int y_position = 67;
        graphics_draw_text(d, x_start_position, y_position, "N64 ROM Information:\n\n");
        y_position += (font_vertical_pixels * 2);

        // FIXME: issues with non root path as mix of stdio and fatfs!
        rom_header_t temp_header = file_read_rom_header(info.fname);

        sprintf(str_buffer,"File Endian: %s\n", get_rom_endian_s(temp_header.endian));
        graphics_draw_text(d, x_start_position, y_position += font_vertical_pixels, str_buffer);
        y_position += (font_vertical_pixels * 2);
        sprintf(str_buffer,"Title: %s\n", temp_header.title);
        graphics_draw_text(d, x_start_position, y_position += font_vertical_pixels, str_buffer);
        sprintf(str_buffer,"Media Type: %s\n", get_rom_mediatype_s(temp_header.metadata.media_type));
        graphics_draw_text(d, x_start_position, y_position += font_vertical_pixels, str_buffer);
        sprintf(str_buffer,"Unique ID: %.2s\n", (char*)&(temp_header.metadata.unique_identifier));
        graphics_draw_text(d, x_start_position, y_position += font_vertical_pixels, str_buffer);
        sprintf(str_buffer,"Destination Market: %c\n", temp_header.metadata.destination_market);
        graphics_draw_text(d, x_start_position, y_position += font_vertical_pixels, str_buffer);
        sprintf(str_buffer,"Version: %hhu\n", temp_header.version);
        graphics_draw_text(d, x_start_position, y_position += font_vertical_pixels, str_buffer);
        sprintf(str_buffer,"Checksum: %llu\n", temp_header.checksum);
        graphics_draw_text(d, x_start_position, y_position += font_vertical_pixels, str_buffer);
        y_position += (font_vertical_pixels * 2);
        uint8_t save_type = rom_db_match_save_type(temp_header);
        sprintf(str_buffer,"Save Type: %s\n", get_rom_savetype_s(save_type));
        graphics_draw_text(d, x_start_position, y_position += font_vertical_pixels, str_buffer);

        //menu_fileinfo_draw_n64_rom_info(d);
    }

	graphics_draw_line(d, 0, d->height - overscan_vertical_pixels - font_vertical_pixels, d->width,d->height - overscan_vertical_pixels - font_vertical_pixels, 0xff);
	graphics_draw_text(d, (d->width / 2) - 80,d->height - overscan_vertical_pixels, "Press (B) to return!"); // centre = numchars * font_horizontal_pixels / 2

    display_show(d);
}


void view_file_info_init (menu_t *menu) {
    path_t *file = path_clone(menu->browser.directory);
    path_push(file, menu->browser.list[menu->browser.selected].name);
    if (f_stat(path_get(file), &info) != FR_OK) {
        menu->next_mode = MENU_MODE_ERROR;
    }
    path_free(file);
}

void view_file_info_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
