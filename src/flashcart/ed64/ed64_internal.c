#include <libdragon.h>

#include "ed64_internal.h"

/* Locks the ED64 registers  */
#define ED64_KEY_LOCK   0x0000
/* Unlocks the ED64 registers  */
#define ED64_KEY_UNLOCK 0x1234

/* ED64 registers base address  */
#define REGS_BASE 0xA8040000

typedef enum {
    REG_CFG = 0,
    REG_STATUS = 1,
    REG_DMA_LENGTH = 2,
    REG_DMA_RAM_ADDR = 3,
    REG_MSG = 4,
    REG_DMA_CFG = 5,
    REG_SPI = 6,
    REG_SPI_CFG = 7,
    REG_KEY = 8,
    REG_SAV_CFG = 9,
    REG_SEC = 10,
    REG_FPGA_VER = 11,
    REG_GPIO = 12,

} ed64_registers_t;


typedef enum {
    REG_CFG_COUNT = 16,
    REG_CFG_DATA = 17,
    REG_FPGA_FW_DATA = 18,
    REG_FPGA_FW_VER = 19,
    REG_FLASHRAM_ADDR = 20,
    REG_FLASHRAM_DATA = 21,
    
} ed64_register_values_t;


#define STATE_DMA_BUSY 1
#define STATE_DMA_TOUT 2
#define STATE_USB_TXE 4
#define STATE_USB_RXF 8
#define STATE_SPI 16

// Default config?
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

typedef enum {
    ED_CFG_SDRAM_ON = 1,
    ED_CFG_BYTESWAP_ON = 2,
    ED_CFG_WRITE_MODE_ON = 4,
    ED_CFG_WRITE_ADDR_MASK = 8,
    ED_CFG_RTC_ON = 32,
    ED_CFG_GPIO_ON = 96,
    ED_CFG_64DD_ON = 256,
    ED_CFG_64DD_WRITE_ENABLED = 512,
} ed64_config_t;


// #define FPGA_FW_DATA_SKIP_FW_INIT (1 << 8)
// #define FPGA_FW_DATA_SKIP_TV_INIT (1 << 9)
// #define FPGA_FW_DATA_TV_TYPE1 (1 << 10)
// #define FPGA_FW_DATA_TV_TYPE2 (1 << 11)
// #define FPGA_FW_DATA_SKIP_SD_INIT (1 << 12)
// #define FPGA_FW_DATA_SD_TYPE (1 << 13)
#define FPGA_FW_DATA_HOT_START (1 << 14)

uint32_t ed64_bios_reg_read(uint32_t reg);
void ed64_bios_reg_write(uint32_t reg, uint32_t data);
void ed64_bios_dma_r(void * ram_address, unsigned long pi_address, unsigned long len);
void ed64_bios_dma_w(void * ram_address, unsigned long pi_address, unsigned long len);
void ed64_bios_dma_read(void *ram, uint32_t addr, uint32_t len);
void ed64_bios_dma_write(void *ram, uint32_t addr, uint32_t len);

static uint16_t spi_cfg;
uint8_t ed64_bios_ram_bank;
uint8_t ed64_bios_save_type;

#define REG_LAT 0x04
#define REG_PWD 0x04

#define ROM_LAT 0x40
#define ROM_PWD 0x12

/* register functions (dependent on flashcart version) */

/* register functions for V2 & V2.5 */
void ed64_bios_io_reg_v2(uint32_t addr, uint32_t dat) {

    *(volatile uint32_t *) (ROM_ADDR);
    *(volatile uint32_t *) (ROM_ADDR + addr) = dat;
}

/* register functions for V3 */
void ed64_bios_io_reg_v3(uint32_t addr, uint16_t dat) {

    ed64_bios_reg_write(REG_FLASHRAM_ADDR, addr);
    ed64_bios_reg_write(REG_FLASHRAM_DATA, dat);
}

/* initialize functions (dependent on flashcart version) */

/* Initilize V2 & 2.5 cart */
void ed64_bios_init_v2() {

    uint8_t buff[512];
    uint16_t cfg = ed64_bios_reg_read(REG_CFG);

    ed64_bios_reg_write(REG_CFG, 0);
    ed64_bios_io_reg_v2(0xaa / 4 * 4, 0x00980098);
    ed64_bios_dma_read_rom(buff, 0, 1);
    ed64_bios_io_reg_v2(0xaa / 4 * 4, 0x00f000f0);
    ed64_bios_dma_read_rom(buff, 0, 1);
    ed64_bios_reg_write(REG_CFG, cfg);

}

