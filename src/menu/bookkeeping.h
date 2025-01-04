/**
 * @file bookkeeping.h
 * @brief Bookkeeping of loadded ROM's.
 * @ingroup menu 
 */

#ifndef BOOKKEEPING_H__
#define BOOKKEEPING_H__

#include "path.h"


#define FAVORITES_COUNT 8
#define HISTORY_COUNT 8

typedef enum {
    BOOKKEEPING_TYPE_EMPTY,
    BOOKKEEPING_TYPE_ROM,
    BOOKKEEPING_TYPE_DISK,
} bookkeeping_item_types_t;

typedef struct {
    path_t *primary_path;
    path_t *secondary_path;

    bookkeeping_item_types_t bookkeeping_type;

} bookkeeping_item_t;

/** @brief ROM bookkeeping Structure */
typedef struct {
    bookkeeping_item_t history_items[HISTORY_COUNT];

    bookkeeping_item_t favorite_items[HISTORY_COUNT];
} bookkeeping_t;


/** @brief Init ROM bookkeeping path */
void bookkeeping_init (char *path);

/** @brief The ROM bookkeeping to load */
void bookkeeping_load (bookkeeping_t *history);

/** @brief The ROM bookkeeping to save */
void bookkeeping_save (bookkeeping_t *history);

/**
 * @brief Add a ROM to the history.
 * 
 * @param bookkeeping The bookkeeping structure.
 * @param primary_path The primary path of the ROM.
 * @param secondary_path The secondary path of the ROM.
 * @param type The type of the bookkeeping item.
 */
void bookkeeping_history_add(bookkeeping_t *bookkeeping, path_t *primary_path, path_t *secondary_path, bookkeeping_item_types_t type );

/**
 * @brief Add a ROM to the favorites.
 * 
 * @param bookkeeping The bookkeeping structure.
 * @param primary_path The primary path of the ROM.
 * @param secondary_path The secondary path of the ROM.
 * @param type The type of the bookkeeping item.
 */
void bookkeeping_favorite_add(bookkeeping_t *bookkeeping, path_t *primary_path, path_t *secondary_path, bookkeeping_item_types_t type );

/**
 * @brief Remove a ROM from the favorites.
 * 
 * @param bookkeeping The bookkeeping structure.
 * @param selection The index of the favorite item to remove.
 */
void bookkeeping_favorite_remove(bookkeeping_t *bookkeeping, int selection);

#endif
