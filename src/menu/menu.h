#ifndef MENU_H__
#define MENU_H__


#include "path.h"
#include "settings.h"


#define BROWSER_LIST_SIZE   4096


typedef enum {
    MENU_MODE_INIT,
    MENU_MODE_BROWSER,
    MENU_MODE_FILE_INFO,
    MENU_MODE_SYSTEM_INFO,
    MENU_MODE_CREDITS,
    MENU_MODE_LOAD,
    MENU_MODE_ERROR,
    MENU_MODE_BOOT,
} menu_mode_t;

typedef enum {
    ENTRY_TYPE_DIR,
    ENTRY_TYPE_ROM,
    ENTRY_TYPE_SAVE,
    ENTRY_TYPE_UNKNOWN,
} entry_type_t;

typedef struct {
    char *name;
    entry_type_t type;
} entry_t;

typedef struct {
    menu_mode_t mode;
    menu_mode_t next_mode;

    struct {
        bool go_up;
        bool go_down;
        bool fast;
        bool enter;
        bool back;
        bool fileinfo;
        bool sysinfo;
        bool settings;
        int held_counter;
    } actions;

    struct {
        bool valid;
        path_t *directory;
        entry_t list[BROWSER_LIST_SIZE];
        int entries;
        int selected;
    } browser;
} menu_t;


void menu_run (settings_t *settings);


#endif
