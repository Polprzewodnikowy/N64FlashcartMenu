#ifndef MENU_STRUCT_H__
#define MENU_STRUCT_H__


#include <rdpq_font.h>

#include "path.h"


#define BROWSER_LIST_SIZE   10000


typedef enum {
    MENU_MODE_NONE,
    MENU_MODE_STARTUP,
    MENU_MODE_BROWSER,
    MENU_MODE_FILE_INFO,
    MENU_MODE_PLAYER,
    MENU_MODE_CREDITS,
    MENU_MODE_LOAD,
    MENU_MODE_ERROR,
    MENU_MODE_BOOT,
} menu_mode_t;

typedef enum {
    ENTRY_TYPE_DIR,
    ENTRY_TYPE_ROM,
    ENTRY_TYPE_SAVE,
    ENTRY_TYPE_MUSIC,
    ENTRY_TYPE_OTHER,
} entry_type_t;

typedef struct {
    char *name;
    entry_type_t type;
    int size;
} entry_t;

typedef struct {
    menu_mode_t mode;
    menu_mode_t next_mode;

    struct {
        rdpq_font_t *font;
        int font_height;
    } assets;

    struct {
        bool go_up;
        bool go_down;
        bool fast;
        bool enter;
        bool back;
        bool info;
        bool settings;
        bool override;
        int held_counter;
    } actions;

    struct {
        bool valid;
        path_t *directory;
        entry_t list[BROWSER_LIST_SIZE];
        int entries;
        int selected;
        bool show_hidden;
    } browser;

    struct {
        path_t *path;
    } player;
} menu_t;


#endif
