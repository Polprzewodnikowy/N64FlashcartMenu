/**
 * @file menu.h
 * @brief Menu Subsystem
 * @ingroup menu
 */

#ifndef MENU_H__
#define MENU_H__

#include "boot/boot.h"

/**
 * @brief Runs the menu subsystem.
 *
 * This function initializes and runs the menu subsystem using the provided
 * boot parameters.
 *
 * @param boot_params A pointer to the boot parameters.
 */
void menu_run(boot_params_t *boot_params);

#endif // MENU_H__
