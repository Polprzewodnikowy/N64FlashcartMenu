#include <libdragon.h>
#include <mini.c/src/mini.h>

#include "settings.h"
#include "utils/fs.h"


#define SETTINGS_FILE_PATH  "sd:/menu/config.ini"


static settings_t init = {
    .pal60 = false,
    .show_hidden_files = false,
    .default_directory = "/",
    .use_saves_folder = false,
};


void settings_load (settings_t *settings) {
    if (!file_exists(SETTINGS_FILE_PATH)) {
        settings_save(&init);
    }

    mini_t *ini = mini_try_load(SETTINGS_FILE_PATH);

    settings->pal60 = mini_get_bool(ini, "menu", "pal60", init.pal60);
    settings->show_hidden_files = mini_get_bool(ini, "menu", "show_hidden_files", init.show_hidden_files);
    settings->default_directory = strdup(mini_get_string(ini, "menu", "default_directory", init.default_directory));
    settings->use_saves_folder = mini_get_bool(ini, "menu", "use_saves_folder", init.use_saves_folder);

    mini_free(ini);
}

void settings_save (settings_t *settings) {
    mini_t *ini = mini_create(SETTINGS_FILE_PATH);

    mini_set_bool(ini, "menu", "pal60", settings->pal60);
    mini_set_bool(ini, "menu", "show_hidden_files", settings->show_hidden_files);
    mini_set_string(ini, "menu", "default_directory", settings->default_directory);
    mini_set_bool(ini, "menu", "use_saves_folder", settings->use_saves_folder);

    mini_save(ini);

    mini_free(ini);
}
