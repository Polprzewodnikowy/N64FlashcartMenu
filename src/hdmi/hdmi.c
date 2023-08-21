// Implementation based on https://gitlab.com/pixelfx-public/n64-game-id

#include <libdragon.h>

#include "hdmi.h"


#define ROM_ADDRESS         (0x10000000)
#define DDIPL_ADDRESS       (0x06000000)
#define CONTROLLER_PORT     (0)
#define GAME_ID_COMMAND     (0x1D)


void hdmi_reset_game_id (void) {
    uint8_t joybus_tx[10];
    uint8_t joybus_rx[1];

    memset(joybus_tx, 0, sizeof(joybus_tx));

    execute_raw_command(
        CONTROLLER_PORT,
        GAME_ID_COMMAND,
        sizeof(joybus_tx),
        sizeof(joybus_rx),
        joybus_tx,
        joybus_rx
    );
}

void hdmi_broadcast_game_id (boot_params_t *boot_params) {
    uint8_t rom_header[0x40] __attribute__((aligned(8)));
    uint32_t pi_address = ROM_ADDRESS;

    if (boot_params->device_type == BOOT_DEVICE_TYPE_DD) {
        pi_address = DDIPL_ADDRESS;
    }

    dma_read_async(rom_header, pi_address, sizeof(rom_header));
    dma_wait();

    uint8_t joybus_tx[10];
    uint8_t joybus_rx[1];

    // Copy CRC
    memcpy(joybus_tx, rom_header + 0x10, 8);

    // Copy media format
    joybus_tx[8] = rom_header[0x3B];

    // Copy country code
    joybus_tx[9] = rom_header[0x3E];

    execute_raw_command(
        CONTROLLER_PORT,
        GAME_ID_COMMAND,
        sizeof(joybus_tx),
        sizeof(joybus_rx),
        joybus_tx,
        joybus_rx
    );
}