/* Initilize V3 cart */
void ed64_bios_init_v3() {

    uint8_t buff[1024];
    uint16_t cfg = ed64_bios_reg_read(REG_CFG);

    ed64_bios_reg_write(REG_CFG, 0);
    ed64_bios_reg_write(REG_CFG_COUNT, 161);
    ed64_bios_io_reg_v3(0x55, 0x98);
    ed64_bios_dma_read_rom(buff, 0, 2);
    ed64_bios_io_reg_v3(0x55, 0xF0);
    ed64_bios_dma_read_rom(buff, 0, 2);
    ed64_bios_dma_read_rom(buff, 1024, 2);
    ed64_bios_dma_read_rom(buff, 1024 + 256 - 2, 2);
    ed64_bios_reg_write(REG_CFG_COUNT, 1);

    ed64_bios_reg_write(REG_CFG, cfg);
}

/* Initialize cart */
uint8_t ed64_bios_init() {

    uint16_t firmware_ver;
    uint16_t firmware_msg;
    uint8_t cold_start;


    IO_WRITE(PI_BSD_DOM2_LAT_REG, REG_LAT);
    IO_WRITE(PI_BSD_DOM2_PWD_REG, REG_PWD);


    ed64_bios_reg_write(REG_KEY, ED64_KEY_UNLOCK);
    ed64_bios_reg_write(REG_CFG, 0x0000);


    firmware_msg = ed64_bios_reg_read(REG_FPGA_FW_DATA);
    cold_start = (firmware_msg & FPGA_FW_DATA_HOT_START) == 0 ? 1 : 0;
    if (cold_start) {
        firmware_msg |= FPGA_FW_DATA_HOT_START;
        ed64_bios_reg_write(REG_FPGA_FW_DATA, firmware_msg);
    }

    firmware_ver = ed64_bios_reg_read(REG_FPGA_FW_VER);
    if ((firmware_ver & 0xf000) >= 0x2000) {
        ed64_bios_init_v3();
    } else {
        ed64_bios_init_v2();
    }



    spi_cfg = SPI_CFG_SS | BI_SPI_SPD_LO;
    ed64_bios_reg_write(REG_CFG, ED_CFG_SDRAM_ON);
    ed64_bios_reg_write(REG_SPI_CFG, spi_cfg);
    ed64_bios_save_type = SAVE_TYPE_OFF;


    return cold_start;
}

void ed64_bios_reset_spx() {

    uint16_t cfg = ed64_bios_reg_read(REG_CFG);

    ed64_bios_reg_write(REG_CFG, 0x8000);
    wait_ms(100);
    ed64_bios_reg_write(REG_CFG, cfg);
    wait_ms(100);
}

uint32_t ed64_bios_reg_read(uint32_t reg) {

    *(volatile uint32_t *) (REGS_BASE);
    return *(volatile uint32_t *) (REGS_BASE + reg * 4);
}

void ed64_bios_reg_write(uint32_t reg, uint32_t data) {

    *(volatile uint32_t *) (REGS_BASE);
    *(volatile uint32_t *) (REGS_BASE + reg * 4) = data;
    *(volatile uint32_t *) (ROM_ADDR);

}

uint8_t ed64_bios_dma_busy() {

    while ((ed64_bios_reg_read(REG_STATUS) & STATE_DMA_BUSY) != 0);
    return ed64_bios_reg_read(REG_STATUS) & STATE_DMA_TOUT;
}


/* USB functions */

/* USB read is busy */
uint8_t ed64_bios_usb_read_busy() {

    return ed64_bios_reg_read(REG_STATUS) & STATE_USB_RXF;
}

/* USB write is busy */
uint8_t ed64_bios_usb_write_busy() {

    return ed64_bios_reg_read(REG_STATUS) & STATE_USB_TXE;
}

/* USB read */
uint8_t ed64_bios_usb_read(uint32_t start_address, uint32_t slen) {

    start_address /= 4;
    while (ed64_bios_usb_read_busy() != 0);

    ed64_bios_reg_write(REG_DMA_LENGTH, slen - 1);
    ed64_bios_reg_write(REG_DMA_RAM_ADDR, start_address);
    ed64_bios_reg_write(REG_DMA_CFG, DCFG_USB_TO_RAM);

    if (ed64_bios_dma_busy() != 0)return USB_ERROR_FIFO_TIMEOUT;

    return 0;
}

