/**
 * @file e664_state.h
 * @brief ED64 state
 * @ingroup flashcart
 */

#ifndef FLASHCART_ED64_STATE_H__
#define FLASHCART_ED64_STATE_H__

/** @brief ED64 Pseudo Writeback Structure */
typedef struct {
/** @brief The reset button was used */
    bool is_expecting_save_writeback;
/** @brief The last save type was flash ram */
    bool is_fram_save_type;
/** @brief The path to the last loaded ROM */
    char *last_save_path;
} ed64_pseudo_writeback_t;

void ed64_state_load (ed64_pseudo_writeback_t *state);
void ed64_state_save (ed64_pseudo_writeback_t *state);


#endif