#include <libdragon.h>
#include <mini.c/src/mini.h>

#include "settings.h"
#include "utils/fs.h"

#ifndef SETTINGS_FILE_PATH
#define SETTINGS_FILE_PATH  "sd:/menu/config.ini"
#endif

static settings_t init = {
    .pal60_enabled = false,
    .hidden_files_enabled = false,
    .default_directory = "/",
    .use_saves_folder = true,

    /* Beta feature flags */
    .bgm_enabled = false,
    .sound_enabled = false,
    .rumble_enabled = true,
    .auto_firmware_update_enabled = false,
};


void settings_load (settings_t *settings) {
    if (!file_exists(SETTINGS_FILE_PATH)) {
        settings_save(&init);
    }

    mini_t *ini = mini_try_load(SETTINGS_FILE_PATH);

    settings->pal60_enabled = mini_get_bool(ini, "menu", "pal60", init.pal60_enabled); // TODO: consider changing file setting name
    settings->hidden_files_enabled = mini_get_bool(ini, "menu", "show_hidden_files", init.hidden_files_enabled);
    settings->default_directory = strdup(mini_get_string(ini, "menu", "default_directory", init.default_directory));
    settings->use_saves_folder = mini_get_bool(ini, "menu", "use_saves_folder", init.use_saves_folder);
    
    /* Beta feature flags, they might not be in the file */
    settings->bgm_enabled = mini_get_bool(ini, "menu_beta_flag", "bgm_enabled", init.bgm_enabled);
    settings->sound_enabled = mini_get_bool(ini, "menu_beta_flag", "sound_enabled", init.sound_enabled);
    settings->rumble_enabled = mini_get_bool(ini, "menu_beta_flag", "rumble_enabled", init.rumble_enabled);
    settings->auto_firmware_update_enabled = mini_get_bool(ini, "menu_beta_flag", "auto_firmware_update_enabled", init.auto_firmware_update_enabled);


    mini_free(ini);
}

void settings_save (settings_t *settings) {
    mini_t *ini = mini_create(SETTINGS_FILE_PATH);

    mini_set_bool(ini, "menu", "pal60", settings->pal60_enabled);
    mini_set_bool(ini, "menu", "show_hidden_files", settings->hidden_files_enabled);
    mini_set_string(ini, "menu", "default_directory", settings->default_directory);
    mini_set_bool(ini, "menu", "use_saves_folder", settings->use_saves_folder);

    /* Beta feature flags, they should not save until production ready! */
    // mini_set_bool(ini, "menu_beta_flag", "bgm_enabled", init.bgm_enabled);
    // mini_set_bool(ini, "menu_beta_flag", "sound_enabled", init.sound_enabled);
    // mini_set_bool(ini, "menu_beta_flag", "rumble_enabled", init.rumble_enabled);
    // mini_set_bool(ini, "menu_beta_flag", "auto_firmware_update_enabled", init.auto_firmware_update_enabled);

    mini_save(ini, MINI_FLAGS_SKIP_EMPTY_GROUPS);

    mini_free(ini);
}
