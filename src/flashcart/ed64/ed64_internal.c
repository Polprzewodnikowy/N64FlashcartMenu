#include <libdragon.h>

#include "ed64_internal.h"



#define REG_CFG 0
#define REG_STATUS 1
#define REG_DMA_LEN 2
#define REG_DMA_RAM_ADDR 3
#define REG_MSG 4
#define REG_DMA_CFG 5
#define REG_SPI 6
#define REG_SPI_CFG 7
#define REG_KEY 8
#define REG_SAV_CFG 9
#define REG_SEC 10
#define REG_VER 11
#define REG_GPIO 12

#define REG_CFG_CNT 16
#define REG_CFG_DAT 17
#define REG_MAX_MSG 18
#define REG_MAX_VER 19
#define REG_FL_ADDR 20
#define REG_FL_DATA 21

#define STATE_DMA_BUSY 1
#define STATE_DMA_TOUT 2
#define STATE_USB_TXE 4
#define STATE_USB_RXF 8
#define STATE_SPI 16

#define DCFG_SD_TO_RAM 1
#define DCFG_RAM_TO_SD 2
#define DCFG_USB_TO_RAM 3
#define DCFG_RAM_TO_USB 4


#define SAV_EEP_ON 1
#define SAV_SRM_ON 2
#define SAV_EEP_SIZE 4
#define SAV_SRM_SIZE 8
#define SAV_RAM_BANK 128
#define SAV_RAM_BANK_APPLY 32768

#define ED_CFG_SDRAM_ON 1
#define ED_CFG_SWAP 2
#define ED_CFG_WR_MOD 4
#define ED_CFG_WR_ADDR_MASK 8
#define ED_CFG_RTC_ON 32
#define ED_CFG_GPIO_ON 96
#define ED_CFG_DD_ON 256
#define ED_CFG_DD_WE 512

#define REGS_BASE 0xA8040000

#define MAX_MSG_SKIP_FW_INIT (1 << 8)
#define MAX_MSG_SKIP_TV_INIT (1 << 9)
#define MAX_MSG_TV_TYPE1 (1 << 10)
#define MAX_MSG_TV_TYPE2 (1 << 11)
#define MAX_MSG_SKIP_SD_INIT (1 << 12)
#define MAX_MSG_SD_TYPE (1 << 13)
#define MAX_MSG_HOT_START (1 << 14)


uint32_t bi_reg_rd(uint32_t reg);
void bi_reg_wr(uint32_t reg, uint32_t data);
void bi_dma_r(void * ram_address, unsigned long pi_address, unsigned long len);
void bi_dma_w(void * ram_address, unsigned long pi_address, unsigned long len);
void bi_dma_read(void *ram, uint32_t addr, uint32_t len);
void bi_dma_write(void *ram, uint32_t addr, uint32_t len);

static uint16_t spi_cfg;
uint8_t bi_ram_bank;
uint8_t bi_save_type;

#define REG_LAT 0x04
#define REG_PWD 0x04

#define ROM_LAT 0x40
#define ROM_PWD 0x12

void bi_io_reg_v2(uint32_t addr, uint32_t dat) {

    *(volatile uint32_t *) (ROM_ADDR);
    *(volatile uint32_t *) (ROM_ADDR + addr) = dat;
}

void bi_io_reg_v3(uint32_t addr, uint16_t dat) {

    bi_reg_wr(REG_FL_ADDR, addr);
    bi_reg_wr(REG_FL_DATA, dat);
}

void bi_init_v2() {

    uint8_t buff[512];
    uint16_t cfg = bi_reg_rd(REG_CFG);

    bi_reg_wr(REG_CFG, 0);
    bi_io_reg_v2(0xaa / 4 * 4, 0x00980098);
    bi_dma_read_rom(buff, 0, 1);
    bi_io_reg_v2(0xaa / 4 * 4, 0x00f000f0);
    bi_dma_read_rom(buff, 0, 1);
    bi_reg_wr(REG_CFG, cfg);

}

