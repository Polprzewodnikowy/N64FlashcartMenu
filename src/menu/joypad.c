/**
 * @file joypad.c
 * @brief Joypad system implementation
 * @ingroup joypad
 */

#include <libdragon.h>

#include "joypad.h"

/**
 * @brief "Player ID" Joybus command structure.
 *
 * This is a custom command not supported by official controllers.
 * 
 * @see #JOYBUS_COMMAND_ID_PLAYER_IDENTIFY
 */
typedef struct __attribute__((packed)) joybus_cmd_player_id_s
{
    /** @brief "Player ID" command send data */
    struct __attribute__((__packed__))
    {
        /** @brief Joybus command ID (#JOYBUS_COMMAND_ID_PLAYER_IDENTIFY) */
        uint8_t command;
        /** @brief Bitmask with a single bit set indicating the port number */
        uint8_t bitmask;
    } send;
    /** @brief Controller responds with the old bitmask */
    uint8_t recv;
} joybus_cmd_player_id_t;

void joypad_send_player_id(void) {
    uint8_t input[JOYBUS_BLOCK_SIZE] = {0};
    joybus_cmd_player_id_t cmd = { .send = {
        .command = JOYBUS_COMMAND_ID_PLAYER_IDENTIFY,
    }};
    const size_t recv_offset = offsetof(typeof(cmd), recv);
    size_t i = 0;

    // Populate the Joybus commands on each port
    JOYPAD_PORT_FOREACH (port)
    {
        cmd.send.bitmask = 1 << port;
        // Set the command metadata
        input[i++] = sizeof(cmd.send);
        input[i++] = sizeof(cmd.recv);
        // Micro-optimization: Minimize copy length
        memcpy(&input[i], &cmd, recv_offset);
        i += sizeof(cmd);
    }

    // Close out the Joybus operation block
    input[i] = 0xFE;
    input[JOYBUS_BLOCK_SIZE - 1] = 0x01;

    // This is a fire-and-forget command where the response can be ignored
    joybus_exec_async(input, NULL, NULL);
}