/* USB write */
uint8_t ed64_bios_usb_write(uint32_t start_address, uint32_t slen) {

    start_address /= 4;
    while (ed64_bios_usb_write_busy() != 0);

    ed64_bios_reg_write(REG_DMA_LENGTH, slen - 1);
    ed64_bios_reg_write(REG_DMA_RAM_ADDR, start_address);
    ed64_bios_reg_write(REG_DMA_CFG, DCFG_RAM_TO_USB);

    if (ed64_bios_dma_busy() != 0)return USB_ERROR_FIFO_TIMEOUT;

    return 0;
}


/* SPI functions */

uint16_t ed64_bios_spi_busy() {

    return ed64_bios_reg_read(REG_STATUS) & STATE_DMA_BUSY;
}

uint8_t ed64_bios_spi(uint8_t data) {

    ed64_bios_reg_write(REG_SPI, data);
    while (ed64_bios_spi_busy());
    return ed64_bios_reg_read(REG_SPI);
}

void ed64_bios_spi_nr(uint8_t data) {

    ed64_bios_reg_write(REG_SPI, data);
    while (ed64_bios_spi_busy());
}

void ed64_bios_set_spi_speed(uint16_t speed) {

    spi_cfg &= ~3;
    spi_cfg |= speed;
    ed64_bios_reg_write(REG_SPI_CFG, spi_cfg);
}

void ed64_bios_sd_mode(uint16_t mode) {

    spi_cfg &= ~(SPI_CFG_1BIT | SPI_CFG_RD | SPI_CFG_DAT);
    spi_cfg |= mode;
    ed64_bios_reg_write(REG_SPI_CFG, spi_cfg);
}

uint8_t ed64_bios_spi_read_to_rom(uint32_t start_address, uint16_t slen) {

    start_address /= 4;

    ed64_bios_reg_write(REG_DMA_LENGTH, slen - 1);
    ed64_bios_reg_write(REG_DMA_RAM_ADDR, start_address);
    ed64_bios_reg_write(REG_DMA_CFG, DCFG_SD_TO_RAM);

    if (ed64_bios_dma_busy() != 0)return EVD_ERROR_MMC_TIMEOUT;

    return 0;
}

void ed64_bios_byteswap_on() {
    uint16_t cfg = ed64_bios_reg_read(REG_CFG);
    cfg |= ED_CFG_BYTESWAP_ON;
    ed64_bios_reg_write(REG_CFG, cfg);
}

void ed64_bios_byteswap_off() {
    uint16_t cfg = ed64_bios_reg_read(REG_CFG);
    cfg &= ~ED_CFG_BYTESWAP_ON;
    ed64_bios_reg_write(REG_CFG, cfg);
}

uint8_t ed64_bios_get_save_type() {

    return ed64_bios_save_type;
}

void ed64_bios_set_save_type(uint8_t type) {

    uint16_t save_cfg;
    uint8_t eeprom_on, sram_on, eeprom_size, sram_size, ram_bank;
    ed64_bios_save_type = type;
    eeprom_on = 0;
    sram_on = 0;
    eeprom_size = 0;
    sram_size = 0;
    ram_bank = ed64_bios_ram_bank;


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

    ed64_bios_reg_write(REG_SAV_CFG, save_cfg);

}

void ed64_bios_set_ram_bank(uint8_t bank) {

    ed64_bios_ram_bank = bank == 0 ? 0 : 1;

}

void ed64_bios_read_bios(void *dst, uint16_t start_address, uint16_t slen) {

    uint16_t cfg = ed64_bios_reg_read(REG_CFG);

    cfg &= ~ED_CFG_SDRAM_ON;
    ed64_bios_reg_write(REG_CFG, cfg);

    ed64_bios_dma_read_rom(dst, start_address, slen);

    cfg |= ED_CFG_SDRAM_ON;
    ed64_bios_reg_write(REG_CFG, cfg);
}

void ed64_bios_dma_read_rom(void *ram, uint32_t start_address, uint32_t slen) {

    ed64_bios_dma_read(ram, ROM_ADDR + start_address * 512, slen * 512);

}

void ed64_bios_dma_write_rom(void *ram, uint32_t start_address, uint32_t slen) {

    ed64_bios_dma_write(ram, ROM_ADDR + start_address * 512, slen * 512);
}