void bi_init_v3() {

    uint8_t buff[1024];
    uint16_t cfg = bi_reg_rd(REG_CFG);

    bi_reg_wr(REG_CFG, 0);
    bi_reg_wr(REG_CFG_CNT, 161);
    bi_io_reg_v3(0x55, 0x98);
    bi_dma_read_rom(buff, 0, 2);
    bi_io_reg_v3(0x55, 0xF0);
    bi_dma_read_rom(buff, 0, 2);
    bi_dma_read_rom(buff, 1024, 2);
    bi_dma_read_rom(buff, 1024 + 256 - 2, 2);
    bi_reg_wr(REG_CFG_CNT, 1);

    bi_reg_wr(REG_CFG, cfg);
}

uint8_t bi_init() {

    uint16_t max_ver;
    uint16_t max_msg;
    uint8_t cold_start;


    IO_WRITE(PI_BSD_DOM2_LAT_REG, REG_LAT);
    IO_WRITE(PI_BSD_DOM2_PWD_REG, REG_PWD);


    bi_reg_wr(REG_KEY, 0x1234);
    bi_reg_wr(REG_CFG, 0x0000);


    max_msg = bi_reg_rd(REG_MAX_MSG);
    cold_start = (max_msg & MAX_MSG_HOT_START) == 0 ? 1 : 0;
    if (cold_start) {
        max_msg |= MAX_MSG_HOT_START;
        bi_reg_wr(REG_MAX_MSG, max_msg);
    }

    max_ver = bi_reg_rd(REG_MAX_VER);
    if ((max_ver & 0xf000) >= 0x2000) {
        bi_init_v3();
    } else {
        bi_init_v2();
    }



    spi_cfg = SPI_CFG_SS | BI_SPI_SPD_LO;
    bi_reg_wr(REG_CFG, ED_CFG_SDRAM_ON);
    bi_reg_wr(REG_SPI_CFG, spi_cfg);
    bi_save_type = 0;


    return cold_start;
}

void bi_reset_spx() {

    uint16_t cfg = bi_reg_rd(REG_CFG);

    bi_reg_wr(REG_CFG, 0x8000);
    bi_sleep(100);
    bi_reg_wr(REG_CFG, cfg);
    bi_sleep(100);
}

uint32_t bi_reg_rd(uint32_t reg) {

    *(volatile uint32_t *) (REGS_BASE);
    return *(volatile uint32_t *) (REGS_BASE + reg * 4);
}

void bi_reg_wr(uint32_t reg, uint32_t data) {

    *(volatile uint32_t *) (REGS_BASE);
    *(volatile uint32_t *) (REGS_BASE + reg * 4) = data;
    *(volatile uint32_t *) (ROM_ADDR);

}

uint8_t bi_dma_busy() {

    while ((bi_reg_rd(REG_STATUS) & STATE_DMA_BUSY) != 0);
    return bi_reg_rd(REG_STATUS) & STATE_DMA_TOUT;
}

uint8_t bi_usb_rd_busy() {

    return bi_reg_rd(REG_STATUS) & STATE_USB_RXF;
}

uint8_t bi_usb_wr_busy() {

    return bi_reg_rd(REG_STATUS) & STATE_USB_TXE;
}

uint8_t bi_usb_rd(uint32_t saddr, uint32_t slen) {

    saddr /= 4;
    while (bi_usb_rd_busy() != 0);

    bi_reg_wr(REG_DMA_LEN, slen - 1);
    bi_reg_wr(REG_DMA_RAM_ADDR, saddr);
    bi_reg_wr(REG_DMA_CFG, DCFG_USB_TO_RAM);

    if (bi_dma_busy() != 0)return EVD_ERROR_FIFO_TIMEOUT;

    return 0;
}

uint8_t bi_usb_wr(uint32_t saddr, uint32_t slen) {

    saddr /= 4;
    while (bi_usb_wr_busy() != 0);

    bi_reg_wr(REG_DMA_LEN, slen - 1);
    bi_reg_wr(REG_DMA_RAM_ADDR, saddr);
    bi_reg_wr(REG_DMA_CFG, DCFG_RAM_TO_USB);

    if (bi_dma_busy() != 0)return EVD_ERROR_FIFO_TIMEOUT;

    return 0;
}

uint16_t bi_spi_busy() {

    return bi_reg_rd(REG_STATUS) & STATE_DMA_BUSY;
}

