#include <fatfs/ff.h>
#include <libdragon.h>

#include "views.h"

#include "../menu_res_setup.h"
#include "../../utils/str_utils.h"

#include "fragments/fragments.h"


static FILINFO info;


static char *get_file_type (void) {
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
    else if (str_endswith(info.fname, ".sav")) {
        return "Save File";
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
    // const color_t bg_color = RGBA32(0x00, 0x00, 0x00, 0xFF);

    // rdpq_attach(d, NULL);
    // rdpq_clear(bg_color);

    // fragment_borders(d);


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
    graphics_draw_text(d, horizontal_indent, vertical_position += font_vertical_pixels, get_file_type());

    //menu_fileinfo_draw_n64_rom_info(d);

	graphics_draw_line(d, 0, d->height - overscan_vertical_pixels - font_vertical_pixels, d->width,d->height - overscan_vertical_pixels - font_vertical_pixels, 0xff);
	graphics_draw_text(d, (d->width / 2) - 80,d->height - overscan_vertical_pixels, "Press (B) to return!"); // centre = numchars * font_horizontal_pixels / 2

    display_show(d);

    // rdpq_detach_show();
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
