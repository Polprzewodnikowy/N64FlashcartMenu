#include <stdlib.h>
#include <string.h>

#include "ed64_pseudo_state.h"
#include "menu/ini_parser.h"

static char *ed64_pseudo_state_path = NULL;

void ed64_pseudo_state_init (const char *path) {
    if (ed64_pseudo_state_path) {
        free(ed64_pseudo_state_path);
    }
    ed64_pseudo_state_path = strdup(path);
}

void ed64_pseudo_state_load (ed64_pseudo_writeback_t *state) {
    ini_t *ini = ini_try_load(ed64_pseudo_state_path);

    state->is_expecting_save_writeback = ini_get_bool(ini, "ed64", "is_expecting_save_writeback", false);
    state->is_fram_save_type = ini_get_bool(ini, "ed64", "is_fram_save_type", false);
    state->save_type = (flashcart_save_type_t) ini_get_int(ini, "ed64", "save_type", FLASHCART_SAVE_TYPE_NONE);

    if (state->last_save_path) {
        free(state->last_save_path);
    }
    state->last_save_path = strdup(ini_get_string(ini, "ed64", "last_save_path", ""));

    ini_free(ini);
}

void ed64_pseudo_state_save (ed64_pseudo_writeback_t *state) {
    ini_t *ini = ini_try_load(ed64_pseudo_state_path);

    ini_set_bool(ini, "ed64", "is_expecting_save_writeback", state->is_expecting_save_writeback);
    ini_set_bool(ini, "ed64", "is_fram_save_type", state->is_fram_save_type);
    ini_set_int(ini, "ed64", "save_type", state->save_type);
    ini_set_string(ini, "ed64", "last_save_path", state->last_save_path ? state->last_save_path : "");

    ini_save(ini, ed64_pseudo_state_path);
    ini_free(ini);
}

void ed64_pseudo_state_free (ed64_pseudo_writeback_t *state) {
    if (state->last_save_path) {
        free(state->last_save_path);
        state->last_save_path = NULL;
    }
}
