#include <libdragon.h>

#include "sc64_internal.h"


typedef struct {
    uint32_t SR_CMD;
    uint32_t DATA[2];
    uint32_t IDENTIFIER;
    uint32_t KEY;
} sc64_regs_t;

#define SC64_REGS_BASE          (0x1FFF0000UL)
#define SC64_REGS               ((sc64_regs_t *) SC64_REGS_BASE)

#define SC64_SR_IRQ_PENDING     (1 << 29)
#define SC64_SR_CMD_ERROR       (1 << 30)
#define SC64_SR_CPU_BUSY        (1 << 31)

#define SC64_V2_IDENTIFIER      (0x53437632)

#define SC64_KEY_RESET          (0x00000000UL)
#define SC64_KEY_UNLOCK_1       (0x5F554E4CUL)
#define SC64_KEY_UNLOCK_2       (0x4F434B5FUL)
#define SC64_KEY_LOCK           (0xFFFFFFFFUL)


typedef enum {
    CMD_ID_VERSION_GET          = 'V',
    CMD_ID_CONFIG_GET           = 'c',
    CMD_ID_CONFIG_SET           = 'C',
    CMD_ID_SD_CARD_OP           = 'i',
    CMD_ID_WRITEBACK_PENDING    = 'w',
    CMD_ID_WRITEBACK_SD_INFO    = 'W',
    CMD_ID_FLASH_PROGRAM        = 'K',
    CMD_ID_FLASH_WAIT_BUSY      = 'p',
    CMD_ID_FLASH_ERASE_BLOCK    = 'P',
} sc64_cmd_id_t;

typedef enum {
    SD_CARD_OP_DEINIT           = 0,
    SD_CARD_OP_INIT             = 1,
    SD_CARD_OP_GET_STATUS       = 2,
    SD_CARD_OP_GET_INFO         = 3,
    SD_CARD_OP_BYTE_SWAP_ON     = 4,
    SD_CARD_OP_BYTE_SWAP_OFF    = 5,
} sc64_sd_card_op_t;

typedef struct {
    sc64_cmd_id_t id;
    uint32_t arg[2];
    uint32_t rsp[2];
} sc64_cmd_t;


static sc64_error_t sc64_execute_cmd (sc64_cmd_t *cmd) {
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


void sc64_unlock (void) {
    io_write((uint32_t) (&SC64_REGS->KEY), SC64_KEY_RESET);
    io_write((uint32_t) (&SC64_REGS->KEY), SC64_KEY_UNLOCK_1);
    io_write((uint32_t) (&SC64_REGS->KEY), SC64_KEY_UNLOCK_2);
}

void sc64_lock (void) {
    io_write((uint32_t) (&SC64_REGS->KEY), SC64_KEY_LOCK);
}

bool sc64_check_presence (void) {
    return (io_read((uint32_t) (&SC64_REGS->IDENTIFIER)) == SC64_V2_IDENTIFIER);
}

void sc64_read_data (void *src, void *dst, size_t length) {
    data_cache_hit_writeback_invalidate(dst, length);
    dma_read_raw_async(dst, (uint32_t) (src), length);
    dma_wait();
}

void sc64_write_data (void *src, void *dst, size_t length) {
    data_cache_hit_writeback(src, length);
    dma_write_raw_async(src, (uint32_t) (dst), length);
    dma_wait();
}

sc64_error_t sc64_get_version (uint16_t *major, uint16_t *minor) {
    sc64_cmd_t cmd = { .id = CMD_ID_VERSION_GET };
    sc64_error_t error = sc64_execute_cmd(&cmd);
    *major = ((cmd.rsp[0] >> 16) & 0xFFFF);
    *minor = (cmd.rsp[0] & 0xFFFF);
    return error;
}

sc64_error_t sc64_get_config (sc64_cfg_t id, void *value) {
    sc64_cmd_t cmd = { .id = CMD_ID_CONFIG_GET, .arg = { id } };
    sc64_error_t error = sc64_execute_cmd(&cmd);
    *((uint32_t *) (value)) = cmd.rsp[1];
    return error;
}

sc64_error_t sc64_set_config (sc64_cfg_t id, uint32_t value) {
    sc64_cmd_t cmd = { .id = CMD_ID_CONFIG_SET, .arg = { id, value } };
    return sc64_execute_cmd(&cmd);
}

sc64_error_t sc64_sd_set_byte_swap (bool enabled) {
    sc64_cmd_t cmd = {
        .id = CMD_ID_SD_CARD_OP,
        .arg = { 0, enabled ? SD_CARD_OP_BYTE_SWAP_ON : SD_CARD_OP_BYTE_SWAP_OFF }
    };
    return sc64_execute_cmd(&cmd);
}

sc64_error_t sc64_writeback_pending (bool *pending) {
    sc64_cmd_t cmd = { .id = CMD_ID_WRITEBACK_PENDING };
    sc64_error_t error = sc64_execute_cmd(&cmd);
    *pending = (cmd.rsp[0] != 0);
    return error;
}

sc64_error_t sc64_writeback_enable (void *address) {
    sc64_cmd_t cmd = { .id = CMD_ID_WRITEBACK_SD_INFO, .arg = { (uint32_t) (address) } };
    return sc64_execute_cmd(&cmd);
}

sc64_error_t sc64_flash_program (void *address, size_t length) {
    sc64_cmd_t cmd = { .id = CMD_ID_FLASH_PROGRAM, .arg = { (uint32_t) (address), length } };
    return sc64_execute_cmd(&cmd);
}

sc64_error_t sc64_flash_wait_busy (void) {
    sc64_cmd_t cmd = { .id = CMD_ID_FLASH_WAIT_BUSY, .arg = { true } };
    return sc64_execute_cmd(&cmd);
}

sc64_error_t sc64_flash_get_erase_block_size (size_t *erase_block_size) {
    sc64_cmd_t cmd = { .id = CMD_ID_FLASH_WAIT_BUSY, .arg = { false } };
    sc64_error_t error = sc64_execute_cmd(&cmd);
    *erase_block_size = (size_t) (cmd.rsp[0]);
    return error;
}

sc64_error_t sc64_flash_erase_block (void *address) {
    sc64_cmd_t cmd = { .id = CMD_ID_FLASH_ERASE_BLOCK, .arg = { (uint32_t) (address) } };
    return sc64_execute_cmd(&cmd);
}
