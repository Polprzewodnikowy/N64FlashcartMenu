#include <libdragon.h>
#include "menu_res_setup.h"
#include "menu_fileinfo.h"
#include "../utils/str_utils.h"


static char *get_file_type(FILINFO current_fileinfo) {
    // TODO: should be at least a switch statement!
    if (str_endswith(current_fileinfo.fname, ".z64") || str_endswith(current_fileinfo.fname, ".n64") || str_endswith(current_fileinfo.fname, ".v64") || str_endswith(current_fileinfo.fname, ".rom")) {
        // TODO: check the necessary bytes in the header to ensure!
        return "N64 ROM";
    }
    else if (str_endswith(current_fileinfo.fname, ".txt")) {
        return "Text File";
    }
    else if (str_endswith(current_fileinfo.fname, ".ini")) {
        return "INI File";
    }
    else if (str_endswith(current_fileinfo.fname, ".yml") || str_endswith(current_fileinfo.fname, ".yaml")) {
        return "YAML File";
    }
    else if (str_endswith(current_fileinfo.fname, ".toml")) {
        return "TOML File";
    }
    else if (str_endswith(current_fileinfo.fname, ".sav")) {
        return "Save File";
    }
    else if (str_endswith(current_fileinfo.fname, ".emu")) {
        return "Emulator File";
    }
    else {
        return "Unknown File";
    }
}

void menu_fileinfo_draw_header(display_context_t disp) {
	
	graphics_draw_text(disp, (disp->width / 2) - 64, vertical_start_position, "FILE INFORMATION"); // centre = numchars * font_horizontal_pixels / 2
	graphics_draw_line(disp,0,30,disp->width,30, 0xff);
	
}

void menu_fileinfo_draw_footer(display_context_t disp) {
	
	graphics_draw_line(disp,0,disp->height - overscan_vertical_pixels - font_vertical_pixels,disp->width,disp->height - overscan_vertical_pixels - font_vertical_pixels, 0xff);
	graphics_draw_text(disp, (disp->width / 2) - 80,disp->height - overscan_vertical_pixels, "Press (B) to return!"); // centre = numchars * font_horizontal_pixels / 2
	
}

void menu_fileinfo(FILINFO current_fileinfo) {

    char str_buffer[1024];

	display_context_t disp = display_try_get();
	graphics_fill_screen(disp, 0x00);
	menu_fileinfo_draw_header(disp);

    int16_t vertical_position = 40;

    graphics_draw_text(disp, horizontal_start_position, vertical_position, "Name:");
    graphics_draw_text(disp, horizontal_indent, vertical_position += font_vertical_pixels, current_fileinfo.fname);
    vertical_position += (font_vertical_pixels * 2);

    graphics_draw_text(disp, horizontal_start_position, vertical_position, "Size:");
    sprintf(str_buffer, "%d %s", (int)current_fileinfo.fsize, "Bytes");
    graphics_draw_text(disp, horizontal_indent, vertical_position += font_vertical_pixels, str_buffer);
    vertical_position += (font_vertical_pixels * 2);

    graphics_draw_text(disp, horizontal_start_position, vertical_position, "Attributes:");
    sprintf(str_buffer, "%s%s%s%s%s\n",
                ((current_fileinfo.fattrib & AM_DIR) ? "Directory" : "File"),
                ((current_fileinfo.fattrib & AM_RDO) ? " | Readonly" : ""),
                ((current_fileinfo.fattrib & AM_SYS) ? " | System" : ""),
                ((current_fileinfo.fattrib & AM_ARC) ? " | Archive" : ""),
                ((current_fileinfo.fattrib & AM_HID) ? " | Hidden" : ""));
           
    graphics_draw_text(disp, horizontal_indent, vertical_position += font_vertical_pixels, str_buffer);
    vertical_position += (font_vertical_pixels * 2);

    graphics_draw_text(disp, horizontal_start_position, vertical_position, "Modified Timestamp:");
    sprintf(str_buffer, "%u-%02u-%02u, %02u:%02u", (current_fileinfo.fdate >> 9) + 1980, current_fileinfo.fdate >> 5 & 15, current_fileinfo.fdate & 31, current_fileinfo.ftime >> 11, current_fileinfo.ftime >> 5 & 63);
    graphics_draw_text(disp, horizontal_indent, vertical_position += font_vertical_pixels, str_buffer);
    vertical_position += (font_vertical_pixels * 2);

    graphics_draw_text(disp, horizontal_start_position, vertical_position, "Type:");
    graphics_draw_text(disp, horizontal_indent, vertical_position += font_vertical_pixels, get_file_type(current_fileinfo));

	menu_fileinfo_draw_footer(disp);

	display_show(disp);

	for (;;) {
		controller_scan();
		struct controller_data ckeys = get_keys_down();

		if (ckeys.c[0].B) {
			break;
		}
	}
}