void ed64_bios_dma_read_sram(void *ram, uint32_t addr, uint32_t len) {

    volatile uint32_t piLatReg = IO_READ(PI_BSD_DOM2_LAT_REG);
    volatile uint32_t piPwdReg = IO_READ(PI_BSD_DOM2_PWD_REG);
    volatile uint32_t piPgsReg = IO_READ(PI_BSD_DOM2_PGS_REG);
    volatile uint32_t piRlsReg = IO_READ(PI_BSD_DOM2_RLS_REG);

    IO_WRITE(PI_BSD_DOM2_PGS_REG, 0x0D);
    IO_WRITE(PI_BSD_DOM2_RLS_REG, 0x02);
    IO_WRITE(PI_BSD_DOM2_LAT_REG, 0x05);
    IO_WRITE(PI_BSD_DOM2_PWD_REG, 0x0C);

    ed64_bios_dma_read(ram, SRAM_ADDR + addr, len);

    IO_WRITE(PI_BSD_DOM2_LAT_REG, piLatReg);
    IO_WRITE(PI_BSD_DOM2_PWD_REG, piPwdReg);
    IO_WRITE(PI_BSD_DOM2_PGS_REG, piPgsReg);
    IO_WRITE(PI_BSD_DOM2_RLS_REG, piRlsReg);
}

void ed64_bios_dma_write_sram(void *ram, uint32_t addr, uint32_t len) {

    volatile uint32_t piLatReg = IO_READ(PI_BSD_DOM2_LAT_REG);
    volatile uint32_t piPwdReg = IO_READ(PI_BSD_DOM2_PWD_REG);
    volatile uint32_t piPgsReg = IO_READ(PI_BSD_DOM2_PGS_REG);
    volatile uint32_t piRlsReg = IO_READ(PI_BSD_DOM2_RLS_REG);

    IO_WRITE(PI_BSD_DOM2_PGS_REG, 0x0D);
    IO_WRITE(PI_BSD_DOM2_RLS_REG, 0x02);
    IO_WRITE(PI_BSD_DOM2_LAT_REG, 0x05);
    IO_WRITE(PI_BSD_DOM2_PWD_REG, 0x0C);

    ed64_bios_dma_write(ram, SRAM_ADDR + addr, len);


    IO_WRITE(PI_BSD_DOM2_LAT_REG, piLatReg);
    IO_WRITE(PI_BSD_DOM2_PWD_REG, piPwdReg);
    IO_WRITE(PI_BSD_DOM2_PGS_REG, piPgsReg);
    IO_WRITE(PI_BSD_DOM2_RLS_REG, piRlsReg);

}

uint16_t ed64_bios_msg_rd() {

    return ed64_bios_reg_read(REG_MSG);
}