uint8_t bi_spi(uint8_t data) {

    bi_reg_wr(REG_SPI, data);
    while (bi_spi_busy());
    return bi_reg_rd(REG_SPI);
}

void bi_spi_nr(uint8_t data) {

    bi_reg_wr(REG_SPI, data);
    while (bi_spi_busy());
}

void bi_set_spi_spd(uint16_t speed) {

    spi_cfg &= ~3;
    spi_cfg |= speed;
    bi_reg_wr(REG_SPI_CFG, spi_cfg);
}

void bi_sd_mode(uint16_t mode) {

    spi_cfg &= ~(SPI_CFG_1BIT | SPI_CFG_RD | SPI_CFG_DAT);
    spi_cfg |= mode;
    bi_reg_wr(REG_SPI_CFG, spi_cfg);
}

uint8_t bi_spi_read_to_rom(uint32_t saddr, uint16_t slen) {

    saddr /= 4;

    bi_reg_wr(REG_DMA_LEN, slen - 1);
    bi_reg_wr(REG_DMA_RAM_ADDR, saddr);
    bi_reg_wr(REG_DMA_CFG, DCFG_SD_TO_RAM);

    if (bi_dma_busy() != 0)return EVD_ERROR_MMC_TIMEOUT;

    return 0;
}

void bi_swap_on() {
    uint16_t cfg = bi_reg_rd(REG_CFG);
    cfg |= ED_CFG_SWAP;
    bi_reg_wr(REG_CFG, cfg);
}

void bi_swap_off() {
    uint16_t cfg = bi_reg_rd(REG_CFG);
    cfg &= ~ED_CFG_SWAP;
    bi_reg_wr(REG_CFG, cfg);
}

uint8_t bi_get_save_type() {

    return bi_save_type;
}

