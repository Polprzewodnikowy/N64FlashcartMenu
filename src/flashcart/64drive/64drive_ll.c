/**
 * @file 64drive_ll.c
 * @brief Low-level functions for 64drive
 * @ingroup flashcart
 */

#include <libdragon.h>

#include "../flashcart_utils.h"
#include "64drive_ll.h"

#define CI_STATUS_BUSY              (1 << 12)

#define D64_DEVICE_VARIANT_MASK     (0xFFFF)
#define D64_FPGA_REVISION_MASK      (0xFFFF)

/**
 * @brief Command IDs for 64drive.
 */
typedef enum {
    CMD_ID_SET_SAVE_TYPE            = 0xD0,
    CMD_ID_DISABLE_SAVE_WRITEBACK   = 0xD1,
    CMD_ID_ENABLE_SAVE_WRITEBACK    = 0xD2,
    CMD_ID_DISABLE_BYTESWAP_ON_LOAD = 0xE0,
    CMD_ID_ENABLE_BYTESWAP_ON_LOAD  = 0xE1,
    CMD_ID_ENABLE_CARTROM_WRITES    = 0xF0,
    CMD_ID_DISABLE_CARTROM_WRITES   = 0xF1,
    CMD_ID_ENABLE_EXTENDED_MODE     = 0xF8,
    CMD_ID_DISABLE_EXTENDED_MODE    = 0xF9,
} d64_ci_cmd_id_t;

static d64_regs_t *d64_regs = D64_REGS;

/**
 * @brief Wait for the CI (Command Interface) to become idle.
 * 
 * @return true if a timeout occurred, false otherwise.
 */
static bool d64_ll_ci_wait (void) {
    int timeout = 0;
    do {
        if (timeout++ >= 0x10000) {
            return true;
        }
    } while (io_read((uint32_t) (&d64_regs->STATUS)) & CI_STATUS_BUSY);
    return false;
}

/**
 * @brief Send a command to the CI (Command Interface).
 * 
 * @param id The command ID.
 * @return true if a timeout occurred, false otherwise.
 */
static bool d64_ll_ci_cmd (d64_ci_cmd_id_t id) {
    io_write((uint32_t) (&d64_regs->COMMAND), id);
    return d64_ll_ci_wait();
}

/**
 * @brief Get the version information of the 64drive.
 * 
 * @param device_variant Pointer to store the device variant.
 * @param fpga_revision Pointer to store the FPGA revision.
 * @param bootloader_version Pointer to store the bootloader version.
 * @return true if a timeout occurred, false otherwise.
 */
bool d64_ll_get_version (d64_device_variant_t *device_variant, uint16_t *fpga_revision, uint32_t *bootloader_version) {
    if (d64_ll_ci_wait()) {
        return true;
    }
    *device_variant = (d64_device_variant_t) (io_read((uint32_t) (&d64_regs->VARIANT)) & D64_DEVICE_VARIANT_MASK);
    *fpga_revision = (io_read((uint32_t) (&d64_regs->REVISION)) & D64_FPGA_REVISION_MASK);
    *bootloader_version = io_read((uint32_t) (&d64_regs->PERSISTENT));
    return d64_ll_ci_wait();
}

/**
 * @brief Set the persistent variable storage on the 64drive.
 * 
 * @param quick_reboot Flag indicating whether to enable quick reboot.
 * @param force_tv_type The TV type to force.
 * @param cic_seed The CIC seed value.
 * @return true if a timeout occurred, false otherwise.
 */
bool d64_ll_set_persistent_variable_storage (bool quick_reboot, d64_tv_type_t force_tv_type, uint8_t cic_seed) {
    if (d64_ll_ci_wait()) {
        return true;
    }
    io_write((uint32_t) (&d64_regs->PERSISTENT), (quick_reboot << 16) | ((force_tv_type & 0x03) << 8) | (cic_seed & 0xFF));
    return d64_ll_ci_wait();
}

/**
 * @brief Set the save type on the 64drive.
 * 
 * @param save_type The save type.
 * @return true if a timeout occurred, false otherwise.
 */
bool d64_ll_set_save_type (d64_save_type_t save_type) {
    if (d64_ll_ci_wait()) {
        return true;
    }
    io_write((uint32_t) (&d64_regs->BUFFER), save_type);
    return d64_ll_ci_cmd(CMD_ID_SET_SAVE_TYPE);
}

/**
 * @brief Enable or disable save writeback on the 64drive.
 * 
 * @param enabled Flag indicating whether to enable save writeback.
 * @return true if a timeout occurred, false otherwise.
 */
bool d64_ll_enable_save_writeback (bool enabled) {
    if (d64_ll_ci_wait()) {
        return true;
    }
    return d64_ll_ci_cmd(enabled ? CMD_ID_ENABLE_SAVE_WRITEBACK : CMD_ID_DISABLE_SAVE_WRITEBACK);
}

/**
 * @brief Enable or disable cart ROM writes on the 64drive.
 * 
 * @param enabled Flag indicating whether to enable cart ROM writes.
 * @return true if a timeout occurred, false otherwise.
 */
bool d64_ll_enable_cartrom_writes (bool enabled) {
    if (d64_ll_ci_wait()) {
        return true;
    }
    return d64_ll_ci_cmd(enabled ? CMD_ID_ENABLE_CARTROM_WRITES : CMD_ID_DISABLE_CARTROM_WRITES);
}

/**
 * @brief Enable or disable extended mode on the 64drive.
 * 
 * @param enabled Flag indicating whether to enable extended mode.
 * @return true if a timeout occurred, false otherwise.
 */
bool d64_ll_enable_extended_mode (bool enabled) {
    d64_ll_ci_wait();
    if (enabled) {
        io_write((uint32_t) (&D64_REGS->COMMAND), CMD_ID_ENABLE_EXTENDED_MODE);
    } else {
        io_write((uint32_t) (&D64_REGS_EXT->COMMAND), CMD_ID_DISABLE_EXTENDED_MODE);
    }
    d64_regs = enabled ? D64_REGS_EXT : D64_REGS;
    return d64_ll_ci_wait();
}

/**
 * @brief Write EEPROM contents to the 64drive.
 * 
 * @param contents Pointer to the EEPROM contents.
 * @return true if a timeout occurred, false otherwise.
 */
bool d64_ll_write_eeprom_contents (void *contents) {
    if (d64_ll_ci_wait()) {
        return true;
    }
    pi_dma_write_data(contents, d64_regs->EEPROM, 2048);
    return d64_ll_ci_wait();
}

/**
 * @brief Write the save writeback LBA list to the 64drive.
 * 
 * @param list Pointer to the LBA list.
 * @return true if a timeout occurred, false otherwise.
 */
bool d64_ll_write_save_writeback_lba_list (void *list) {
    if (d64_ll_ci_wait()) {
        return true;
    }
    pi_dma_write_data(list, d64_regs->WRITEBACK, 1024);
    return d64_ll_ci_wait();
}
