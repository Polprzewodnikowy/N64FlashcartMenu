#include <libdragon.h>
#include <mini.c/src/mini.h>

#include "ed64_state.h"
#include "utils/fs.h"

#ifndef ED64_STATE_FILE_PATH
#define ED64_STATE_FILE_PATH  "sd:/menu/ed64_state.ini"
#endif

static ed64_pseudo_writeback_t init = {
    .is_expecting_save_writeback = false,
    .is_fram_save_type = false,
    .last_save_path = ""
};


void ed64_state_load (ed64_pseudo_writeback_t *state) {
    if (!file_exists(ED64_STATE_FILE_PATH)) {
        ed64_state_save(&init);
    }

    mini_t *ini = mini_try_load(ED64_STATE_FILE_PATH);

    state->is_expecting_save_writeback = mini_get_bool(ini, "ed64", "is_expecting_save_writeback", init.is_expecting_save_writeback);
    state->is_fram_save_type = mini_get_bool(ini, "ed64", "is_fram_save_type", init.is_fram_save_type);
    state->last_save_path = strdup(mini_get_string(ini, "ed64", "last_save_path", init.last_save_path));

    mini_free(ini);
}

void ed64_state_save (ed64_pseudo_writeback_t *state) {
    mini_t *ini = mini_create(ED64_STATE_FILE_PATH);

    mini_set_bool(ini, "ed64", "is_expecting_save_writeback", state->is_expecting_save_writeback);
    mini_set_bool(ini, "ed64", "is_fram_save_type", state->is_fram_save_type);
    mini_set_string(ini, "ed64", "last_save_path", state->last_save_path);

    mini_save(ini, MINI_FLAGS_SKIP_EMPTY_GROUPS);

    mini_free(ini);
}
