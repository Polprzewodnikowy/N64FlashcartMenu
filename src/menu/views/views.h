/**
 * @file views.h
 * @brief Menu Views
 * @ingroup menu 
 */

#ifndef VIEWS_H__
#define VIEWS_H__

#include "../ui_components.h"
#include "../menu_state.h"

/**
 * @addtogroup view
 * @{
 */

/**
 * @brief Initialize the startup view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_startup_init(menu_t *menu);

/**
 * @brief Display the startup view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_startup_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the browser view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_browser_init(menu_t *menu);

/**
 * @brief Display the browser view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_browser_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the file info view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_file_info_init(menu_t *menu);

/**
 * @brief Display the file info view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_file_info_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the system info view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_system_info_init(menu_t *menu);

/**
 * @brief Display the system info view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_system_info_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the image viewer view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_image_viewer_init(menu_t *menu);

/**
 * @brief Display the image viewer view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_image_viewer_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the text viewer view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_text_viewer_init(menu_t *menu);

/**
 * @brief Display the text viewer view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_text_viewer_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the music player view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_music_player_init(menu_t *menu);

/**
 * @brief Display the music player view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_music_player_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the credits view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_credits_init(menu_t *menu);

/**
 * @brief Display the credits view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_credits_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the settings view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_settings_init(menu_t *menu);

/**
 * @brief Display the settings view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_settings_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the RTC view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_rtc_init(menu_t *menu);

/**
 * @brief Display the RTC view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_rtc_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the flashcart info view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_flashcart_info_init(menu_t *menu);

/**
 * @brief Display the flashcart info view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_flashcart_info_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the load ROM view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_load_rom_init(menu_t *menu);

/**
 * @brief Display the load ROM view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_load_rom_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the load disk view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_load_disk_init(menu_t *menu);

/**
 * @brief Display the load disk view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_load_disk_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the load emulator view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_load_emulator_init(menu_t *menu);

/**
 * @brief Display the load emulator view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_load_emulator_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the error view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_error_init(menu_t *menu);

/**
 * @brief Display the error view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_error_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the fault view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_fault_init(menu_t *menu);

/**
 * @brief Display the fault view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_fault_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the favorite view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_favorite_init(menu_t *menu);

/**
 * @brief Display the favorite view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_favorite_display(menu_t *menu, surface_t *display);

/**
 * @brief Initialize the history view.
 * 
 * @param menu Pointer to the menu structure.
 */
void view_history_init(menu_t *menu);

/**
 * @brief Display the history view.
 * 
 * @param menu Pointer to the menu structure.
 * @param display Pointer to the display surface.
 */
void view_history_display(menu_t *menu, surface_t *display);

/**
 * @brief Show an error message in the menu.
 * 
 * @param menu Pointer to the menu structure.
 * @param error_message Error message to be displayed.
 */
void menu_show_error(menu_t *menu, char *error_message);

/** @} */ /* view */

#endif // VIEWS_H__
