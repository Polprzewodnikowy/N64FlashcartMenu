#include <libdragon.h>

#include "hdmi.h"


#define ROM_ADDRESS         (0x10000000)
#define DDIPL_ADDRESS       (0x06000000)


void hdmi_clear_game_id (void) {
    pixelfx_clear_game_id();
}

void hdmi_send_game_id (boot_params_t *boot_params) {
    uint8_t rom_header[0x40] __attribute__((aligned(8)));
    uint32_t pi_address = ROM_ADDRESS;

    if (boot_params->device_type == BOOT_DEVICE_TYPE_64DD) {
        pi_address = DDIPL_ADDRESS;
    }

    dma_read_async(rom_header, pi_address, sizeof(rom_header));
    dma_wait();

    uint64_t rom_check_code;
    uint8_t media_format;
    uint8_t region_code;

    memcpy((uint8_t *) (&rom_check_code), rom_header + 0x10, sizeof(rom_check_code));
    media_format = rom_header[0x3B];
    region_code = rom_header[0x3E];

    pixelfx_send_game_id(rom_check_code, media_format, region_code);
}
