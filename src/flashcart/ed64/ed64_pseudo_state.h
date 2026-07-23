/**
 * @file ed64_pseudo_state.h
 * @brief Persistent EverDrive save-writeback state
 * @ingroup flashcart
 */

#ifndef FLASHCART_ED64_PSEUDO_STATE_H__
#define FLASHCART_ED64_PSEUDO_STATE_H__

#include <stdbool.h>

#include "../flashcart.h"

/** @brief EverDrive pseudo save-writeback state (reset-button flow on V-series). */
typedef struct {
    bool is_expecting_save_writeback;
    bool is_fram_save_type;
    flashcart_save_type_t save_type;
    char *last_save_path;
} ed64_pseudo_writeback_t;

void ed64_pseudo_state_init (const char *path);
void ed64_pseudo_state_load (ed64_pseudo_writeback_t *state);
void ed64_pseudo_state_save (ed64_pseudo_writeback_t *state);
void ed64_pseudo_state_free (ed64_pseudo_writeback_t *state);

#endif
