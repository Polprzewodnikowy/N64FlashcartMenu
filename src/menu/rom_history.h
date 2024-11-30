/**
 * @file rom_history.h
 * @brief Menu History
 * @ingroup menu 
 */

#ifndef HISTORY_H__
#define HISTORY_H__

#include "path.h"

#define FAVORITES_COUNT 5

/** @brief Settings Structure */
typedef struct {
    path_t* last_rom;
    path_t* last_disk;

    path_t* favorites_rom[FAVORITES_COUNT];    
    path_t* favorites_disk[FAVORITES_COUNT];
} history_t;

/** @brief Init history path */
void history_init (char *path);
/** @brief The history to load */
void history_load (history_t *history);
/** @brief The history to save */
void history_save (history_t *history);

void history_last_rom_set(history_t *history, path_t* disk_path, path_t* rom_path);

void history_favorite_add(history_t *history, path_t* rom, path_t* disk);
void history_favorite_remove(history_t *history, int location);

#endif