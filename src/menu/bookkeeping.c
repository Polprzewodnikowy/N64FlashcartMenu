#include <libdragon.h>
#include <mini.c/src/mini.h>

#include "bookkeeping.h"
#include "utils/fs.h"
#include "path.h"

static char *history_path = NULL;

static path_t *empty_path = NULL;
static bookkeeping_t init;

/** @brief Init history path */
void bookkeeping_init (char *path) {
    if (history_path) {
        free(history_path);
    }
    history_path = strdup(path);
    empty_path = path_create("");
}


void bookkeeping_ini_load_list(bookkeeping_item_t *list, int count, mini_t *ini, const char *group)
{
    char buf[64];
    for(int i=0; i<count; i++) {
        sprintf(buf,"%d_primary_path", i);
        list[i].primary_path = path_create(mini_get_string(ini, group, buf, ""));

        sprintf(buf,"%d_secondary_path", i);
        list[i].secondary_path = path_create(mini_get_string(ini, group, buf, ""));
        
        sprintf(buf,"%d_type", i);
        list[i].bookkeeping_type = mini_get_int(ini, group, buf, BOOKKEEPING_TYPE_EMPTY);
    }
}

/** @brief The history to load */
void bookkeeping_load (bookkeeping_t *history) {
    if (!file_exists(history_path)) {
        bookkeeping_save(&init);
    }

    mini_t *bookkeeping_ini = mini_try_load(history_path);
    bookkeeping_ini_load_list(history->history_items, HISTORY_COUNT, bookkeeping_ini, "history");
    bookkeeping_ini_load_list(history->favorite_items, HISTORY_COUNT, bookkeeping_ini, "favorite");


    mini_free(bookkeeping_ini);
}

static void bookkeeping_ini_save_list(bookkeeping_item_t *list, int count, mini_t *ini, const char *group)
{
    char buf[64];
    for(int i=0; i<count; i++) {
        sprintf(buf, "%d_primary_path", i);
        path_t* path = list[i].primary_path;
        mini_set_string(ini, group, buf, path != NULL ? path_get(path) : "");   

        sprintf(buf, "%d_secondary_path", i);
        path = list[i].secondary_path;
        mini_set_string(ini, group, buf, path != NULL ? path_get(path) : "");   

        sprintf(buf,"%d_type", i);
        mini_set_int(ini, group, buf, list[i].bookkeeping_type);           
    }
}

/** @brief The history to save */
void bookkeeping_save (bookkeeping_t *history)
{
    mini_t *bookkeeping_ini = mini_create(history_path);

    bookkeeping_ini_save_list(history->history_items, HISTORY_COUNT, bookkeeping_ini, "history");
    bookkeeping_ini_save_list(history->favorite_items, FAVORITES_COUNT, bookkeeping_ini, "favorite");

    mini_save(bookkeeping_ini, MINI_FLAGS_SKIP_EMPTY_GROUPS);
    mini_free(bookkeeping_ini);    
}

static bool bookkeeping_item_match(bookkeeping_item_t *left, bookkeeping_item_t *right) {
    if(left != NULL && right != NULL) {
        return path_are_match(left->primary_path, right->primary_path) && path_are_match(left->secondary_path, right->secondary_path) && left->bookkeeping_type == right->bookkeeping_type;
    }

    return false;
}

static void bookkeeping_clear_item(bookkeeping_item_t *item, bool leave_null) {
    if(item->primary_path != NULL){
        path_free(item->primary_path);

        if(leave_null) {
            item->primary_path = NULL;
        } else {
            item->primary_path = path_create("");
        }
    }
    if(item->secondary_path != NULL){
        path_free(item->secondary_path);

        if(leave_null) {
            item->secondary_path = NULL;
        } else {
            item->secondary_path = path_create("");
        }
    }
    item->bookkeeping_type = BOOKKEEPING_TYPE_EMPTY;
}

static void bookkeeping_copy_item(bookkeeping_item_t *source, bookkeeping_item_t *destination) {
    bookkeeping_clear_item(destination, true);

    destination->primary_path = path_clone(source->primary_path);    
    destination->secondary_path = source->secondary_path != NULL ? path_clone(source->secondary_path) : path_create("");
    destination->bookkeeping_type = source->bookkeeping_type;
}

static void bookkeeping_move_items_down(bookkeeping_item_t *list, int start, int end) {
    int current = end;

    do {
        if(current <= start || current < 0) {
            break;
        }        

        bookkeeping_copy_item(&list[current - 1], &list[current]);
        current--;
    } while(true);
}


static void bookkeeping_move_items_up(bookkeeping_item_t *list, int start, int end) {
    int current = start;

    do {
        if(current > end) {
            break;
        }        

        bookkeeping_copy_item(&list[current + 1], &list[current]);
        current++;
    } while(true);
}


static void bookkeeping_insert_top(bookkeeping_item_t *list, int count, bookkeeping_item_t *new_item) {
    // if it matches the top of the list already then nothing to do
    if(bookkeeping_item_match(&list[0], new_item)) {
        return;
    }

    // if the top isn't empty then we need to move things around
    if(list[0].bookkeeping_type != BOOKKEEPING_TYPE_EMPTY) {
        int found_at = -1;
        for(int i=1; i < count; i++) {
            if(bookkeeping_item_match(&list[i], new_item)){
                found_at = i;
                break;
            }
        }

        if(found_at == -1) {
            bookkeeping_move_items_down(list, 0, count - 1);
        } else {
            bookkeeping_move_items_down(list, 0, found_at);
        }
    }
    
    bookkeeping_copy_item(new_item, &list[0]);
}

void bookkeeping_history_add(bookkeeping_t *bookkeeping, path_t *primary_path, path_t *secondary_path, bookkeeping_item_types_t type ) {
    bookkeeping_item_t new_item = {
        .primary_path = primary_path,
        .secondary_path = secondary_path,
        .bookkeeping_type = type
    };

    bookkeeping_insert_top(bookkeeping->history_items, HISTORY_COUNT,  &new_item);
    bookkeeping_save(bookkeeping);
}


void bookkeeping_favorite_add(bookkeeping_t *bookkeeping, path_t *primary_path, path_t *secondary_path, bookkeeping_item_types_t type ) {
    bookkeeping_item_t new_item = {
        .primary_path = primary_path,
        .secondary_path = secondary_path,
        .bookkeeping_type = type
    };

    bookkeeping_insert_top(bookkeeping->favorite_items, FAVORITES_COUNT,  &new_item);
    bookkeeping_save(bookkeeping);
}

void bookkeeping_favorite_remove(bookkeeping_t *bookkeeping, int selection) {
    if(bookkeeping->favorite_items[selection].bookkeeping_type != BOOKKEEPING_TYPE_EMPTY) {

        bookkeeping_move_items_up(bookkeeping->favorite_items, selection, FAVORITES_COUNT -1);
        bookkeeping_clear_item(&bookkeeping->favorite_items[FAVORITES_COUNT -1], false);

        bookkeeping_save(bookkeeping);
    }
}