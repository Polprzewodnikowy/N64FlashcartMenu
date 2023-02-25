#ifndef SETTINGS_H__
#define SETTINGS_H__

#include "flashcart/flashcart.h"
#include "libs/toml/toml.h"
#include "boot/boot.h"

#define SC64_SETTINGS_FILEPATH "sd://config.sc64.toml.txt"
#define SETTINGS_SCHEMA_VERSION 1


typedef struct {
    char *rom_path;
    char *save_path;
    flashcart_save_type_t save_type; //TODO: should this be converted from a string, or only use an integer value?
    bool save_writeback; // TODO: this is likely SC64 specific.
} settings_last_rom_t;

typedef struct {
    bool auto_load_last_rom;
    char* current_directory;
} settings_last_state_t;

typedef struct {
    settings_last_rom_t last_rom;
    settings_last_state_t last_state;
    boot_params_t boot_params;
} settings_t;

// TODO:
// Menu layout: list vs grid
// Hide extensions
// Hide hidden files and dirs
// colour scheme
// background pic

void settings_load_from_file (settings_t *settings);
void settings_save (void);
void settings_reset (void);
void settings_load_default_state(settings_t *settings);
void settings_save_default_state(void);
void settings_validate (void);

#endif
