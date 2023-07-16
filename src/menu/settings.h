/**
 * @file settings.h
 * @brief Menu Settings
 * @ingroup menu 
 */

#ifndef SETTINGS_H__
#define SETTINGS_H__


/** @brief Settings Structure */
typedef struct {
    /** @brief Select 60 Hz refresh rate if running on PAL console */
    bool pal60;

    /** @brief Show files marked as hidden in the browser */
    bool show_hidden_files;

    /** @brief Default directory to navigate to when menu loads */
    char *default_directory;
} settings_t;


void settings_load (settings_t *settings);
void settings_save (settings_t *settings);


#endif
