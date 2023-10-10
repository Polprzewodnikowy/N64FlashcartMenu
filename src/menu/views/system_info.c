#include <time.h>

#include "views.h"


static int joypad[4];
static int accessory[4];


static char *format_accessory (int joypad) {
    switch (accessory[joypad]) {
        case JOYPAD_ACCESSORY_TYPE_RUMBLE_PAK:
            return "[Rumble Pak is inserted]";
        case JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK:
            return "[Controller Pak is inserted]";
        case JOYPAD_ACCESSORY_TYPE_TRANSFER_PAK:
            return "[Transfer Pak is inserted]";
        case JOYPAD_ACCESSORY_TYPE_BIO_SENSOR:
            return "[BIO Sensor is inserted]";
        case JOYPAD_ACCESSORY_TYPE_SNAP_STATION:
            return "[Snap Station is inserted]";
        case JOYPAD_ACCESSORY_TYPE_NONE:
            return "";
        default:
            return "[unknown accessory inserted]";
    }
}


static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    component_background_draw();

    component_layout_draw();

    component_main_text_draw(
        ALIGN_CENTER, VALIGN_TOP,
        "N64 SYSTEM INFORMATION"
    );

    component_main_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "\n"
        "Current date & time: %s"
        "\n"
        "Expansion PAK is %sinserted\n"
        "\n"
        "JoyPad 1 is %sconnected %s\n"
        "JoyPad 2 is %sconnected %s\n"
        "JoyPad 3 is %sconnected %s\n"
        "JoyPad 4 is %sconnected %s\n",
        menu->current_time >= 0 ? ctime(&menu->current_time) : "Unknown\n",
        is_memory_expanded() ? "" : "not ",
        (joypad[0]) ? "" : "not ", format_accessory(0),
        (joypad[1]) ? "" : "not ", format_accessory(1),
        (joypad[2]) ? "" : "not ", format_accessory(2),
        (joypad[3]) ? "" : "not ", format_accessory(3)
    );

    component_actions_bar_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "\n"
        "B: Exit"
    );

    rdpq_detach_show();
}


void view_system_info_init (menu_t *menu) {
    JOYPAD_PORT_FOREACH (port) {
        joypad[port] = (joypad_get_style(port) != JOYPAD_STYLE_NONE);
        accessory[port] = joypad_get_accessory_type(port);
    }
}

void view_system_info_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);
}
