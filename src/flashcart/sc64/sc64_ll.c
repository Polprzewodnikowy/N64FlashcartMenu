/**
 * @file sc64_ll.c
 * @brief Low-level functions for SummerCart64
 * @ingroup flashcart
 */

#include <libdragon.h>

#include "../flashcart_utils.h"
#include "sc64_ll.h"

/** @brief SummerCart64 Registers Structure. */
typedef struct {
    uint32_t SR_CMD; /**< Command Status Register. */
    uint32_t DATA[2]; /**< Data Registers. */
    uint32_t IDENTIFIER; /**< Identifier Register. */
    uint32_t KEY; /**< Key Register. */
} sc64_regs_t;

#define SC64_REGS_BASE          (0x1FFF0000UL)
#define SC64_REGS               ((sc64_regs_t *) SC64_REGS_BASE)

#define SC64_SR_CMD_ERROR       (1 << 30)
#define SC64_SR_CPU_BUSY        (1 << 31)

#define SC64_KEY_LOCK           (0xFFFFFFFFUL)

/** @brief SummerCart64 Command IDs. */
typedef enum {
    CMD_ID_VERSION_GET          = 'V',
    CMD_ID_CONFIG_GET           = 'c',
    CMD_ID_CONFIG_SET           = 'C',
    CMD_ID_DISK_MAPPING_SET     = 'D',
    CMD_ID_WRITEBACK_PENDING    = 'w',
    CMD_ID_WRITEBACK_SD_INFO    = 'W',
    CMD_ID_FLASH_WAIT_BUSY      = 'p',
    CMD_ID_FLASH_ERASE_BLOCK    = 'P',
} sc64_cmd_id_t;

/** @brief SummerCart64 Commands Structure. */
typedef struct {
    sc64_cmd_id_t id; /**< Command ID. */
    uint32_t arg[2]; /**< Command arguments. */
    uint32_t rsp[2]; /**< Command response. */
} sc64_cmd_t;

/**
 * @brief Execute a command on the SummerCart64.
 * 
 * @param cmd Pointer to the command structure.
 * @return sc64_error_t Error code.
 */
static sc64_error_t sc64_ll_execute_cmd (sc64_cmd_t *cmd) {
    io_write((uint32_t) (&SC64_REGS->DATA[0]), cmd->arg[0]);
    io_write((uint32_t) (&SC64_REGS->DATA[1]), cmd->arg[1]);

    io_write((uint32_t) (&SC64_REGS->SR_CMD), (cmd->id & 0xFF));

    uint32_t sr;
    do {
        sr = io_read((uint32_t) (&SC64_REGS->SR_CMD));
    } while (sr & SC64_SR_CPU_BUSY);

    if (sr & SC64_SR_CMD_ERROR) {
        return (sc64_error_t) (io_read((uint32_t) (&SC64_REGS->DATA[0])));
    }

    cmd->rsp[0] = io_read((uint32_t) (&SC64_REGS->DATA[0]));
    cmd->rsp[1] = io_read((uint32_t) (&SC64_REGS->DATA[1]));

    return SC64_OK;
}

/**
 * @brief Lock the SummerCart64.
 */
void sc64_ll_lock (void) {
    io_write((uint32_t) (&SC64_REGS->KEY), SC64_KEY_LOCK);
}

/**
 * @brief Get the firmware version of the SummerCart64.
 * 
 * @param major Pointer to store the major version.
 * @param minor Pointer to store the minor version.
 * @param revision Pointer to store the revision number.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_get_version (uint16_t *major, uint16_t *minor, uint32_t *revision) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_VERSION_GET
    };
    sc64_error_t error = sc64_ll_execute_cmd(&cmd);
    *major = ((cmd.rsp[0] >> 16) & 0xFFFF);
    *minor = (cmd.rsp[0] & 0xFFFF);
    *revision = cmd.rsp[1];
    return error;
}

/**
 * @brief Get a configuration value from the SummerCart64.
 * 
 * @param id Configuration ID.
 * @param value Pointer to store the configuration value.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_get_config (sc64_cfg_id_t id, uint32_t *value) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_CONFIG_GET,
        .arg = { id }
    };
    sc64_error_t error = sc64_ll_execute_cmd(&cmd);
    *value = cmd.rsp[1];
    return error;
}

/**
 * @brief Set a configuration value on the SummerCart64.
 * 
 * @param id Configuration ID.
 * @param value Configuration value.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_set_config (sc64_cfg_id_t id, uint32_t value) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_CONFIG_SET,
        .arg = { id, value }
    };
    return sc64_ll_execute_cmd(&cmd);
}

/**
 * @brief Set the disk mapping on the SummerCart64.
 * 
 * @param disk_mapping Pointer to the disk mapping structure.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_set_disk_mapping (sc64_disk_mapping_t *disk_mapping) {
    int disk_count = disk_mapping->count;

    if (disk_count <= 0 || disk_count > 4) {
        return SC64_ERROR_BAD_ARGUMENT;
    }

    uint32_t info[8] __attribute__((aligned(8)));

    for (int i = 0; i < disk_count; i++) {
        info[i * 2] = disk_mapping->disks[i].thb_table;
        info[(i * 2) + 1] = disk_mapping->disks[i].sector_table;
    }

    uint32_t length = (disk_mapping->count * 2 * sizeof(uint32_t));

    pi_dma_write_data(info, SC64_BUFFERS->BUFFER, length);

    sc64_cmd_t cmd = {
        .id = CMD_ID_DISK_MAPPING_SET,
        .arg = { (uint32_t) (SC64_BUFFERS->BUFFER), length }
    };

    return sc64_ll_execute_cmd(&cmd);
}

/**
 * @brief Check if there are pending writebacks on the SummerCart64.
 * 
 * @param pending Pointer to store the pending status.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_writeback_pending (bool *pending) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_WRITEBACK_PENDING
    };
    sc64_error_t error = sc64_ll_execute_cmd(&cmd);
    *pending = (cmd.rsp[0] != 0);
    return error;
}

/**
 * @brief Enable writeback on the SummerCart64.
 * 
 * @param address Address to enable writeback.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_writeback_enable (void *address) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_WRITEBACK_SD_INFO,
        .arg = { (uint32_t) (address) }
    };
    return sc64_ll_execute_cmd(&cmd);
}

/**
 * @brief Wait for the flash to become idle on the SummerCart64.
 * 
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_flash_wait_busy (void) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_FLASH_WAIT_BUSY,
        .arg = { true }
    };
    return sc64_ll_execute_cmd(&cmd);
}

/**
 * @brief Get the flash erase block size on the SummerCart64.
 * 
 * @param erase_block_size Pointer to store the erase block size.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_flash_get_erase_block_size (size_t *erase_block_size) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_FLASH_WAIT_BUSY,
        .arg = { false }
    };
    sc64_error_t error = sc64_ll_execute_cmd(&cmd);
    *erase_block_size = (size_t) (cmd.rsp[0]);
    return error;
}

/**
 * @brief Erase a flash block on the SummerCart64.
 * 
 * @param address Address of the block to erase.
 * @return sc64_error_t Error code.
 */
sc64_error_t sc64_ll_flash_erase_block (void *address) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_FLASH_ERASE_BLOCK,
        .arg = { (uint32_t) (address) }
    };
    return sc64_ll_execute_cmd(&cmd);
}
