#include <libdragon.h>

#include "../flashcart_utils.h"
#include "sc64_ll.h"


/** @brief SummerCart64 Registers Structure. */
typedef struct {
    uint32_t SR_CMD;
    uint32_t DATA[2];
    uint32_t IDENTIFIER;
    uint32_t KEY;
} sc64_regs_t;

#define SC64_REGS_BASE          (0x1FFF0000UL)
#define SC64_REGS               ((sc64_regs_t *) SC64_REGS_BASE)

#define SC64_SR_CMD_ERROR       (1 << 30)
#define SC64_SR_CPU_BUSY        (1 << 31)

#define SC64_KEY_LOCK           (0xFFFFFFFFUL)


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
    sc64_cmd_id_t id;
    uint32_t arg[2];
    uint32_t rsp[2];
} sc64_cmd_t;


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


void sc64_ll_lock (void) {
    io_write((uint32_t) (&SC64_REGS->KEY), SC64_KEY_LOCK);
}

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

sc64_error_t sc64_ll_get_config (sc64_cfg_id_t id, uint32_t *value) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_CONFIG_GET,
        .arg = { id }
    };
    sc64_error_t error = sc64_ll_execute_cmd(&cmd);
    *value = cmd.rsp[1];
    return error;
}

sc64_error_t sc64_ll_set_config (sc64_cfg_id_t id, uint32_t value) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_CONFIG_SET,
        .arg = { id, value }
    };
    return sc64_ll_execute_cmd(&cmd);
}

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

sc64_error_t sc64_ll_writeback_pending (bool *pending) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_WRITEBACK_PENDING
    };
    sc64_error_t error = sc64_ll_execute_cmd(&cmd);
    *pending = (cmd.rsp[0] != 0);
    return error;
}

sc64_error_t sc64_ll_writeback_enable (void *address) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_WRITEBACK_SD_INFO,
        .arg = { (uint32_t) (address) }
    };
    return sc64_ll_execute_cmd(&cmd);
}

sc64_error_t sc64_ll_flash_wait_busy (void) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_FLASH_WAIT_BUSY,
        .arg = { true }
    };
    return sc64_ll_execute_cmd(&cmd);
}

sc64_error_t sc64_ll_flash_get_erase_block_size (size_t *erase_block_size) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_FLASH_WAIT_BUSY,
        .arg = { false }
    };
    sc64_error_t error = sc64_ll_execute_cmd(&cmd);
    *erase_block_size = (size_t) (cmd.rsp[0]);
    return error;
}

sc64_error_t sc64_ll_flash_erase_block (void *address) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_FLASH_ERASE_BLOCK,
        .arg = { (uint32_t) (address) }
    };
    return sc64_ll_execute_cmd(&cmd);
}
