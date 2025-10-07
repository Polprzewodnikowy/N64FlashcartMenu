#include "utils/fs.h"
#include "views.h"


static void draw (menu_t *menu, surface_t *d) {
    rdpq_attach_clear(d, NULL);
    rdpq_detach_show();
}


/**
 * Initialize startup view state and decide the next menu mode.
 *
 * When ROM autoload support is compiled in and autoload is enabled, this function
 * checks controller input to allow cancelling autoload (clearing and persisting
 * autoload settings if cancelled). If autoload remains enabled, it prepares the
 * browser/load paths, marks a ROM load as pending, sets the next mode to
 * MENU_MODE_LOAD_ROM, and returns early. Otherwise, for a first run it clears
 * the first-run flag, persists settings, and sets the next mode to
 * MENU_MODE_CREDITS; on subsequent runs it sets the next mode to MENU_MODE_BROWSER.
 *
 * @param menu Pointer to the menu state to initialize and update (may be modified).
 */
void view_startup_init (menu_t *menu) {
#ifdef FEATURE_AUTOLOAD_ROM_ENABLED
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
        menu->browser.directory = path_init(menu->storage_prefix, menu->settings.rom_autoload_path);
        menu->load.rom_path = path_clone_push(menu->browser.directory, menu->settings.rom_autoload_filename);
        menu->load_pending.rom_file = true;
        menu->next_mode = MENU_MODE_LOAD_ROM;

        return;
    }
#endif
    
    if (menu->settings.first_run) {
        menu->settings.first_run = false;
        settings_save(&menu->settings);
        menu->next_mode = MENU_MODE_CREDITS;
    }
    else {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

void view_startup_display (menu_t *menu, surface_t *display) {
    draw(menu, display);
}