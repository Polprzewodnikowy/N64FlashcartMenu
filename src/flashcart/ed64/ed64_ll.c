#include <libcart/cart.h>
#include <libdragon.h>

#include "utils/utils.h"
#include "ed64_ll.h"

#define ED64_CONFIG_REGS_BASE   0xA8040000UL

#define REG_SAV_CFG             9

#define SAV_EEP_ON              1
#define SAV_SRM_ON              2
#define SAV_EEP_SIZE            4
#define SAV_SRM_SIZE            8
#define SAV_RAM_BANK            128
#define SAV_RAM_BANK_APPLY      32768

#define PI_BASE_REG             0x04600000UL
#define PI_STATUS_REG           (PI_BASE_REG + 0x10)
#define PI_DRAM_ADDR_REG        (PI_BASE_REG + 0x00)
#define PI_CART_ADDR_REG        (PI_BASE_REG + 0x04)
#define PI_RD_LEN_REG           (PI_BASE_REG + 0x08)
#define PI_WR_LEN_REG           (PI_BASE_REG + 0x0C)
#define PI_BSD_DOM1_LAT_REG     (PI_BASE_REG + 0x14)
#define PI_BSD_DOM1_PWD_REG     (PI_BASE_REG + 0x18)
#define PI_BSD_DOM1_PGS_REG     (PI_BASE_REG + 0x1C)
#define PI_BSD_DOM1_RLS_REG     (PI_BASE_REG + 0x20)
#define PI_BSD_DOM2_LAT_REG     (PI_BASE_REG + 0x24)
#define PI_BSD_DOM2_PWD_REG     (PI_BASE_REG + 0x28)
#define PI_BSD_DOM2_PGS_REG     (PI_BASE_REG + 0x2C)
#define PI_BSD_DOM2_RLS_REG     (PI_BASE_REG + 0x30)

#define K1_TO_PHYS(x)           ((unsigned long) (x) & 0x1FFFFFFFUL)

/* 128K SRAM saves on V-series live 64 KiB below the primary save window. */
#define SRAM_128K_OFFSET        (-(int) KiB(64))

#define ED_BASE_REG             0x08040000UL
#define ED_KEY_REG              (ED_BASE_REG + 0x20)
#define ED_VER_REG              (ED_BASE_REG + 0x2C)
#define ED_KEY                  0x1234

static uint8_t ed64_ll_sram_bank;
static ed64_save_type_t ed64_ll_save_type;
static uint16_t ed64_ll_hw_version;
static ed64_hw_model_t ed64_ll_hw_model;

static ed64_hw_model_t ed64_ll_model_from_version (uint16_t ver) {
    if (ver < 0x100 || ver >= 0x400) {
        return ED64_HW_UNKNOWN;
    }
    if (ver >= 0x330) {
        return ED64_HW_V3;
    }
    if (ver >= 0x325) {
        return ED64_HW_V2_5;
    }
    if (ver >= 0x320) {
        return ED64_HW_V2;
    }
    return ED64_HW_V1;
}

void ed64_ll_detect_hardware (void) {
    io_write(ED_KEY_REG, ED_KEY);
    ed64_ll_hw_version = (uint16_t) (io_read(ED_VER_REG) & 0xFFFF);
    ed64_ll_hw_model = ed64_ll_model_from_version(ed64_ll_hw_version);
}

uint16_t ed64_ll_get_hw_version (void) {
    return ed64_ll_hw_version;
}

ed64_hw_model_t ed64_ll_get_hw_model (void) {
    return ed64_ll_hw_model;
}

bool ed64_ll_has_battery_backed_save (void) {
    return ed64_ll_hw_version >= ED64_HW_BATTERY_SAVE_THRESHOLD;
}

bool ed64_ll_needs_reset_for_save (void) {
    return !ed64_ll_has_battery_backed_save();
}

static void ed64_ll_reg_write (uint32_t reg, uint32_t data) {
    *(volatile uint32_t *) ED64_CONFIG_REGS_BASE;
    *(volatile uint32_t *) (ED64_CONFIG_REGS_BASE + reg * 4) = data;
    *(volatile uint32_t *) ROM_ADDRESS;
}

static void pi_dma_wait (void) {
    while (io_read(PI_STATUS_REG) & (PI_STATUS_IO_BUSY | PI_STATUS_DMA_BUSY)) {
    }
}

static void pi_initialize (void) {
    pi_dma_wait();
    io_write(PI_STATUS_REG, 0x03);
}

static void pi_initialize_sram (void) {
    io_write(PI_BSD_DOM2_LAT_REG, 0x05);
    io_write(PI_BSD_DOM2_PWD_REG, 0x0C);
    io_write(PI_BSD_DOM2_PGS_REG, 0x0D);
    io_write(PI_BSD_DOM2_RLS_REG, 0x02);
}

static void pi_dma_from_sram (void *dest, int offset, unsigned long size) {
    io_write(PI_DRAM_ADDR_REG, K1_TO_PHYS(dest));
    io_write(PI_CART_ADDR_REG, PI_SAVE_ADDR + offset);
    io_write(PI_WR_LEN_REG, size - 1);
}

