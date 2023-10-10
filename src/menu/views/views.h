/**
 * @file views.h
 * @brief Menu Views
 * @ingroup menu 
 */

#ifndef VIEWS_H__
#define VIEWS_H__


#include "../components.h"
#include "../menu_state.h"


/**
 * @addtogroup view
 * @{
 */

void view_startup_init (menu_t *menu);
void view_startup_display (menu_t *menu, surface_t *display);

void view_browser_init (menu_t *menu);
void view_browser_display (menu_t *menu, surface_t *display);

void view_system_info_init (menu_t *menu);
void view_system_info_display (menu_t *menu, surface_t *display);

void view_file_info_init (menu_t *menu);
void view_file_info_display (menu_t *menu, surface_t *display);

void view_image_viewer_init (menu_t *menu);
void view_image_viewer_display (menu_t *menu, surface_t *display);

void view_music_player_init (menu_t *menu);
void view_music_player_display (menu_t *menu, surface_t *display);

void view_credits_init (menu_t *menu);
void view_credits_display (menu_t *menu, surface_t *display);

void view_load_rom_init (menu_t *menu);
void view_load_rom_display (menu_t *menu, surface_t *display);

void view_load_disk_init (menu_t *menu);
void view_load_disk_display (menu_t *menu, surface_t *display);

void view_load_emulator_init (menu_t *menu);
void view_load_emulator_display (menu_t *menu, surface_t *display);

void view_error_init (menu_t *menu);
void view_error_display (menu_t *menu, surface_t *display);

void view_fault_init (menu_t *menu);
void view_fault_display (menu_t *menu, surface_t *display);

void menu_show_error (menu_t *menu, char *error_message);

/** @} */ /* view */


#endif
