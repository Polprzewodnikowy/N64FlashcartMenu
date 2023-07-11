#ifndef SETTINGS_H__
#define SETTINGS_H__


typedef struct {
    bool pal60;
    bool show_hidden_files;
    char *default_directory;
} settings_t;


void settings_set_default_state (settings_t *settings);
void settings_save_to_file (char *path, settings_t *settings);
void settings_load_from_file (char *path, settings_t *settings);


#endif
