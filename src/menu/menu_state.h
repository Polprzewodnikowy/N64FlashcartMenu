/**
 * @file menu_state.h
 * @brief Menu State
 * @ingroup menu 
 */

#ifndef MENU_STRUCT_H__
#define MENU_STRUCT_H__


#include <time.h>

#include "boot/boot.h"
#include "disk_info.h"
#include "flashcart/flashcart.h"
#include "path.h"
#include "rom_info.h"
#include "settings.h"


#define BROWSER_LIST_SIZE   2048


/** @brief Menu mode enumeration */
typedef enum {
    MENU_MODE_NONE,
    MENU_MODE_STARTUP,
    MENU_MODE_BROWSER,
    MENU_MODE_FILE_INFO,
    MENU_MODE_SYSTEM_INFO,
    MENU_MODE_IMAGE_VIEWER,
    MENU_MODE_MUSIC_PLAYER,
    MENU_MODE_CREDITS,
    MENU_MODE_LOAD_ROM,
    MENU_MODE_LOAD_DISK,
    MENU_MODE_LOAD_EMULATOR,
    MENU_MODE_ERROR,
    MENU_MODE_FAULT,
    MENU_MODE_BOOT,
    __MENU_MODE_COUNT,
} menu_mode_t;

/** @brief File entry type enumeration */
typedef enum {
    ENTRY_TYPE_DIR,
    ENTRY_TYPE_ROM,
    ENTRY_TYPE_DISK,
    ENTRY_TYPE_EMULATOR,
    ENTRY_TYPE_SAVE,
    ENTRY_TYPE_IMAGE,
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

    char *error_message;
    flashcart_err_t flashcart_err;

    time_t current_time;

    struct {
        bool go_up;
        bool go_down;
        bool go_left;
        bool go_right;
        bool go_fast;

        bool enter;
        bool back;
        bool options;
        bool system_info;
        bool settings;
    } actions;

    struct {
        bool valid;
        path_t *directory;
        entry_t list[BROWSER_LIST_SIZE];
        int entries;
        entry_t *entry;
        int selected;
    } browser;

    struct {
        path_t *rom_path;
        rom_info_t rom_info;
        path_t *disk_path;
        disk_info_t disk_info;
    } load;
} menu_t;


#endif
