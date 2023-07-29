#include <libdragon.h>

#include "64drive_ll.h"


#define CI_STATUS_BUSY              (1 << 12)

#define DEVICE_VARIANT_MASK         (0xFFFF)
#define FPGA_REVISION_MASK          (0xFFFF)


typedef enum {
    CMD_ID_SET_SAVE_TYPE            = 0xD0,
    CMD_ID_DISABLE_SAVE_WRITEBACK   = 0xD1,
    CMD_ID_ENABLE_SAVE_WRITEBACK    = 0xD2,
    CMD_ID_ENABLE_CARTROM_WRITES    = 0xF0,
    CMD_ID_DISABLE_CARTROM_WRITES   = 0xF1,
} d64_ci_cmd_id_t;


static bool d64_ll_ci_cmd (d64_ci_cmd_id_t id) {
    while (io_read((uint32_t) (&D64_REGS->STATUS)) & CI_STATUS_BUSY);

    io_write((uint32_t) (&D64_REGS->COMMAND), id);

    while (io_read((uint32_t) (&D64_REGS->STATUS)) & CI_STATUS_BUSY);

    return false;
}


void d64_ll_get_version (uint16_t *device_variant, uint16_t *fpga_revision, uint32_t *bootloader_version) {
    *device_variant = (io_read((uint32_t) (&D64_REGS->VARIANT)) & DEVICE_VARIANT_MASK);
    *fpga_revision = (io_read((uint32_t) (&D64_REGS->REVISION)) & FPGA_REVISION_MASK);
    *bootloader_version = io_read((uint32_t) (&D64_REGS->PERSISTENT));
}

bool d64_ll_set_save_type (d64_save_type_t save_type) {
    io_write((uint32_t) (&D64_REGS->BUFFER), save_type);
    return d64_ll_ci_cmd(CMD_ID_SET_SAVE_TYPE);
}

bool d64_ll_enable_save_writeback (bool enabled) {
    return d64_ll_ci_cmd(enabled ? CMD_ID_ENABLE_SAVE_WRITEBACK : CMD_ID_DISABLE_SAVE_WRITEBACK);
}

bool d64_ll_enable_cartrom_writes (bool enabled) {
    return d64_ll_ci_cmd(enabled ? CMD_ID_ENABLE_CARTROM_WRITES : CMD_ID_DISABLE_CARTROM_WRITES);
}
