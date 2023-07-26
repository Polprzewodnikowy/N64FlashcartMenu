#include <time.h>
#include <libdragon.h>

#include "fragments/fragments.h"
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

    layout_t *layout = layout_get();

    const int text_x = layout->offset_x + layout->offset_text_x;
    int text_y = layout->offset_y + layout->offset_text_y;

    const color_t bg_color = RGBA32(0x00, 0x00, 0x00, 0xFF);
    const color_t text_color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF);

    rdpq_attach(d, NULL);
    rdpq_clear(bg_color);

    // Layout
    fragment_borders(d);

    // Text start
    fragment_text_start(text_color);

    text_y += fragment_textf((d->width / 2) - 108, text_y, "N64 SYSTEM INFORMATION\n\n");

    text_y += fragment_textf(text_x, text_y, "\n");


    time_t current_time = -1;
    current_time = time( NULL );
    if( current_time != -1 )
    {
        text_y += fragment_textf(text_x, text_y, "Current date & time: %s\n\n", ctime( &current_time ));
        text_y += fragment_textf(text_x, text_y, "To change the time, use USB App...");
    }

    text_y += fragment_textf(text_x, text_y, "\n");

    text_y += fragment_textf(text_x, text_y, "Expansion PAK is %sinserted\n", is_memory_expanded() ? "" : "not " );

    text_y += fragment_textf(text_x, text_y, "\n");

    int controllers = get_controllers_present();

    text_y += fragment_textf(text_x, text_y, "JoyPad 1 is %sconnected\n", (controllers & CONTROLLER_1_INSERTED) ? "" : "not " );
    text_y += fragment_textf(text_x, text_y, "JoyPad 2 is %sconnected\n", (controllers & CONTROLLER_2_INSERTED) ? "" : "not " );
    text_y += fragment_textf(text_x, text_y, "JoyPad 3 is %sconnected\n", (controllers & CONTROLLER_3_INSERTED) ? "" : "not " );
    text_y += fragment_textf(text_x, text_y, "JoyPad 4 is %sconnected\n", (controllers & CONTROLLER_4_INSERTED) ? "" : "not " );

    text_y += fragment_textf(text_x, text_y, "\n");

    struct controller_data output;
    int accessories = get_accessories_present( &output );

    text_y += fragment_textf(text_x, text_y, "JoyPad 1 Accessory Pak is %sinserted %s\n", (accessories & CONTROLLER_1_INSERTED) ? "" : "not ", 
                                            (accessories & CONTROLLER_1_INSERTED) ? accessory_type_s( identify_accessory( 0 ) ) : "" );
    text_y += fragment_textf(text_x, text_y, "JoyPad 2 Accessory Pak is %sinserted %s\n", (accessories & CONTROLLER_2_INSERTED) ? "" : "not ",
                                            (accessories & CONTROLLER_2_INSERTED) ? accessory_type_s( identify_accessory( 1 ) ) : "" );
    text_y += fragment_textf(text_x, text_y, "JoyPad 3 Accessory Pak is %sinserted %s\n", (accessories & CONTROLLER_3_INSERTED) ? "" : "not ",
                                            (accessories & CONTROLLER_3_INSERTED) ? accessory_type_s( identify_accessory( 2 ) ) : "" );
    text_y += fragment_textf(text_x, text_y, "JoyPad 4 Accessory Pak is %sinserted %s\n", (accessories & CONTROLLER_4_INSERTED) ? "" : "not ",
                                            (accessories & CONTROLLER_4_INSERTED) ? accessory_type_s( identify_accessory( 3 ) ) : "" );


    // Actions bar
    text_y = layout->actions_y + layout->offset_text_y;
    text_y += fragment_textf(text_x, text_y, "B: Exit");

    rdpq_detach_show();
}


void view_system_info_init (menu_t *menu) {
    // Nothing to initialize (yet)
}

void view_system_info_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}