void bi_set_save_type(uint8_t type) {

    uint16_t save_cfg;
    uint8_t eeprom_on, sram_on, eeprom_size, sram_size, ram_bank;
    bi_save_type = type;
    eeprom_on = 0;
    sram_on = 0;
    eeprom_size = 0;
    sram_size = 0;
    ram_bank = bi_ram_bank;


    switch (type) {
        case SAVE_TYPE_EEP16k:
            eeprom_on = 1;
            eeprom_size = 1;
            break;
        case SAVE_TYPE_EEP4k:
            eeprom_on = 1;
            break;
        case SAVE_TYPE_SRAM:
            sram_on = 1;
            break;
        case SAVE_TYPE_SRAM128:
            sram_on = 1;
            sram_size = 1;
            break;
        case SAVE_TYPE_FLASH:
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

    bi_reg_wr(REG_SAV_CFG, save_cfg);

}

void bi_read_bios(void *dst, uint16_t saddr, uint16_t slen) {

    uint16_t cfg = bi_reg_rd(REG_CFG);

    cfg &= ~ED_CFG_SDRAM_ON;
    bi_reg_wr(REG_CFG, cfg);

    bi_dma_read_rom(dst, saddr, slen);

    cfg |= ED_CFG_SDRAM_ON;
    bi_reg_wr(REG_CFG, cfg);
}

void bi_dma_read_rom(void *ram, uint32_t saddr, uint32_t slen) {

    bi_dma_read(ram, ROM_ADDR + saddr * 512, slen * 512);

}

void bi_dma_write_rom(void *ram, uint32_t saddr, uint32_t slen) {

    bi_dma_write(ram, ROM_ADDR + saddr * 512, slen * 512);
}

void bi_dma_read_sram(void *ram, uint32_t addr, uint32_t len) {

    volatile uint32_t piLatReg = IO_READ(PI_BSD_DOM2_LAT_REG);
    volatile uint32_t piPwdReg = IO_READ(PI_BSD_DOM2_PWD_REG);
    volatile uint32_t piPgsReg = IO_READ(PI_BSD_DOM2_PGS_REG);
    volatile uint32_t piRlsReg = IO_READ(PI_BSD_DOM2_RLS_REG);

    IO_WRITE(PI_BSD_DOM2_PGS_REG, 0x0D);
    IO_WRITE(PI_BSD_DOM2_RLS_REG, 0x02);
    IO_WRITE(PI_BSD_DOM2_LAT_REG, 0x05);
    IO_WRITE(PI_BSD_DOM2_PWD_REG, 0x0C);

    bi_dma_read(ram, SRAM_ADDR + addr, len);

    IO_WRITE(PI_BSD_DOM2_LAT_REG, piLatReg);
    IO_WRITE(PI_BSD_DOM2_PWD_REG, piPwdReg);
    IO_WRITE(PI_BSD_DOM2_PGS_REG, piPgsReg);
    IO_WRITE(PI_BSD_DOM2_RLS_REG, piRlsReg);
}

void bi_dma_write_sram(void *ram, uint32_t addr, uint32_t len) {

    volatile uint32_t piLatReg = IO_READ(PI_BSD_DOM2_LAT_REG);
    volatile uint32_t piPwdReg = IO_READ(PI_BSD_DOM2_PWD_REG);
    volatile uint32_t piPgsReg = IO_READ(PI_BSD_DOM2_PGS_REG);
    volatile uint32_t piRlsReg = IO_READ(PI_BSD_DOM2_RLS_REG);

    IO_WRITE(PI_BSD_DOM2_PGS_REG, 0x0D);
    IO_WRITE(PI_BSD_DOM2_RLS_REG, 0x02);
    IO_WRITE(PI_BSD_DOM2_LAT_REG, 0x05);
    IO_WRITE(PI_BSD_DOM2_PWD_REG, 0x0C);

    bi_dma_write(ram, SRAM_ADDR + addr, len);


    IO_WRITE(PI_BSD_DOM2_LAT_REG, piLatReg);
    IO_WRITE(PI_BSD_DOM2_PWD_REG, piPwdReg);
    IO_WRITE(PI_BSD_DOM2_PGS_REG, piPgsReg);
    IO_WRITE(PI_BSD_DOM2_RLS_REG, piRlsReg);

}

uint16_t bi_msg_rd() {

    return bi_reg_rd(REG_MSG);
}

void bi_msg_wr(uint16_t val) {

    bi_reg_wr(REG_MSG, val);
}

typedef struct PI_regs_s {
    /** @brief Uncached address in RAM where data should be found */
    void * ram_address;
    /** @brief Address of data on peripheral */
    unsigned long pi_address;
    /** @brief How much data to read from RAM into the peripheral */
    unsigned long read_length;
    /** @brief How much data to write to RAM from the peripheral */
    unsigned long write_length;
    /** @brief Status of the PI, including DMA busy */
    unsigned long status;
} _PI_regs_s;

static volatile struct PI_regs_s * const PI_regs = (struct PI_regs_s *) 0xa4600000;

void bi_dma_r(void * ram_address, unsigned long pi_address, unsigned long len) {


    disable_interrupts();

    while (dma_busy());
    IO_WRITE(PI_STATUS_REG, 3);
    PI_regs->ram_address = ram_address;
    PI_regs->pi_address = pi_address & 0x1FFFFFFF; // (pi_address | 0x10000000) & 0x1FFFFFFF;
    PI_regs->write_length = len - 1;
    while (dma_busy());

    enable_interrupts();
}

void bi_dma_w(void * ram_address, unsigned long pi_address, unsigned long len) {


    disable_interrupts();
    while (dma_busy());
    IO_WRITE(PI_STATUS_REG, 3);
    PI_regs->ram_address = ram_address;
    PI_regs->pi_address = pi_address & 0x1FFFFFFF; // (pi_address | 0x10000000) & 0x1FFFFFFF;
    PI_regs->read_length = len - 1;
    while (dma_busy());
    enable_interrupts();
}

void bi_dma_read(void *ram, uint32_t addr, uint32_t len) {


    if (((uint32_t) ram & 0xF0000000) == 0x80000000) {
        data_cache_hit_writeback_invalidate(ram, len);
        bi_dma_r(ram, addr, len);
    } else {
        bi_dma_r(ram, addr, len);
    }

}

void bi_dma_write(void *ram, uint32_t addr, uint32_t len) {

    if (((uint32_t) ram & 0xF0000000) == 0x80000000)data_cache_hit_writeback(ram, len);
    bi_dma_w(ram, addr, len);
}

uint16_t bi_fpga_ver() {

    return bi_reg_rd(REG_VER);
}

void bi_load_firmware(uint8_t *firm) {

    uint32_t i;
    uint16_t f_ctr = 0;
    uint16_t cfg = bi_reg_rd(REG_CFG);

    cfg &= ~ED_CFG_SDRAM_ON;
    bi_reg_wr(REG_CFG, cfg);

    bi_reg_wr(REG_CFG_CNT, 0);
    bi_sleep(10);
    bi_reg_wr(REG_CFG_CNT, 1);
    bi_sleep(10);

    i = 0;
    for (;;) {

        bi_reg_wr(REG_CFG_DAT, *(uint16_t *) & firm[i]);
        while ((bi_reg_rd(REG_CFG_CNT) & 8) != 0);

        f_ctr = firm[i++] == 0xff ? f_ctr + 1 : 0;
        if (f_ctr >= 47)break;
        f_ctr = firm[i++] == 0xff ? f_ctr + 1 : 0;
        if (f_ctr >= 47)break;
    }


    while ((bi_reg_rd(REG_CFG_CNT) & 4) == 0) {
        bi_reg_wr(REG_CFG_DAT, 0xffff);
        while ((bi_reg_rd(REG_CFG_CNT) & 8) != 0);
    }


    bi_sleep(20);
}

void bi_sleep(uint32_t ms) {

    uint32_t current_ms = get_ticks_ms();

    while (get_ticks_ms() - current_ms < ms);

}

void bi_lock_regs() {
    bi_reg_wr(REG_KEY, 0);
}

void bi_unlock_regs() {
    bi_reg_wr(REG_KEY, 0x1234);
}


uint16_t bi_cpld_ver() {

    uint16_t ver;
    uint16_t cfg = bi_reg_rd(REG_CFG);

    bi_reg_wr(REG_CFG, 0);
    ver = bi_reg_rd(REG_MAX_VER);
    bi_reg_wr(REG_CFG, cfg);

    return ver;
}

void bi_gpio_mode_rtc() {

    uint16_t cfg = bi_reg_rd(REG_CFG);
    cfg &= ~ED_CFG_GPIO_ON;
    cfg |= ED_CFG_RTC_ON;
    bi_reg_wr(REG_CFG, cfg);
}

void bi_gpio_mode_io() {

    uint16_t cfg = bi_reg_rd(REG_CFG);
    cfg |= ED_CFG_GPIO_ON;
    bi_reg_wr(REG_CFG, cfg);
}

void bi_gpio_off() {

    uint16_t cfg = bi_reg_rd(REG_CFG);
    cfg &= ~ED_CFG_GPIO_ON;
    bi_reg_wr(REG_CFG, cfg);
}

void bi_gpio_wr(uint8_t data) {

    bi_reg_wr(REG_GPIO, data);
}

uint8_t bi_gpio_rd() {

    return bi_reg_rd(REG_GPIO);
}

void bi_set_ram_bank(uint8_t bank) {

    bi_ram_bank = bank == 0 ? 0 : 1;

}



void bi_dd_ram_oe() {

    uint16_t cfg = bi_reg_rd(REG_CFG);
    cfg &= ~ED_CFG_DD_WE;
    cfg |= ED_CFG_DD_ON;
    bi_reg_wr(REG_CFG, cfg);
}

void bi_dd_ram_we() {

    uint16_t cfg = bi_reg_rd(REG_CFG);
    cfg |= ED_CFG_DD_ON | ED_CFG_DD_WE;
    bi_reg_wr(REG_CFG, cfg);
}

void bi_dd_ram_off() {

    uint16_t cfg = bi_reg_rd(REG_CFG);
    cfg &= ~(ED_CFG_DD_ON | ED_CFG_DD_WE);
    bi_reg_wr(REG_CFG, cfg);
}

void bi_dd_ram_clr() {

    uint16_t cfg = bi_reg_rd(REG_CFG);
    cfg |= ED_CFG_DD_WE;
    cfg &= ~ED_CFG_DD_ON;
    bi_reg_wr(REG_CFG, cfg);
    bi_sleep(100);
}

uint8_t bi_dd_ram_supported() {

    return (bi_reg_rd(REG_STATUS) >> 15) & 1;

}