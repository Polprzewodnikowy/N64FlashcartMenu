#include <flashram.h>
#include <libdragon.h>

#include "../flashcart_utils.h"
#include "ed64_bios_ll.h"

/* Shared FPGA command register bank, active on every model once its full-feature
 * personality is loaded (see ed64_vseries_ll_configure_fpga for V series; X series
 * boots directly into this bank). Physical base 0x1F800000. */
#define ED64_BIOS_REG_BASE          (0x1F800000)
#define ED64_BIOS_REG_BOOT_CFG      (ED64_BIOS_REG_BASE + 0x0010)
#define ED64_BIOS_REG_EDID          (ED64_BIOS_REG_BASE + 0x0014)
#define ED64_BIOS_REG_ROM_CFG       (ED64_BIOS_REG_BASE + 0x8018)

#define ED64_BIOS_EDID_SIGNATURE    (0xED640000)
#define ED64_BIOS_BCFG_GAMEMOD      (1 << 3) /**< "return from game", per the official OS's bios.h. */

/* Cart save RAM/EEPROM/FlashRAM window, mapped into the PI SRAM domain (physical 0x08000000). */
#define ED64_BIOS_SAVE_ADDRESS      (0x08000000)

/* PI DOM2 bus timing registers, shared by SRAM and FlashRAM (not reachable via
 * io_read/io_write - see n64 PI controller's own register bank, same as dma.h's PI_* ). */
#define PI_BSD_DOM2_LAT             ((volatile uint32_t *) (0xA4600024))
#define PI_BSD_DOM2_PWD             ((volatile uint32_t *) (0xA4600028))
#define PI_BSD_DOM2_PGS             ((volatile uint32_t *) (0xA460002C))
#define PI_BSD_DOM2_RLS             ((volatile uint32_t *) (0xA4600030))

ed64_cart_id_t ed64_bios_get_cart_id (void) {
    uint32_t id = io_read(ED64_BIOS_REG_EDID);

    if ((id & 0xFFFF0000) != ED64_BIOS_EDID_SIGNATURE) {
        return ED64_CART_ID_UNKNOWN;
    }

    return (ed64_cart_id_t) (id & 0xFFFF);
}

void ed64_bios_set_save_type (ed64_save_type_t save_type) {
    io_write(ED64_BIOS_REG_ROM_CFG, (uint32_t) (save_type));
}

/* flashram_init() reconfigures PI DOM2 for its own command protocol; re-apply libdragon's
 * own sram.c timing explicitly before plain SRAM access rather than trusting leftover state. */
static void ed64_bios_set_sram_pi_timing (void) {
    *PI_BSD_DOM2_LAT = 0x5;
    *PI_BSD_DOM2_PWD = 0xC;
    *PI_BSD_DOM2_PGS = 0xD;
    *PI_BSD_DOM2_RLS = 0x2;
}

void ed64_bios_read_save (ed64_save_type_t save_type, void *buffer, size_t size) {
    if ((save_type == ED64_SAVE_TYPE_EEPROM_4K) || (save_type == ED64_SAVE_TYPE_EEPROM_16K)) {
        uint8_t *dst = (uint8_t *) (buffer);
        for (size_t offset = 0; offset < size; offset += 8) {
            eeprom_read(offset / 8, &dst[offset]);
        }
        return;
    }

    if (save_type == ED64_SAVE_TYPE_FLASHRAM) {
        ed64_bios_set_save_type(ED64_SAVE_TYPE_FLASHRAM);
        if (flashram_init(NULL, NULL)) {
            flashram_read(buffer, 0, size);
        }
        ed64_bios_set_save_type(ED64_SAVE_TYPE_NONE);
        return;
    }

    // SRAM save types all live in the same raw window; read with the widest view then
    // let the caller re-apply the game's actual save type afterward.
    ed64_bios_set_sram_pi_timing();
    ed64_bios_set_save_type(ED64_SAVE_TYPE_SRAM_128K);
    pi_dma_read_data((void *) (ED64_BIOS_SAVE_ADDRESS), buffer, size);
    ed64_bios_set_save_type(ED64_SAVE_TYPE_NONE);
}

void ed64_bios_write_save (ed64_save_type_t save_type, void *buffer, size_t size) {
    if ((save_type == ED64_SAVE_TYPE_EEPROM_4K) || (save_type == ED64_SAVE_TYPE_EEPROM_16K)) {
        uint8_t *src = (uint8_t *) (buffer);
        for (size_t offset = 0; offset < size; offset += 8) {
            eeprom_write(offset / 8, &src[offset]);
        }
        return;
    }

    if (save_type == ED64_SAVE_TYPE_FLASHRAM) {
        ed64_bios_set_save_type(ED64_SAVE_TYPE_FLASHRAM);
        if (flashram_init(NULL, NULL)) {
            flashram_write(buffer, 0, size);
        }
        ed64_bios_set_save_type(ED64_SAVE_TYPE_NONE);
        return;
    }

    ed64_bios_set_sram_pi_timing();
    ed64_bios_set_save_type(ED64_SAVE_TYPE_SRAM_128K);
    pi_dma_write_data(buffer, (void *) (ED64_BIOS_SAVE_ADDRESS), size);
    ed64_bios_set_save_type(ED64_SAVE_TYPE_NONE);
}

bool ed64_bios_save_is_blank (const void *buffer, size_t size) {
    const uint8_t *bytes = (const uint8_t *) (buffer);
    for (size_t i = 0; i < size; i++) {
        if (bytes[i] != 0xFF) {
            return false;
        }
    }
    return true;
}

void ed64_bios_set_game_mode (void) {
    uint32_t boot_cfg = io_read(ED64_BIOS_REG_BOOT_CFG);
    io_write(ED64_BIOS_REG_BOOT_CFG, boot_cfg | ED64_BIOS_BCFG_GAMEMOD);
}

bool ed64_bios_take_game_mode_flag (void) {
    uint32_t boot_cfg = io_read(ED64_BIOS_REG_BOOT_CFG);
    io_write(ED64_BIOS_REG_BOOT_CFG, boot_cfg & ~ED64_BIOS_BCFG_GAMEMOD);
    return (boot_cfg & ED64_BIOS_BCFG_GAMEMOD) != 0;
}
