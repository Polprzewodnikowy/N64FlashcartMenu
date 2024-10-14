#include <libdragon.h>
#include <mini.c/src/mini.h>

#include "settings.h"
#include "utils/fs.h"


static char *settings_path = NULL;


static settings_t init = {
    .pal60_enabled = false,
    .show_protected_entries = false,
    .default_directory = "/",
    .use_saves_folder = true,
    .sound_enabled = true,
    .rom_autoload_enabled = false,
    .rom_autoload_path = "",
    
    /* Beta feature flags (should always init to off) */
    .bgm_enabled = false,
    .rumble_enabled = false,
};


void settings_init (char *path) {
    if (settings_path) {
        free(settings_path);
    }
    settings_path = strdup(path);
}

void settings_load (settings_t *settings) {
    if (!file_exists(settings_path)) {
        settings_save(&init);
    }

    mini_t *ini = mini_try_load(settings_path);

    settings->pal60_enabled = mini_get_bool(ini, "menu", "pal60", init.pal60_enabled); // TODO: consider changing file setting name
    settings->show_protected_entries = mini_get_bool(ini, "menu", "show_protected_entries", init.show_protected_entries);
    settings->default_directory = strdup(mini_get_string(ini, "menu", "default_directory", init.default_directory));
    settings->use_saves_folder = mini_get_bool(ini, "menu", "use_saves_folder", init.use_saves_folder);
    settings->sound_enabled = mini_get_bool(ini, "menu", "sound_enabled", init.sound_enabled);

    settings->rom_autoload_enabled = mini_get_bool(ini, "menu", "autoload_rom_enabled", init.rom_autoload_enabled);
    settings->rom_autoload_path = strdup(mini_get_string(ini, "autoload", "rom_path", init.rom_autoload_path));

    /* Beta feature flags, they might not be in the file */
    settings->bgm_enabled = mini_get_bool(ini, "menu_beta_flag", "bgm_enabled", init.bgm_enabled);
    settings->rumble_enabled = mini_get_bool(ini, "menu_beta_flag", "rumble_enabled", init.rumble_enabled);

    mini_free(ini);
}

void settings_save (settings_t *settings) {
    mini_t *ini = mini_create(settings_path);

    mini_set_bool(ini, "menu", "pal60", settings->pal60_enabled);
    mini_set_bool(ini, "menu", "show_protected_entries", settings->show_protected_entries);
    mini_set_string(ini, "menu", "default_directory", settings->default_directory);
    mini_set_bool(ini, "menu", "use_saves_folder", settings->use_saves_folder);
    mini_set_bool(ini, "menu", "sound_enabled", settings->sound_enabled);
    mini_set_bool(ini, "menu", "autoload_rom_enabled", settings->rom_autoload_enabled);
    mini_set_string(ini, "autoload", "rom_path", settings->rom_autoload_path);

    /* Beta feature flags, they should not save until production ready! */
    // mini_set_bool(ini, "menu_beta_flag", "bgm_enabled", settings->bgm_enabled);
    // mini_set_bool(ini, "menu_beta_flag", "rumble_enabled", settings->rumble_enabled);

    mini_save(ini, MINI_FLAGS_SKIP_EMPTY_GROUPS);

    mini_free(ini);
}
