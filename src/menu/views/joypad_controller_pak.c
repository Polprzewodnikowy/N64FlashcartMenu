#include "views.h"
#include "../cpak_handler.h"
#include "utils/fs.h"

#define CPAK_BACKUP_DIRECTORY    "/cpak"
#define CPAK_BACKUP_FILE_PREFIX  "cpak_backup"
#define CPAK_BACKUP_FILE_EXT     "pak"


static int accessory_is_cpak[4];
static cpak_info_t cpak_info;
static bool backup_in_progress = false;


static void exec_cpak_backup(menu_t *menu) {
    backup_in_progress = true;
    char file_name[128];
                
    path_t *path = path_init(menu->storage_prefix, CPAK_BACKUP_DIRECTORY);
    directory_create(path_get(path));

    // TODO: preferably with the time added to the filename so it does not overwrite the existing one!
    sprintf(file_name, "%s-%s.%s", CPAK_BACKUP_FILE_PREFIX, "1", CPAK_BACKUP_FILE_EXT);


    path_push(path, file_name);

    //int res = 
    cpak_clone_contents_to_file(path_get(path), JOYPAD_PORT_1);

    // TODO: draw progress bar or error!
    // if (res == CONTROLLER_PAK_OK) {
        
    // }
    // else {
        
    // }

    path_free(path);

    backup_in_progress = false;
}

static void process (menu_t *menu) {

    if (menu->actions.enter) {
        // TODO: handle all ports
        if (accessory_is_cpak[JOYPAD_PORT_1]) {
            exec_cpak_backup(menu);         
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
            "Pages: %d/123. \n"
            "Notes: ?/16. \n", // "Notes: %d/16. \n",
            cpak_info.free_pages
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

    if (backup_in_progress) {
        component_messagebox_draw("Saving...");
    }

    rdpq_detach_show();
}

void view_joypad_controller_pak_init (menu_t *menu){

    // check which paks are available
    JOYPAD_PORT_FOREACH (port) {
        accessory_is_cpak[port] = joypad_get_accessory_type(port) == JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK;
    }

    cpak_info_load(JOYPAD_PORT_1, &cpak_info);
}

void view_joypad_controller_pak_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
