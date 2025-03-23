/**
 * @file actions.h
 * @brief Menu Actions
 * @ingroup menu 
 */

#ifndef ACTIONS_H__
#define ACTIONS_H__

#include "menu_state.h"

/**
 * @brief Initialize the actions module.
 */
void actions_init (void);

/**
 * @brief Update the actions based on the current menu state.
 * 
 * @param menu Pointer to the menu structure.
 */
void actions_update (menu_t *menu);

#endif /* ACTIONS_H__ */
