/**
 * @file settings.h
 * @brief Menu Settings
 * @ingroup menu 
 */

#ifndef FLASHCART_ED64_STATE_H__
#define LASHCART_ED64_STATE_H__

#include <stdint.h>

/** @brief ed64 pseudo Writeback Structure */
typedef struct {
/** @brief The reset button was used */
    bool is_warm_start;
/** @brief The last save type used */
    bool last_rom_save_type; // FIXME: for the moment only a bool for flashram
/** @brief The path to the last loaded ROM */
    char *last_rom_path;
} ed64_pseudo_writeback_t;

void ed64_state_load (ed64_pseudo_writeback_t *state);
void ed64_state_save (ed64_pseudo_writeback_t *state);


#endif