#include <time.h>

#include "views.h"


static int controllers;
static int accessory[4];


static char *format_accessory (int controller) {
    switch (accessory[controller]) {
        case ACCESSORY_RUMBLEPAK:
            return "[Rumble Pak is inserted]";
        case ACCESSORY_MEMPAK:
            return "[Controller Pak is inserted]";
        case ACCESSORY_VRU:
            return "[VRU is inserted]";
        case ACCESSORY_TRANSFERPAK:
            return "[Transfer Pak is inserted]";
        case ACCESSORY_NONE:
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

    time_t current_time = time(NULL);

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
        current_time >= 0 ? ctime(&current_time) : "Unknown\n",
        is_memory_expanded() ? "" : "not ",
        (controllers & CONTROLLER_1_INSERTED) ? "" : "not ", format_accessory(0),
        (controllers & CONTROLLER_2_INSERTED) ? "" : "not ", format_accessory(1),
        (controllers & CONTROLLER_3_INSERTED) ? "" : "not ", format_accessory(2),
        (controllers & CONTROLLER_4_INSERTED) ? "" : "not ", format_accessory(3)
    );

    component_actions_bar_text_draw(
        ALIGN_LEFT, VALIGN_TOP,
        "B: Exit"
    );

    rdpq_detach_show();
}


void view_system_info_init (menu_t *menu) {    
    controllers = get_controllers_present();

    for (int i = 0; i < 4; i++) {
        accessory[i] = identify_accessory(i);
    }
}

void view_system_info_display (menu_t *menu, surface_t *display) {
    process(menu);

    draw(menu, display);
}
