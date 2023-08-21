#include <libdragon.h>

#include "ed64_ll.h"


/* ED64 configuration registers base address  */
#define ED64_CONFIG_REGS_BASE (0xA8040000)

typedef enum {
    // REG_CFG = 0,
    // REG_STATUS = 1,
    // REG_DMA_LENGTH = 2,
    // REG_DMA_RAM_ADDR = 3,
    // REG_MSG = 4,
    // REG_DMA_CFG = 5,
    // REG_SPI = 6,
    // REG_SPI_CFG = 7,
    // REG_KEY = 8,
    REG_SAV_CFG = 9,
    // REG_SEC = 10, /* Sectors?? */
    // REG_FPGA_VERSION = 11, /* Altera (Intel) MAX */
    // REG_GPIO = 12,

} ed64_registers_t;


#define SAV_EEP_ON 1
#define SAV_SRM_ON 2
#define SAV_EEP_SIZE 4
#define SAV_SRM_SIZE 8

#define SAV_RAM_BANK 128
#define SAV_RAM_BANK_APPLY 32768

uint32_t ed64_ll_reg_read(uint32_t reg);
void ed64_ll_reg_write(uint32_t reg, uint32_t data);

uint8_t ed64_ll_sram_bank;
ed64_save_type_t ed64_ll_save_type;


uint32_t ed64_ll_reg_read(uint32_t reg) {

    *(volatile uint32_t *) (ED64_CONFIG_REGS_BASE);
    return *(volatile uint32_t *) (ED64_CONFIG_REGS_BASE + reg * 4);
}

void ed64_ll_reg_write(uint32_t reg, uint32_t data) {

    *(volatile uint32_t *) (ED64_CONFIG_REGS_BASE);
    *(volatile uint32_t *) (ED64_CONFIG_REGS_BASE + reg * 4) = data;
    *(volatile uint32_t *) (ROM_ADDRESS);

}


ed64_save_type_t ed64_ll_get_save_type() {

    return ed64_ll_save_type;
}

void ed64_ll_set_save_type(ed64_save_type_t type) {

    uint16_t save_cfg;
    uint8_t eeprom_on, sram_on, eeprom_size, sram_size, ram_bank;
    ed64_ll_save_type = type;
    eeprom_on = 0;
    sram_on = 0;
    eeprom_size = 0;
    sram_size = 0;
    ram_bank = ed64_ll_sram_bank;


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

    save_cfg = 0;
    if (eeprom_on)save_cfg |= SAV_EEP_ON;
    if (sram_on)save_cfg |= SAV_SRM_ON;
    if (eeprom_size)save_cfg |= SAV_EEP_SIZE;
    if (sram_size)save_cfg |= SAV_SRM_SIZE;
    if (ram_bank)save_cfg |= SAV_RAM_BANK;
    save_cfg |= SAV_RAM_BANK_APPLY;

    ed64_ll_reg_write(REG_SAV_CFG, save_cfg);

}

void ed64_ll_set_sram_bank(uint8_t bank) {

    ed64_ll_sram_bank = bank == 0 ? 0 : 1;

}
