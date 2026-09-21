#include <libdragon.h>
#include <mini.c/src/mini.h>

#include "ed64_state.h"
#include "utils/fs.h"

static char *ed64_state_path = NULL;

static ed64_pseudo_writeback_t init = {
    .is_expecting_save_writeback = false,
    .is_fram_save_type = false,
    .last_save_path = ""
};

void ed64_state_init (char *path) {
    if (ed64_state_path) {
        free(ed64_state_path);
    }
    ed64_state_path = strdup(path);
}

void ed64_state_load (ed64_pseudo_writeback_t *state) {
    if (!file_exists(ed64_state_path)) {
        ed64_state_save(&init);
    }

    mini_t *ini = mini_try_load(ed64_state_path);

    state->is_expecting_save_writeback = mini_get_bool(ini, "ed64", "is_expecting_save_writeback", init.is_expecting_save_writeback);
    state->is_fram_save_type = mini_get_bool(ini, "ed64", "is_fram_save_type", init.is_fram_save_type);
    state->last_save_path = strdup(mini_get_string(ini, "ed64", "last_save_path", init.last_save_path));

    mini_free(ini);
}

void ed64_state_save (ed64_pseudo_writeback_t *state) {
    mini_t *ini = mini_create(ed64_state_path);

    mini_set_bool(ini, "ed64", "is_expecting_save_writeback", state->is_expecting_save_writeback);
    mini_set_bool(ini, "ed64", "is_fram_save_type", state->is_fram_save_type);
    mini_set_string(ini, "ed64", "last_save_path", state->last_save_path);

    mini_save(ini, MINI_FLAGS_SKIP_EMPTY_GROUPS);

    mini_free(ini);
}