static void pi_dma_to_sram (void *src, int offset, unsigned long size) {
    pi_dma_wait();
    io_write(PI_STATUS_REG, 2);
    io_write(PI_DRAM_ADDR_REG, K1_TO_PHYS(src));
    io_write(PI_CART_ADDR_REG, PI_SAVE_ADDR + offset);
    data_cache_hit_writeback_invalidate(src, size);
    io_write(PI_RD_LEN_REG, size - 1);
}

/* Match libdragon ed_init() __cart_dom2 for EverDrive SD access.
 * DOM1 is left alone — it uses IPL3 ROM timing (0x80371240); speeding it up
 * breaks short PI DMA reads used by DragonFS init on real hardware. */
#define ED64_PI_DOM2_SD    0x80370404U

void ed64_ll_set_sdcard_timing (void) {
    uint32_t cfg = ED64_PI_DOM2_SD;
    io_write(PI_BSD_DOM2_LAT_REG, cfg >>  0);
    io_write(PI_BSD_DOM2_PWD_REG, cfg >>  8);
    io_write(PI_BSD_DOM2_PGS_REG, cfg >> 16);
    io_write(PI_BSD_DOM2_RLS_REG, cfg >> 20);
}

void ed64_ll_refresh_sd_speed (void) {
    ed64_ll_set_sdcard_timing();
    cart_card_init();
}

ed64_save_type_t ed64_ll_get_save_type (void) {
    return ed64_ll_save_type;
}

void ed64_ll_set_save_type (ed64_save_type_t type) {
    uint16_t save_cfg = 0;
    uint8_t eeprom_on = 0;
    uint8_t sram_on = 0;
    uint8_t eeprom_size = 0;
    uint8_t sram_size = 0;
    uint8_t ram_bank = ed64_ll_sram_bank;

    ed64_ll_save_type = type;

    switch (type) {
        case SAVE_TYPE_EEPROM_16K:
            eeprom_on = 1;
            eeprom_size = 1;
            break;
        case SAVE_TYPE_EEPROM_4K:
            eeprom_on = 1;
            break;
        case SAVE_TYPE_SRAM:
            sram_on = 1;
            break;
        case SAVE_TYPE_SRAM_128K:
            sram_on = 1;
            sram_size = 1;
            break;
        case SAVE_TYPE_FLASHRAM:
            sram_on = 0;
            sram_size = 1;
            break;
        default:
            sram_on = 0;
            sram_size = 0;
            ram_bank = 1;
            break;
    }

    if (eeprom_on) {
        save_cfg |= SAV_EEP_ON;
    }
    if (sram_on) {
        save_cfg |= SAV_SRM_ON;
    }
    if (eeprom_size) {
        save_cfg |= SAV_EEP_SIZE;
    }
    if (sram_size) {
        save_cfg |= SAV_SRM_SIZE;
    }
    if (ram_bank) {
        save_cfg |= SAV_RAM_BANK;
    }
    save_cfg |= SAV_RAM_BANK_APPLY;

    ed64_ll_reg_write(REG_SAV_CFG, save_cfg);
}

void ed64_ll_get_sram (uint8_t *buffer, int size) {
    int offset = (size == (int) KiB(32)) ? 0 : SRAM_128K_OFFSET;

    pi_initialize_sram();
    dma_wait();
    pi_initialize();
    wait_ms(250);

    pi_dma_from_sram(buffer, offset, size);
    dma_wait();

    ed64_ll_set_sdcard_timing();
}

void ed64_ll_get_eeprom (uint8_t *buffer, int size) {
    int blocks = size / 8;
    for (int b = 0; b < blocks; b++) {
        eeprom_read(b, &buffer[b * 8]);
    }
}

void ed64_ll_get_fram (uint8_t *buffer, int size) {
    ed64_ll_set_save_type(SAVE_TYPE_SRAM_128K);
    wait_ms(512);

    data_cache_hit_writeback_invalidate(buffer, size);
    dma_wait();

    ed64_ll_get_sram(buffer, size);

    wait_ms(512);
    ed64_ll_set_save_type(SAVE_TYPE_FLASHRAM);
}

void ed64_ll_set_sram (uint8_t *buffer, int size) {
    int offset = (size == (int) KiB(32)) ? 0 : SRAM_128K_OFFSET;

    pi_dma_wait();
    pi_initialize_sram();
    pi_initialize();

    data_cache_hit_writeback_invalidate(buffer, size);
    dma_wait();

    pi_dma_to_sram(buffer, offset, size);
    pi_dma_wait();

    ed64_ll_set_sdcard_timing();
}

void ed64_ll_set_eeprom (uint8_t *buffer, int size) {
    int blocks = size / 8;
    for (int b = 0; b < blocks; b++) {
        eeprom_write(b, &buffer[b * 8]);
    }
}

void ed64_ll_set_fram (uint8_t *buffer, int size) {
    ed64_ll_set_save_type(SAVE_TYPE_SRAM_128K);
    wait_ms(512);

    data_cache_hit_writeback_invalidate(buffer, size);
    dma_wait();

    ed64_ll_set_sram(buffer, size);

    wait_ms(512);
    ed64_ll_set_save_type(SAVE_TYPE_FLASHRAM);
}
