#include <libdragon.h>
#include <mini.c/src/mini.h>

#include "history.h"
#include "utils/fs.h"
#include "path.h"


static char *history_path = NULL;


static history_t init;

/** @brief Init history path */
void history_init (char *path)
{
    if (history_path) {
        free(history_path);
    }
    history_path = strdup(path);
}

/** @brief The history to load */
void history_load (history_t *history)
{
    if (!file_exists(history_path)) {
        history_save(&init);
    }

    mini_t *ini = mini_try_load(history_path);
    history->last_rom = path_create(mini_get_string(ini, "history", "last_rom", ""));
    history->last_disk = path_create(mini_get_string(ini, "history", "last_64dd", ""));


    // xfavorite_Rom
    char buf[16];
    sprintf(buf,"0favorite_rom");

    for(int i=0;i<FAVORITES_COUNT;i++)
    {
        buf[0] = (char)(48 + i);
        history->favorites_rom[i] = path_create(mini_get_string(ini, "favorite", buf, ""));
    }

    sprintf(buf,"0favorite_disk");

    for(int i=0;i<FAVORITES_COUNT;i++)
    {
        buf[0] = (char)(48 + i);
        history->favorites_disk[i] = path_create(mini_get_string(ini, "favorite", buf, ""));
    }

    mini_free(ini);
}

/** @brief The history to save */
void history_save (history_t *history)
{
    mini_t *ini = mini_create(history_path);

    mini_set_string(ini, "history", "last_rom", history->last_rom != NULL ? path_get(history->last_rom) : "");


    // xfavorite_Rom
    char buf[16];
    sprintf(buf,"0favorite_rom");

    for(int i=0;i<FAVORITES_COUNT;i++)
    {
        buf[0] = (char)(48 + i);
        path_t* path = history->favorites_rom[i];
        mini_set_string(ini, "favorite", buf, path != NULL ? path_get(path) : "");
    }

    sprintf(buf,"0favorite_disk");

    for(int i=0;i<FAVORITES_COUNT;i++)
    {
        buf[0] = (char)(48 + i);
        path_t* path = history->favorites_disk[i];
        mini_set_string(ini, "favorite", buf, path != NULL ? path_get(path) : "");        
    }


    mini_save(ini, MINI_FLAGS_SKIP_EMPTY_GROUPS);
    mini_free(ini);
}



static void history_update_last_rom(history_t *history, path_t* path)
{
    if(history->last_rom != NULL)
    {    
        path_free(history->last_rom);
    }

    if(path != NULL)
    {
        history->last_rom = path_clone(path);
    }
    else
    {
        history->last_rom = path_create("");
    }
}


static void history_update_last_disk(history_t *history, path_t* path)
{
    if(history->last_disk != NULL)
    {    
        path_free(history->last_disk);
    }

    if(path != NULL)
    {
        history->last_disk = path_clone(path);
    }
    else
    {
        history->last_disk = path_create("");
    }
}



void history_set_last_rom(history_t *history, path_t* path)
{
    history_update_last_rom(history, path);
    history_update_last_disk(history, NULL);
    history_save(history);
}


void history_set_last_disk(history_t *history, path_t* disk_path, path_t* rom_path)
{
    history_update_last_rom(history, rom_path);
    history_update_last_disk(history, disk_path);

    history_save(history);
}

static void history_insert_favorite(history_t *history, int location, path_t* rom, path_t* disk)
{
    if(history->favorites_rom[location])
    {
         path_free(history->favorites_rom[location]);
    }
    if(history->favorites_disk[location])
    {
         path_free(history->favorites_disk[location]);
    }


    history->favorites_rom[location] = rom != NULL ? path_clone(rom) : path_create("");
    history->favorites_disk[location] = rom != NULL ? path_clone(disk) : path_create("");
}


static void history_favorite_replace(history_t *history, int toReplace, path_t* rom, path_t* disk)
{
    if(history->favorites_rom[toReplace])
    {
        path_free(history->favorites_rom[toReplace]);
        history->favorites_rom[toReplace] = NULL;
    }
    if(rom != NULL)
    {
        history->favorites_rom[toReplace] = path_clone(rom);
    }

    if(history->favorites_disk[toReplace])
    {
        path_free(history->favorites_disk[toReplace]);
        history->favorites_disk[toReplace] = NULL;
    }
    if(disk != NULL)
    {
        history->favorites_disk[toReplace] = path_clone(disk);
    }    
}

static void history_rotate_favorites(history_t *history)
{
    for(int i=1;i<FAVORITES_COUNT-1;i++)
    {
        int previous = i-1;

        history_favorite_replace(history, previous, history->favorites_rom[i], history->favorites_disk[i]);
    }

    history_favorite_replace(history, FAVORITES_COUNT -1, NULL, NULL);
}

static bool history_check_match(path_t* left, path_t* right)
{
    bool matches = false;
    if(left == NULL && right == NULL)
    {
        matches = true;
    }
    else
    {
        if(left != NULL && right != NULL)
        {
            matches = strcmp(path_get(left), path_get(right));
        }
    }  

    return matches;
}

static bool history_has_favorite(history_t *history, path_t* rom, path_t* disk)
{
    for(int i=0;i<FAVORITES_COUNT;i++)
    {
        bool romMatch = history_check_match(history->favorites_rom[i], rom);
        bool diskMatch = history_check_match(history->favorites_disk[i], disk);

        if(romMatch && diskMatch)
        {
            return true;
        }        
    }

    return false;
}

void history_add_favorite(history_t *history, path_t* rom, path_t* disk)
{
    // if the game is already in the favorite list then don't add again
    if(history_has_favorite(history, rom, disk))
    {
        return;
    }

    // look for a free space in the list
    int place = -1;
    for(int i=0;i < FAVORITES_COUNT;i++)
    {
        if(!(path_has_value(history->favorites_rom[i]) || path_has_value(history->favorites_disk[i])))
        {
            place = i;
            break;
        }
    }

    // if no free space then rotate the favorite list (first in first out)
    // and set the place to add at the end of the list
    if(place == -1)
    {
        history_rotate_favorites(history);
        place = FAVORITES_COUNT - 1;
    }

    history_insert_favorite(history, place, rom, disk);
    history_save(history);
}