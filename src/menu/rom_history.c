#include <libdragon.h>
#include <mini.c/src/mini.h>

#include "rom_history.h"
#include "utils/fs.h"
#include "path.h"

#define EMULATOR_TEST true

static char *history_path = NULL;

static path_t* empty_path = NULL;
static history_t init;

/** @brief Init history path */
void history_init (char *path) {
    if (history_path) {
        free(history_path);
    }
    history_path = strdup(path);
    empty_path = path_create("");
}

/** @brief The history to load */
void history_load (history_t *history) {
    if (!file_exists(history_path)) {
        history_save(&init);
    }

    mini_t *ini = mini_try_load(history_path);
    history->last_rom = path_create(mini_get_string(ini, "history", "last_rom", ""));
    history->last_disk = path_create(mini_get_string(ini, "history", "last_disk", ""));

    char buffer[1024];

    for(int i=0;i<FAVORITES_COUNT;i++) {
#if EMULATOR_TEST
        sprintf(buffer, "test:\\somepaths\\blah\\rom - %d.n64", i);
        history->favorites_rom[i] = path_create(buffer);
#else
        sprintf(buffer,"%dfavorite_rom", i);
        history->favorites_rom[i] = path_create(mini_get_string(ini, "favorite", buffer, ""));
#endif
    }
    
    for(int i=0;i<FAVORITES_COUNT;i++) {
#if EMULATOR_TEST
        sprintf(buffer, "test:\\aother\\foo\\disk - %d.dd", i);
        history->favorites_disk[i] = path_create(buffer);
#else
        sprintf(buffer,"%dfavorite_disk", i);
        history->favorites_disk[i] = path_create(mini_get_string(ini, "favorite", buffer, ""));
#endif        
    }

    mini_free(ini);
}

/** @brief The history to save */
void history_save (history_t *history)
{
    mini_t *ini = mini_create(history_path);

    mini_set_string(ini, "history", "last_rom",  history->last_rom != NULL ? path_get(history->last_rom) : "");
    mini_set_string(ini, "history", "last_disk", history->last_disk != NULL ? path_get(history->last_disk) : "");

    char buf[20];
    for(int i=0;i<FAVORITES_COUNT;i++) {
        sprintf(buf,"%dfavorite_rom", i);
        path_t* path = history->favorites_rom[i];
        mini_set_string(ini, "favorite", buf, path != NULL ? path_get(path) : "");
    }

    for(int i=0;i<FAVORITES_COUNT;i++) {
        sprintf(buf,"%dfavorite_disk", i);
        path_t* path = history->favorites_disk[i];
        mini_set_string(ini, "favorite", buf, path != NULL ? path_get(path) : "");        
    }

    mini_save(ini, MINI_FLAGS_SKIP_EMPTY_GROUPS);
    mini_free(ini);    
}



static void history_last_rom_update(history_t *history, path_t* path) {
    if(history->last_rom != NULL) {    
        path_free(history->last_rom);
    }

    if(path != NULL) {
        history->last_rom = path_clone(path);
    } else {
        history->last_rom = path_create("");
    }
}


static void history_last_disk_update(history_t *history, path_t* path) {
    if(history->last_disk != NULL) {    
        path_free(history->last_disk);
    }

    if(path != NULL) {
        history->last_disk = path_clone(path);
    } else {
        history->last_disk = path_create("");
    }
}

void history_last_rom_set(history_t *history, path_t* disk_path, path_t* rom_path)
{
    history_last_rom_update(history, rom_path);
    history_last_disk_update(history, disk_path);

    history_save(history);
}

static void history_favorite_insert(history_t *history, int location, path_t* rom, path_t* disk) {
    if(history->favorites_rom[location]) {
         path_free(history->favorites_rom[location]);
    }
    if(history->favorites_disk[location]) {
         path_free(history->favorites_disk[location]);
    }

    history->favorites_rom[location] = rom != NULL ? path_clone(rom) : path_create("");
    history->favorites_disk[location] = rom != NULL ? path_clone(disk) : path_create("");
}


static void history_favorite_replace(history_t *history, int toReplace, path_t* rom, path_t* disk) {
    if(history->favorites_rom[toReplace]) {
        path_free(history->favorites_rom[toReplace]);
        history->favorites_rom[toReplace] = NULL;
    }
    if(rom != NULL) {
        history->favorites_rom[toReplace] = path_clone(rom);
    }

    if(history->favorites_disk[toReplace]) {
        path_free(history->favorites_disk[toReplace]);
        history->favorites_disk[toReplace] = NULL;
    }
    if(disk != NULL) {
        history->favorites_disk[toReplace] = path_clone(disk);
    }    
}

static void history_favorite_rotate(history_t *history)
{
    for(int i=1;i<FAVORITES_COUNT-1;i++) {
        int previous = i-1;
        history_favorite_replace(history, previous, history->favorites_rom[i], history->favorites_disk[i]);
    }

    history_favorite_replace(history, FAVORITES_COUNT -1, NULL, NULL);
}

static bool history_favorite_check_match(path_t* left, path_t* right) {
    bool matches = false;
    if(left == NULL && right == NULL) {
        matches = true;
    } else {
        if(left != NULL && right != NULL) {
            matches = (strcmp(path_get(left), path_get(right)) == 0);
        }
    }  

    return matches;
}

static bool history_favorite_exists(history_t *history, path_t* rom, path_t* disk) {
    for(int i=0;i<FAVORITES_COUNT;i++) {
        bool romMatch = history_favorite_check_match(history->favorites_rom[i], rom);
        bool diskMatch = history_favorite_check_match(history->favorites_disk[i], disk);

        if(romMatch && diskMatch) {
            return true;
        }        
    }

    return false;
}

void history_favorite_add(history_t *history, path_t* rom, path_t* disk) {
    // if the game is already in the favorite list then don't add again

    if(rom == NULL) {
        rom = empty_path;
    }
    if(disk == NULL) {
        disk = empty_path;
    }

    if(history_favorite_exists(history, rom, disk)) {
        return;
    }

    // look for a free space in the list
    int place = -1;
    for(int i=0;i < FAVORITES_COUNT;i++) {
        if(!(path_has_value(history->favorites_rom[i]) || path_has_value(history->favorites_disk[i]))) {
            place = i;
            break;
        }
    }

    // if no free space then rotate the favorite list (first in first out)
    // and set the place to add at the end of the list
    if(place == -1) {
        history_favorite_rotate(history);
        place = FAVORITES_COUNT - 1;
    }

    history_favorite_insert(history, place, rom, disk);
    history_save(history);
}

void history_favorite_remove(history_t *history, int location)
{
    if(location >= 0 && location < FAVORITES_COUNT) {
        if(path_has_value(history->favorites_rom[location])) {
            path_free(history->favorites_rom[location]);
            history->favorites_rom[location] = path_create("");
        }
        
        if(path_has_value(history->favorites_disk[location])) {
            path_free(history->favorites_disk[location]);
            history->favorites_disk[location] = path_create("");
        }
    }
}