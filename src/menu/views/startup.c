#include "utils/fs.h"
#include "views.h"


static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach_clear(d, NULL);
    rdpq_detach_show();
}


void view_startup_init (menu_t *menu) {
    // FIXME: rather than use a controller button, would it be better to use the cart button?
    JOYPAD_PORT_FOREACH (port) {
        joypad_poll();
        joypad_buttons_t b_held = joypad_get_buttons_held(port);

        if (menu->settings.rom_autoload_enabled && b_held.start) {
            menu->settings.rom_autoload_enabled = false;
            menu->settings.rom_autoload_path = "";
            menu->settings.rom_autoload_filename = "";
            settings_save(&menu->settings);
        }
    }
    if (menu->settings.rom_autoload_enabled) {
        //dir_t info;
        path_t *path = path_init(menu->storage_prefix, menu->settings.rom_autoload_path);

        menu->browser.directory = path_clone(path);
        //menu->browser.list = realloc(menu->browser.list, (menu->browser.entries + 1) * sizeof(entry_t));
        // entry_t entry; // = &menu->browser.list[20];

        //dir_findfirst(path_get(path), &info);

        //menu->browser.selected = 0;

        //menu->browser.entry = &menu->browser.list[menu->browser.selected];

        // entry.name = strdup(menu->settings.rom_autoload_filename);
        // entry.type = ENTRY_TYPE_ROM;
        // menu->browser.entry = &entry;
       // menu->browser.entry->name = strdup(menu->settings.rom_autoload_filename);
        menu->load.rom_path = path_clone_push(menu->browser.directory, menu->settings.rom_autoload_filename);

        // entry_t *autoload_entry;
        // autoload_entry->name = strdup(menu->settings.rom_autoload_entry);
        // autoload_entry->type = ENTRY_TYPE_ROM;
        //menu->browser.entry = (entry_t); //autoload_entry;
        // 

        menu->boot_pending.rom_file = true;
        menu->next_mode = MENU_MODE_LOAD_ROM;
        
        return;
    }
    
    menu->next_mode = MENU_MODE_BROWSER;
}

void view_startup_display (menu_t *menu, surface_t *display) {
    draw(menu, display);
}
