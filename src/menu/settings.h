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

    /** @brief Show files marked as hidden in the browser */
    bool hidden_files_enabled;

    /** @brief Default directory to navigate to when menu loads */
    char *default_directory;

    /** @brief Put saves into separate directory */
    bool use_saves_folder;

    /** @brief Enable Background music */
    bool bgm_enabled;

    /** @brief Background music file path */
    char *bgm_file_path;

    /** @brief Enable Sounds */
    bool sound_enabled;

    /** @brief Enable rumble feedback */
    bool rumble_enabled;

} settings_t;


void settings_load (settings_t *settings);
void settings_save (settings_t *settings);


#endif
