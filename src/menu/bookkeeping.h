/**
 * @file rom_history.h
 * @brief Menu History
 * @ingroup menu 
 */

#ifndef HISTORY_H__
#define HISTORY_H__

#include "path.h"


#define FAVORITES_COUNT 5
#define HISTORY_COUNT 5

typedef enum {
    BOOKKEEPING_TYPE_EMPTY,
    BOOKKEEPING_TYPE_ROM,
    BOOKKEEPING_TYPE_DISK,
} bookkeeping_item_types_t;

typedef struct {
    path_t* primary_path;
    path_t* secondary_path;

    bookkeeping_item_types_t bookkeeping_type;

} bookkeeping_item_t;

/** @brief history Structure */
typedef struct {
    bookkeeping_item_t history_items[HISTORY_COUNT];

    bookkeeping_item_t favorite_items[HISTORY_COUNT];
} bookkeeping_t;


/** @brief Init history path */
void bookkeeping_init (char *path);

/** @brief The history to load */
void bookkeeping_load (bookkeeping_t *history);

/** @brief The history to save */
void bookkeeping_save (bookkeeping_t *history);

void bookkeeping_history_add(bookkeeping_t *bookkeeping, path_t* primary_path, path_t* secondary_path, bookkeeping_item_types_t type );

void bookkeeping_favorite_add(bookkeeping_t *bookkeeping, path_t* primary_path, path_t* secondary_path, bookkeeping_item_types_t type );
void bookkeeping_favorite_remove(bookkeeping_t *bookkeeping, int selection);

#endif