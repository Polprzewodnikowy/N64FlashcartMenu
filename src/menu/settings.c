#include <libdragon.h>

#include "libs/mini.c/src/mini.h"
#include "path.h"
#include "settings.h"
#include "utils/fs.h"


void settings_set_default_state (settings_t *settings) {
    settings->pal60 = false;
    settings->show_hidden_files = false;
    settings->default_directory = "/";
}

void settings_save_to_file (char *path, settings_t *settings) {    
    path_t *config_file_path = path_init("sd:/");
    path_append(config_file_path, path);

    mini_t *ini = mini_create(path_get(config_file_path));

    mini_set_bool(ini, "menu", "pal60", settings->pal60);
    mini_set_bool(ini, "menu", "show_hidden_files", settings->show_hidden_files);
    mini_set_string(ini, "menu", "default_directory", settings->default_directory);

    mini_save(ini);

    mini_free(ini);

    path_free(config_file_path);
}

void settings_load_from_file (char *path, settings_t *settings) {
    if (!file_exists(path)) {
        settings_save_to_file(path, settings);
    }

    path_t *config_file_path = path_init("sd:/");
    path_append(config_file_path, path);

    mini_t *ini = mini_try_load(path_get(config_file_path));

    settings->pal60 = mini_get_bool(ini, "menu", "pal60", false);
    settings->show_hidden_files = mini_get_bool(ini, "menu", "show_hidden_files", false);
    settings->default_directory = strdup(mini_get_string(ini, "menu", "default_directory", "/"));

    mini_free(ini);

    path_free(config_file_path);
}
