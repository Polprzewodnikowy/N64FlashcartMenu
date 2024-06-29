#include "views.h"
#include "../cpak_handler.h"

#define CPAK_BACKUP_DIRECTORY    "/cpak"
#define CPAK_BACKUP_FILE         "cpak_backup.pak"


static int accessory_is_cpak[4];
static cpak_info_t cpak_info;

static void process (menu_t *menu) {

    // check which paks are available
    JOYPAD_PORT_FOREACH (port) {
        accessory_is_cpak[port] = joypad_get_accessory_type(port) == JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK;
    }

    cpak_info_load(JOYPAD_PORT_1, &cpak_info);

    if (menu->actions.enter) {
        // TODO: handle all ports
        if (accessory_is_cpak[JOYPAD_PORT_1]) {
            // TODO: preferably with the time added to the filename so it does not overwrite the existing one!
            path_t *path = path_init(menu->storage_prefix, CPAK_BACKUP_DIRECTORY"/"CPAK_BACKUP_FILE);
            cpak_clone_contents_to_file(path_get(path), JOYPAD_PORT_1);
            path_free(path);
        }
    }

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
        "CONTROLLER PAK MENU\n"
        "\n"
        "\n"
    );

    // TODO: Backup from other ports, restore from SD, and/or Repair functions.
    // Bonus would be to handle individual per game entries!
    if (accessory_is_cpak[0]) {
        component_main_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "Controller Pak (1).\n"
            "Free space: %d blocks. \n",
            cpak_info.free_space
        );
    }
    else {
        component_main_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "Controller Pak (1).\n"
            "  Not inserted.\n"
        );
    }

    if (accessory_is_cpak[0]) {
        component_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "A: Clone\n"
            "B: Back"
        );
    }
    else {
        component_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "B: Back"
        );
    }

    rdpq_detach_show();
}

void view_joypad_controller_pak_init (menu_t *menu){
    // Nothing to initialize (yet)
}

void view_joypad_controller_pak_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
