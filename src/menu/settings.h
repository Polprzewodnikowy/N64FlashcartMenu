/**
 * @file settings.h
 * @brief Menu Settings
 * @ingroup menu 
 */

#ifndef SETTINGS_H__
#define SETTINGS_H__


/** @brief Settings Structure */
typedef struct {
    /** @brief Use 60 Hz refresh rate on a PAL console */
    bool pal60_enabled;

    /** @brief Show files/directories that are filtered in the browser */
    bool show_protected_entries;

    /** @brief Default directory to navigate to when menu loads */
    char *default_directory;

    /** @brief Put saves into separate directory */
    bool use_saves_folder;

    /** @brief Enable Background music */
    bool bgm_enabled;

    /** @brief Enable Sounds */
    bool sound_enabled;

    /** @brief Enable rumble feedback */
    bool rumble_enabled;

    /** @brief Enable the ability to bypass the menu and instantly load a ROM */
    bool rom_autoload_enabled;

    /** @brief A path to the autoloaded ROM */
    char *rom_autoload_path;

    /** @brief Enable favourite roms */
    char *rom_favourites[15];

    /** @brief Enable recent roms */
    char *roms_recent[15];
} settings_t;


/** @brief Init settings path */
void settings_init (char *path);
/** @brief The settings to load */
void settings_load (settings_t *settings);
/** @brief The settings to save */
void settings_save (settings_t *settings);


#endif
