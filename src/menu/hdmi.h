/**
 * @file hdmi.h
 * @brief Header file for HDMI-related functions.
 * @ingroup menu
 */

#ifndef HDMI_H__
#define HDMI_H__

#include "boot/boot.h"

/**
 * @brief Clears the game ID from the HDMI interface.
 *
 * This function clears the current game ID from the HDMI interface.
 */
void hdmi_clear_game_id(void);

/**
 * @brief Sends the game ID to the HDMI interface.
 *
 * This function sends the provided game ID to the HDMI interface using the
 * specified boot parameters.
 *
 * @param boot_params A pointer to the boot parameters containing the game ID.
 */
void hdmi_send_game_id(boot_params_t *boot_params);

#endif // HDMI_H__
