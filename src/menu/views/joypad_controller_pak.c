#include "views.h"
#include "../cpak_handler.h"
#include "utils/fs.h"

#define CPAK_MAX_PAGES 123
#define CPAK_MAX_NOTES 16
#define CPAK_BACKUP_DIRECTORY    "/cpak_backups"
#define CPAK_BACKUP_FILE_PREFIX  "cpak_backup"
#define CPAK_BACKUP_FILE_EXT     "pak"


static int accessory_is_cpak[4];
static cpak_info_t cpak_info;
static bool backup_in_progress = false;


const static char *format_cpak_entries(entry_structure_t *entries) {
    // TODO: either show the note descriptions, or show the notes used.
    // for (int i = 0; i< 16; i++) {
        
    // }
    return "?/16"; //CPAK_MAX_NOTES
}

static void exec_cpak_backup(menu_t *menu, uint8_t port) {
    backup_in_progress = true;
    char file_name[32];
                
    path_t *path = path_init(menu->storage_prefix, CPAK_BACKUP_DIRECTORY);
    directory_create(path_get(path));

    sprintf(file_name, "%s-%lld.%s", CPAK_BACKUP_FILE_PREFIX, time( NULL ), CPAK_BACKUP_FILE_EXT);

    path_push(path, file_name);

    //int res = 
    cpak_clone_contents_to_file(path_get(path), port);

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
            exec_cpak_backup(menu, JOYPAD_PORT_1);         
        }
    } else if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach(d, NULL);

    ui_components_background_draw();

    if (backup_in_progress) {
        ui_components_messagebox_draw("Saving...");
    }

    ui_components_layout_draw();

    // TODO: Backup from other ports, restore from SD, and/or Repair functions.
	ui_components_main_text_draw(
        ALIGN_CENTER, VALIGN_TOP,
        "CONTROLLER PAK MENU\n"
        "\n"
        "This menu only supports cloning the\n"
        "Controller Pak connected to JoyPad 1.\n"
    );

    // Bonus would be to handle individual per game entries!
    if (accessory_is_cpak[0]) {
        ui_components_main_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "\n"
            "\n"
            "\n"
            "Controller Pak (1).\n"
            "  Pages: %d/%d. \n"
            "  Notes: %s.",
            cpak_info.free_pages, CPAK_MAX_PAGES,
            format_cpak_entries(cpak_info.entries)
        );
    }
    else {
        ui_components_main_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "\n"
            "\n"
            "\n"
            "\n"
            "Controller Pak (1).\n"
            "  Not inserted.\n"
            "  - If it is, retry inserting it a few times.\n"
            "  - Load another ROM that has a Controller Pak manager.\n"
        );
    }

    if (accessory_is_cpak[0]) {
        ui_components_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "A: Clone to SD Card\n"
            "B: Back"
        );
    }
    else {
        ui_components_actions_bar_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "\n"
            "B: Back"
        );
    }

    rdpq_detach_show();
}

void view_joypad_controller_pak_init (menu_t *menu){

    //TODO: handle all paks.
    // check which paks are available
    // JOYPAD_PORT_FOREACH (port) {
    //     accessory_is_cpak[port] = joypad_get_accessory_type(port) == JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK;
    // }

    if (joypad_get_accessory_type(JOYPAD_PORT_1) == JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK) {
        accessory_is_cpak[JOYPAD_PORT_1] = JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK;
        cpak_info_load(JOYPAD_PORT_1, &cpak_info);
    }
}

void view_joypad_controller_pak_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
