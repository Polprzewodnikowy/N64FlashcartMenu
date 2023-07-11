#include <time.h>

#include <libdragon.h>

#include "../menu_res_setup.h"
#include "views.h"


char *accessory_type_s( int accessory )
{
    switch( accessory )
    {
        case ACCESSORY_RUMBLEPAK:
            return "[RumblePak]";
        case ACCESSORY_MEMPAK:
            return "[ControllerPak]";
        case ACCESSORY_VRU:
            return "[VRU]";
        case ACCESSORY_TRANSFERPAK:
            return "[TransferPak]"; 
        default:
            return "Unknown";
    }
}

static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    char str_buffer[512];

	graphics_fill_screen(d, 0x00);

	graphics_draw_text(d, (d->width / 2) - 64, vertical_start_position, "N64 SYSTEM INFORMATION"); // centre = numchars * font_horizontal_pixels / 2
	graphics_draw_line(d, 0, 30, d->width, 30, 0xff);

	int16_t vertical_position = 40;

    time_t current_time = -1;
    current_time = time( NULL );
    if( current_time != -1 )
    {
        sprintf(str_buffer, "Current date & time: %s\n\n", ctime( &current_time ));
        graphics_draw_text(d, horizontal_start_position, vertical_position += font_vertical_pixels, str_buffer);

        graphics_draw_text(d, horizontal_start_position, vertical_position += font_vertical_pixels, "To change the time, use USB App...");
    }

    vertical_position += font_vertical_pixels;

    sprintf(str_buffer, "Expansion PAK is %sinserted\n", is_memory_expanded() ? "" : "not " );
    graphics_draw_text(d, horizontal_start_position, vertical_position += font_vertical_pixels, str_buffer);

    vertical_position += font_vertical_pixels;

    int controllers = get_controllers_present();

    sprintf(str_buffer, "JoyPad 1 is %sconnected\n", (controllers & CONTROLLER_1_INSERTED) ? "" : "not " );
    graphics_draw_text(d, horizontal_start_position, vertical_position += font_vertical_pixels, str_buffer);
    sprintf(str_buffer, "JoyPad 2 is %sconnected\n", (controllers & CONTROLLER_2_INSERTED) ? "" : "not " );
    graphics_draw_text(d, horizontal_start_position, vertical_position += font_vertical_pixels, str_buffer);
    sprintf(str_buffer, "JoyPad 3 is %sconnected\n", (controllers & CONTROLLER_3_INSERTED) ? "" : "not " );
    graphics_draw_text(d, horizontal_start_position, vertical_position += font_vertical_pixels, str_buffer);
    sprintf(str_buffer, "JoyPad 4 is %sconnected\n", (controllers & CONTROLLER_4_INSERTED) ? "" : "not " );
    graphics_draw_text(d, horizontal_start_position, vertical_position += font_vertical_pixels, str_buffer);

    vertical_position += font_vertical_pixels;

    struct controller_data output;
    int accessories = get_accessories_present( &output );

    sprintf(str_buffer, "JoyPad 1 Accessory Pak is %sinserted %s\n", (accessories & CONTROLLER_1_INSERTED) ? "" : "not ", 
                                            (accessories & CONTROLLER_1_INSERTED) ? accessory_type_s( identify_accessory( 0 ) ) : "" );
    graphics_draw_text(d, horizontal_start_position, vertical_position += font_vertical_pixels, str_buffer);
    sprintf(str_buffer, "JoyPad 2 Accessory Pak is %sinserted %s\n", (accessories & CONTROLLER_2_INSERTED) ? "" : "not ",
                                            (accessories & CONTROLLER_2_INSERTED) ? accessory_type_s( identify_accessory( 1 ) ) : "" );
    graphics_draw_text(d, horizontal_start_position, vertical_position += font_vertical_pixels, str_buffer);
    sprintf(str_buffer, "JoyPad 3 Accessory Pak is %sinserted %s\n", (accessories & CONTROLLER_3_INSERTED) ? "" : "not ",
                                            (accessories & CONTROLLER_3_INSERTED) ? accessory_type_s( identify_accessory( 2 ) ) : "" );
    graphics_draw_text(d, horizontal_start_position, vertical_position += font_vertical_pixels, str_buffer);
    sprintf(str_buffer, "JoyPad 4 Accessory Pak is %sinserted %s\n", (accessories & CONTROLLER_4_INSERTED) ? "" : "not ",
                                            (accessories & CONTROLLER_4_INSERTED) ? accessory_type_s( identify_accessory( 3 ) ) : "" );
    graphics_draw_text(d, horizontal_start_position, vertical_position += font_vertical_pixels, str_buffer);


	graphics_draw_line(d, 0, d->height - overscan_vertical_pixels - font_vertical_pixels, d->width,d->height - overscan_vertical_pixels - font_vertical_pixels, 0xff);
	graphics_draw_text(d, (d->width / 2) - 80,d->height - overscan_vertical_pixels, "Press (B) to return!"); // centre = numchars * font_horizontal_pixels / 2

    display_show(d);
}


void view_system_info_init (menu_t *menu) {
    // Nothing to initialize (yet)
}

void view_system_info_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}