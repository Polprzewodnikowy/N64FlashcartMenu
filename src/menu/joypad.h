/**
 * @file joypad.h
 * @brief Joypad Subsystem
 * @ingroup joypad
 */

#ifndef JOYPAD_H__
#define JOYPAD_H__

/**
 * @brief "Player ID" Joybus command identifier.
 * 
 * Custom command that is not supported by official controllers.
 */
#define JOYBUS_COMMAND_ID_PLAYER_IDENTIFY 0xF0

/**
 * @brief Sends the player ID to each controller.
 *
 * This function sends the player ID to each controller coresponding to the
 * joybus slot the controller is plugged in to.
 *
 * This is a custom command which is not supported by official controllers.
 */
void joypad_send_player_id(void);

#endif // JOYPAD_H__