void ed64_bios_msg_wr(uint16_t val) {

    ed64_bios_reg_write(REG_MSG, val);
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

static volatile struct PI_regs_s * const PI_regs = (struct PI_regs_s *) PI_BASE_REG;

void ed64_bios_dma_r(void * ram_address, unsigned long pi_address, unsigned long len) {


    disable_interrupts();

    dma_wait();
    IO_WRITE(PI_STATUS_REG, 3);
    PI_regs->ram_address = ram_address;
    PI_regs->pi_address = pi_address & 0x1FFFFFFF;
    PI_regs->write_length = len - 1;
    dma_wait();

    enable_interrupts();
}

void ed64_bios_dma_w(void * ram_address, unsigned long pi_address, unsigned long len) {


    disable_interrupts();
    dma_wait();
    IO_WRITE(PI_STATUS_REG, 3);
    PI_regs->ram_address = ram_address;
    PI_regs->pi_address = pi_address & 0x1FFFFFFF;
    PI_regs->read_length = len - 1;
    dma_wait();
    enable_interrupts();
}

void ed64_bios_dma_read(void *ram, uint32_t addr, uint32_t len) {


    if (((uint32_t) ram & 0xF0000000) == 0x80000000) {
        data_cache_hit_writeback_invalidate(ram, len);
        ed64_bios_dma_r(ram, addr, len);
    } else {
        ed64_bios_dma_r(ram, addr, len);
    }

}

void ed64_bios_dma_write(void *ram, uint32_t addr, uint32_t len) {

    if (((uint32_t) ram & 0xF0000000) == 0x80000000)data_cache_hit_writeback(ram, len);
    ed64_bios_dma_w(ram, addr, len);
}

/** @brief Get the current FPGA version */
uint16_t ed64_bios_get_fpga_version() {

    return ed64_bios_reg_read(REG_FPGA_VER);
}

/** @brief Get the current CPLD version */
uint16_t ed64_bios_get_cpld_version() {

    uint16_t cpld_version;
    uint16_t cfg = ed64_bios_reg_read(REG_CFG);

    ed64_bios_reg_write(REG_CFG, 0);
    cpld_version = ed64_bios_reg_read(REG_FPGA_FW_VER);
    ed64_bios_reg_write(REG_CFG, cfg);

    return cpld_version;
}

void ed64_bios_load_firmware(uint8_t *firmware) {

    uint32_t i;
    uint16_t f_ctr = 0;
    uint16_t cfg = ed64_bios_reg_read(REG_CFG);

    cfg &= ~ED_CFG_SDRAM_ON;
    ed64_bios_reg_write(REG_CFG, cfg);

    ed64_bios_reg_write(REG_CFG_COUNT, 0);
    wait_ms(10);
    ed64_bios_reg_write(REG_CFG_COUNT, 1);
    wait_ms(10);

    i = 0;
    for (;;) {

        ed64_bios_reg_write(REG_CFG_DATA, *(uint16_t *) & firmware[i]);
        while ((ed64_bios_reg_read(REG_CFG_COUNT) & 8) != 0);

        f_ctr = firmware[i++] == 0xff ? f_ctr + 1 : 0;
        if (f_ctr >= 47)break;
        f_ctr = firmware[i++] == 0xff ? f_ctr + 1 : 0;
        if (f_ctr >= 47)break;
    }


    while ((ed64_bios_reg_read(REG_CFG_COUNT) & 4) == 0) {
        ed64_bios_reg_write(REG_CFG_DATA, 0xffff);
        while ((ed64_bios_reg_read(REG_CFG_COUNT) & 8) != 0);
    }


    wait_ms(20);
}


void ed64_bios_lock_regs() {
    ed64_bios_reg_write(REG_KEY, ED64_KEY_LOCK);
}

void ed64_bios_unlock_regs() {
    ed64_bios_reg_write(REG_KEY, ED64_KEY_UNLOCK);
}


/* GPIO functions */

/* GPIO mode RTC */
void ed64_bios_gpio_mode_rtc() {

    uint16_t cfg = ed64_bios_reg_read(REG_CFG);
    cfg &= ~ED_CFG_GPIO_ON;
    cfg |= ED_CFG_RTC_ON;
    ed64_bios_reg_write(REG_CFG, cfg);
}

/* GPIO mode ON */
void ed64_bios_gpio_mode_io() {

    uint16_t cfg = ed64_bios_reg_read(REG_CFG);
    cfg |= ED_CFG_GPIO_ON;
    ed64_bios_reg_write(REG_CFG, cfg);
}

/* GPIO mode OFF */
void ed64_bios_gpio_off() {

    uint16_t cfg = ed64_bios_reg_read(REG_CFG);
    cfg &= ~ED_CFG_GPIO_ON;
    ed64_bios_reg_write(REG_CFG, cfg);
}

/* GPIO mode write */
void ed64_bios_gpio_write(uint8_t data) {

    ed64_bios_reg_write(REG_GPIO, data);
}

/* GPIO mode read */
uint8_t ed64_bios_gpio_read() {

    return ed64_bios_reg_read(REG_GPIO);
}



/* 64DD functions */

/* 64DD ON and Enabled?! */
void ed64_bios_64dd_ram_oe() {

    uint16_t cfg = ed64_bios_reg_read(REG_CFG);
    cfg &= ~ED_CFG_64DD_WRITE_ENABLED;
    cfg |= ED_CFG_64DD_ON;
    ed64_bios_reg_write(REG_CFG, cfg);
}

/* 64DD Write Enable?? */
void ed64_bios_64dd_ram_we() {

    uint16_t cfg = ed64_bios_reg_read(REG_CFG);
    cfg |= ED_CFG_64DD_ON | ED_CFG_64DD_WRITE_ENABLED;
    ed64_bios_reg_write(REG_CFG, cfg);
}

/* 64DD Disabled?? */
void ed64_bios_64dd_ram_off() {

    uint16_t cfg = ed64_bios_reg_read(REG_CFG);
    cfg &= ~(ED_CFG_64DD_ON | ED_CFG_64DD_WRITE_ENABLED);
    ed64_bios_reg_write(REG_CFG, cfg);
}

/* 64DD Save RAM Clear */
void ed64_bios_64dd_ram_clear() {

    uint16_t cfg = ed64_bios_reg_read(REG_CFG);
    cfg |= ED_CFG_64DD_WRITE_ENABLED;
    cfg &= ~ED_CFG_64DD_ON;
    ed64_bios_reg_write(REG_CFG, cfg);
    wait_ms(100);
}

/* 64DD Allowed on this cart?! */
uint8_t ed64_bios_get_64dd_ram_supported() {

    return (ed64_bios_reg_read(REG_STATUS) >> 15) & 1;

}
