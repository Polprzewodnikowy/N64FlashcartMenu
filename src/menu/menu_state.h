/**
 * @file menu_state.h
 * @brief Menu State
 * @ingroup menu 
 */

#ifndef MENU_STRUCT_H__
#define MENU_STRUCT_H__


#include "boot/boot.h"
#include "flashcart/flashcart.h"
#include "path.h"
#include "settings.h"


#define BROWSER_LIST_SIZE   10000


/** @brief Menu mode enumeration */
typedef enum {
    MENU_MODE_NONE,
    MENU_MODE_STARTUP,
    MENU_MODE_BROWSER,
    MENU_MODE_FILE_INFO,
    MENU_MODE_SYSTEM_INFO,
    MENU_MODE_PLAYER,
    MENU_MODE_CREDITS,
    MENU_MODE_LOAD,
    MENU_MODE_ERROR,
    MENU_MODE_FAULT,
    MENU_MODE_BOOT,
} menu_mode_t;

/** @brief File entry type enumeration */
typedef enum {
    ENTRY_TYPE_DIR,
    ENTRY_TYPE_ROM,
    ENTRY_TYPE_SAVE,
    ENTRY_TYPE_MUSIC,
    ENTRY_TYPE_OTHER,
} entry_type_t;

/** @brief File Entry Structure */
typedef struct {
    char *name;
    entry_type_t type;
    int size;
} entry_t;

/** @brief Menu Structure */
typedef struct {
    menu_mode_t mode;
    menu_mode_t next_mode;

    settings_t settings;
    boot_params_t *boot_params;
    flashcart_error_t flashcart_error;

    struct {
        bool go_up;
        bool go_down;
        bool go_left;
        bool go_right;
        bool fast;
        int vertical_held_counter;
        int horizontal_held_counter;

        bool enter;
        bool back;
        bool file_info;
        bool system_info;
        bool settings;
    } actions;

    struct {
        bool valid;
        path_t *directory;
        entry_t list[BROWSER_LIST_SIZE];
        int entries;
        int selected;
    } browser;
} menu_t;


#endif
