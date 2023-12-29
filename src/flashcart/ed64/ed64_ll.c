#include <libdragon.h>

#include "../flashcart_utils.h"
#include "ed64_ll.h"

// NOTE: these are a replica of libcart cart.c defines!
#define PI_BASE_REG		    (0x04600000)
#define PI_STATUS_REG		(PI_BASE_REG+0x10)

#define PI_BSD_DOM1_LAT_REG	(PI_BASE_REG+0x14)

/* PI dom1 pulse width (R/W): [7:0] domain 1 device R/W strobe pulse width */
#define PI_BSD_DOM1_PWD_REG	(PI_BASE_REG+0x18)

/* PI dom1 page size (R/W): [3:0] domain 1 device page size */
#define PI_BSD_DOM1_PGS_REG	(PI_BASE_REG+0x1C)    /*   page size */

/* PI dom1 release (R/W): [1:0] domain 1 device R/W release duration */
#define PI_BSD_DOM1_RLS_REG	(PI_BASE_REG+0x20)

/* PI dom2 latency (R/W): [7:0] domain 2 device latency */
#define PI_BSD_DOM2_LAT_REG	(PI_BASE_REG+0x24)    /* Domain 2 latency */

/* PI dom2 pulse width (R/W): [7:0] domain 2 device R/W strobe pulse width */
#define PI_BSD_DOM2_PWD_REG	(PI_BASE_REG+0x28)    /*   pulse width */

/* PI dom2 page size (R/W): [3:0] domain 2 device page size */
#define PI_BSD_DOM2_PGS_REG	(PI_BASE_REG+0x2C)    /*   page size */

/* PI dom2 release (R/W): [1:0] domain 2 device R/W release duration */
#define PI_BSD_DOM2_RLS_REG	(PI_BASE_REG+0x30)    /*   release duration */

// #define	IO_READ(addr)		(*(volatile uint32_t *)PHYS_TO_K1(addr))
// #define	IO_WRITE(addr,data)	(*(volatile uint32_t *)PHYS_TO_K1(addr)=(uint32_t)(data))

//#define	PHYS_TO_K1(x)	((uint32_t)(x)|0xA0000000)	/* physical to kseg1 */

// END NOTE.


/* Locks the ED64 registers  */
#define ED64_KEY_LOCK   0x0000
/* Unlocks the ED64 registers  */
#define ED64_KEY_UNLOCK 0x1234

/* ED64 registers base address  */
#define ED64_REGS_BASE (0xA8040000)

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
    REG_SEC = 10, /* Sectors?? */
    REG_FPGA_VERSION = 11, /* Altera (Intel) MAX */
    REG_GPIO = 12,

} ed64_registers_t;


typedef enum {
    REG_CFG_COUNT = 16,
    REG_CFG_DATA = 17,
    REG_FPGA_FW_DATA = 18,
    REG_FPGA_FW_VER = 19,
    REG_FLASHRAM_ADDR = 20,
    REG_FLASHRAM_DATA = 21,
    
} ed64_register_args_t;


typedef enum {
    STATE_DMA_BUSY = 1,
    STATE_DMA_TOUT = 2,
    STATE_USB_TXE = 4,
    STATE_USB_RXF = 8,
    STATE_SPI = 16,

} ed64_dma_state_t;


// typedef enum {
//     DCFG_SD_TO_RAM = 1,
//     DCFG_RAM_TO_SD = 2,
//     DCFG_USB_TO_RAM = 3,
//     DCFG_RAM_TO_USB = 4,

// } ed64_disk_mode_t;


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


#define FPGA_FW_DATA_SKIP_FW_INIT   (1 << 8)
#define FPGA_FW_DATA_SKIP_TV_INIT   (1 << 9)
#define FPGA_FW_DATA_TV_TYPE1       (1 << 10)
#define FPGA_FW_DATA_TV_TYPE2       (1 << 11)
#define FPGA_FW_DATA_SKIP_SD_INIT   (1 << 12)
#define FPGA_FW_DATA_SD_TYPE        (1 << 13)
#define FPGA_FW_DATA_HOT_START      (1 << 14)

uint32_t ed64_ll_reg_read(uint32_t reg);
void ed64_ll_reg_write(uint32_t reg, uint32_t data);
void ed64_ll_dma_pi_read(void * ram_address, unsigned long pi_address, unsigned long length);
void ed64_ll_dma_pi_write(void * ram_address, unsigned long pi_address, unsigned long length);
void ed64_ll_dma_read(void *ram, uint32_t address, uint32_t length);
void ed64_ll_dma_write(void *ram, uint32_t address, uint32_t length);

/* DMA functions */
void ed64_ll_dma_read_rom(void *ram, uint32_t start_address, uint32_t slen);
void ed64_ll_dma_write_rom(void *ram, uint32_t start_address, uint32_t slen);
void ed64_ll_dma_read_sram(void *ram, uint32_t address, uint32_t length);
void ed64_ll_dma_write_sram(void *ram, uint32_t address, uint32_t length);

// static uint16_t spi_cfg;
uint8_t ed64_ll_sram_bank;
ed64_save_type_t ed64_ll_save_type;

#define REG_LAT 0x04
#define REG_PWD 0x04

#define ROM_LAT 0x40
#define ROM_PWD 0x12

/* register functions (dependent on flashcart version) */

/* register functions for V2 & V2.5  (These carts do not support FLASHRAM)*/
/* The end of SDRAM is used for SRAM or FlashRAM save types */
void ed64_ll_io_reg_v2(uint32_t address, uint32_t data) {

    *(volatile uint32_t *) (ROM_ADDRESS);
    *(volatile uint32_t *) (ROM_ADDRESS + address) = data;
}

/* register functions for V3 */
void ed64_ll_io_reg_v3(uint32_t address, uint16_t data) {

    ed64_ll_reg_write(REG_FLASHRAM_ADDR, address);
    ed64_ll_reg_write(REG_FLASHRAM_DATA, data);
}

/* initialize functions (dependent on flashcart version) */

/* Initilize V2 & 2.5 cart */
void ed64_ll_init_v2() {

    uint8_t buff[512];
    uint16_t cfg = ed64_ll_reg_read(REG_CFG);

    ed64_ll_reg_write(REG_CFG, 0);
    ed64_ll_io_reg_v2(0xaa / 4 * 4, 0x00980098);
    ed64_ll_dma_read_rom(buff, 0, 1);
    ed64_ll_io_reg_v2(0xaa / 4 * 4, 0x00f000f0);
    ed64_ll_dma_read_rom(buff, 0, 1);
    ed64_ll_reg_write(REG_CFG, cfg);

}

/* Initilize V3 cart */
void ed64_ll_init_v3() {

    uint8_t buff[1024];
    uint16_t cfg = ed64_ll_reg_read(REG_CFG);

    ed64_ll_reg_write(REG_CFG, 0);
    ed64_ll_reg_write(REG_CFG_COUNT, 161);
    ed64_ll_io_reg_v3(0x55, 0x98);
    ed64_ll_dma_read_rom(buff, 0, 2);
    ed64_ll_io_reg_v3(0x55, 0xF0);
    ed64_ll_dma_read_rom(buff, 0, 2);
    ed64_ll_dma_read_rom(buff, 1024, 2);
    ed64_ll_dma_read_rom(buff, 1024 + 256 - 2, 2);
    ed64_ll_reg_write(REG_CFG_COUNT, 1);

    ed64_ll_reg_write(REG_CFG, cfg);
}

/* Initialize cart */
int ed64_ll_init() {

    uint16_t firmware_ver;
    uint16_t firmware_msg;
    uint8_t cold_start;

    // TODO: take into account libCart!
    io_write(PI_BSD_DOM2_LAT_REG, REG_LAT);
    io_write(PI_BSD_DOM2_PWD_REG, REG_PWD);
    dma_wait(); // Make sure the libdragon Async io methods have been performed.


    ed64_ll_reg_write(REG_KEY, ED64_KEY_UNLOCK);
    ed64_ll_reg_write(REG_CFG, 0x0000);


    firmware_msg = ed64_ll_reg_read(REG_FPGA_FW_DATA);
    cold_start = (firmware_msg & FPGA_FW_DATA_HOT_START) == 0 ? 1 : 0;
    if (cold_start) {
        firmware_msg |= FPGA_FW_DATA_HOT_START;
        ed64_ll_reg_write(REG_FPGA_FW_DATA, firmware_msg);
    }

    firmware_ver = ed64_ll_reg_read(REG_FPGA_FW_VER);
    if ((firmware_ver & 0xf000) >= 0x2000) {
        ed64_ll_init_v3();
    } else {
        ed64_ll_init_v2();
    }



    //spi_cfg = SPI_CFG_SS | BI_SPI_SPD_LO;
    ed64_ll_reg_write(REG_CFG, ED_CFG_SDRAM_ON);
    //ed64_ll_reg_write(REG_SPI_CFG, spi_cfg);
    ed64_ll_save_type = SAVE_TYPE_NONE;


    return cold_start;
}

void ed64_ll_reset_spx() {

    uint16_t cfg = ed64_ll_reg_read(REG_CFG);

    ed64_ll_reg_write(REG_CFG, 0x8000);
    wait_ms(100);
    ed64_ll_reg_write(REG_CFG, cfg);
    wait_ms(100);
}

uint32_t ed64_ll_reg_read(uint32_t reg) {

    *(volatile uint32_t *) (ED64_REGS_BASE);
    return *(volatile uint32_t *) (ED64_REGS_BASE + reg * 4);
}

void ed64_ll_reg_write(uint32_t reg, uint32_t data) {

    *(volatile uint32_t *) (ED64_REGS_BASE);
    *(volatile uint32_t *) (ED64_REGS_BASE + reg * 4) = data;
    *(volatile uint32_t *) (ROM_ADDRESS);

}

// /*  Used for USB and SPI functions */
// uint8_t ed64_ll_dma_busy() {

//     while ((ed64_ll_reg_read(REG_STATUS) & STATE_DMA_BUSY) != 0);
//     return ed64_ll_reg_read(REG_STATUS) & STATE_DMA_TOUT;
// }


// /* USB functions */

// /* USB read is busy */
// uint8_t ed64_ll_usb_read_busy() {

//     return ed64_ll_reg_read(REG_STATUS) & STATE_USB_RXF;
// }

// /* USB write is busy */
// uint8_t ed64_ll_usb_write_busy() {

//     return ed64_ll_reg_read(REG_STATUS) & STATE_USB_TXE;
// }

// /* USB read */
// uint8_t ed64_ll_usb_read(uint32_t start_address, uint32_t slen) {

//     start_address /= 4;
//     while (ed64_ll_usb_read_busy() != 0);

//     ed64_ll_reg_write(REG_DMA_LENGTH, slen - 1);
//     ed64_ll_reg_write(REG_DMA_RAM_ADDR, start_address);
//     ed64_ll_reg_write(REG_DMA_CFG, DCFG_USB_TO_RAM);

//     if (ed64_ll_dma_busy() != 0)return USB_ERROR_FIFO_TIMEOUT;

//     return 0;
// }

// /* USB write */
// uint8_t ed64_ll_usb_write(uint32_t start_address, uint32_t slen) {

//     start_address /= 4;
//     while (ed64_ll_usb_write_busy() != 0);

//     ed64_ll_reg_write(REG_DMA_LENGTH, slen - 1);
//     ed64_ll_reg_write(REG_DMA_RAM_ADDR, start_address);
//     ed64_ll_reg_write(REG_DMA_CFG, DCFG_RAM_TO_USB);

//     if (ed64_ll_dma_busy() != 0)return USB_ERROR_FIFO_TIMEOUT;

//     return 0;
// }


ed64_save_type_t ed64_ll_get_save_type() {

    return ed64_ll_save_type;
}

void ed64_ll_set_save_type(ed64_save_type_t type) {

    uint16_t save_cfg = 0;
    uint8_t enable_eeprom = false;
    uint8_t eeprom_size_16k = false;
    uint8_t enable_sram = false;
    uint8_t ram_size_large = false;
    ed64_ll_save_type = type;
    uint8_t ram_bank = ed64_ll_sram_bank;


    switch (type) {
        case SAVE_TYPE_EEPROM_16K:
            enable_eeprom = true;
            eeprom_size_16k = true;
            break;
        case SAVE_TYPE_EEPROM_4K:
            enable_eeprom = true;
            break;
        case SAVE_TYPE_SRAM:
            enable_sram = true;
            break;
        case SAVE_TYPE_SRAM_128K:
            enable_sram = true;
            ram_size_large = true;
            break;
        case SAVE_TYPE_FLASHRAM:
            enable_sram = false;
            ram_size_large = true;
            break;
        default:
            enable_sram = false;
            ram_size_large = false;
            ram_bank = 1;
            break;
    }

    if (enable_eeprom)save_cfg |= SAV_EEP_ON;
    if (enable_sram)save_cfg |= SAV_SRM_ON;
    if (eeprom_size_16k)save_cfg |= SAV_EEP_SIZE;
    if (ram_size_large)save_cfg |= SAV_SRM_SIZE;
    if (ram_bank)save_cfg |= SAV_RAM_BANK;
    save_cfg |= SAV_RAM_BANK_APPLY;

    ed64_ll_reg_write(REG_SAV_CFG, save_cfg);

}

void ed64_ll_set_sram_bank(uint8_t bank) {

    ed64_ll_sram_bank = bank == 0 ? 0 : 1;

}

/* reads metadata related to the assembily date and cart capabilities */
void ed64_ll_read_cart_metadata(void *dest) {

    uint16_t cfg = ed64_ll_reg_read(REG_CFG);

    cfg &= ~ED_CFG_SDRAM_ON;
    ed64_ll_reg_write(REG_CFG, cfg);

    ed64_ll_dma_read_rom(dest, 0, 1);

    // assembily_date = (buff[0x38] << 8) | buff[0x39];
    // assembily_time = (buff[0x3A] << 8) | buff[0x3B];
    // serial_number = (buff[0x3C] << 8) | buff[0x3D];
    // cic_6105_compatible = buff[0x40] == 0x03 ? true : false; // CIC_6105 : CIC_6102;


    cfg |= ED_CFG_SDRAM_ON;
    ed64_ll_reg_write(REG_CFG, cfg);
}

void ed64_ll_dma_read_rom(void *ram, uint32_t start_address, uint32_t slen) {

    ed64_ll_dma_read(ram, ROM_ADDRESS + start_address * 512, slen * 512);

}

void ed64_ll_dma_write_rom(void *ram, uint32_t start_address, uint32_t slen) {

    ed64_ll_dma_write(ram, ROM_ADDRESS + start_address * 512, slen * 512);
}

/* Read from SRAM over DMA */
void ed64_ll_dma_read_sram(void *ram, uint32_t address, uint32_t length) {
    // Note: seemingly the ED64 cannot read SRAM at full speed,
    // so we need to slow it down temporariy!

    // Get the current timings
    // TODO: is `volatile` actually needed?!
    volatile uint32_t piLatReg = io_read(PI_BSD_DOM2_LAT_REG);
    volatile uint32_t piPwdReg = io_read(PI_BSD_DOM2_PWD_REG);
    volatile uint32_t piPgsReg = io_read(PI_BSD_DOM2_PGS_REG);
    volatile uint32_t piRlsReg = io_read(PI_BSD_DOM2_RLS_REG);
    dma_wait(); // Make sure the libdragon Async io methods have been performed.

    // Set slow timings
    io_write(PI_BSD_DOM2_PGS_REG, 0x0D);
    io_write(PI_BSD_DOM2_RLS_REG, 0x02);
    io_write(PI_BSD_DOM2_LAT_REG, 0x05);
    io_write(PI_BSD_DOM2_PWD_REG, 0x0C);
    dma_wait(); // Make sure the libdragon Async io methods have been performed.

    // Read SRAM
    ed64_ll_dma_read(ram, SRAM_ADDRESS + address, length);

    // Restore original timings
    io_write(PI_BSD_DOM2_LAT_REG, piLatReg);
    io_write(PI_BSD_DOM2_PWD_REG, piPwdReg);
    io_write(PI_BSD_DOM2_PGS_REG, piPgsReg);
    io_write(PI_BSD_DOM2_RLS_REG, piRlsReg);
    dma_wait(); // Make sure the libdragon Async io methods have been performed.
}

/* Write to SRAM over DMA */
void ed64_ll_dma_write_sram(void *ram, uint32_t address, uint32_t length) {
    // Note: seemingly the ED64 cannot write SRAM at full speed,
    // so we need to slow it down temporariy!

    // Get the current timings
    // TODO: is `volatile` actually needed?!
    volatile uint32_t piLatReg = io_read(PI_BSD_DOM2_LAT_REG);
    volatile uint32_t piPwdReg = io_read(PI_BSD_DOM2_PWD_REG);
    volatile uint32_t piPgsReg = io_read(PI_BSD_DOM2_PGS_REG);
    volatile uint32_t piRlsReg = io_read(PI_BSD_DOM2_RLS_REG);
    dma_wait(); // Make sure the libdragon Async io methods have been performed.

    // Set slow timings
    io_write(PI_BSD_DOM2_PGS_REG, 0x0D);
    io_write(PI_BSD_DOM2_RLS_REG, 0x02);
    io_write(PI_BSD_DOM2_LAT_REG, 0x05);
    io_write(PI_BSD_DOM2_PWD_REG, 0x0C);
    dma_wait(); // Make sure the libdragon Async io methods have been performed.

    // Write SRAM
    ed64_ll_dma_write(ram, SRAM_ADDRESS + address, length);

    // Restore original timings
    io_write(PI_BSD_DOM2_LAT_REG, piLatReg);
    io_write(PI_BSD_DOM2_PWD_REG, piPwdReg);
    io_write(PI_BSD_DOM2_PGS_REG, piPgsReg);
    io_write(PI_BSD_DOM2_RLS_REG, piRlsReg);
    dma_wait(); // Make sure the libdragon Async io methods have been performed.
}

// uint16_t ed64_ll_msg_rd() {

//     return ed64_ll_reg_read(REG_MSG);
// }

// void ed64_ll_msg_wr(uint16_t val) {

//     ed64_ll_reg_write(REG_MSG, val);
// }

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

void ed64_ll_dma_pi_read(void * ram_address, unsigned long pi_address, unsigned long length) {

    disable_interrupts();
    dma_wait();
    io_write(PI_STATUS_REG, 3);
    PI_regs->ram_address = ram_address;
    PI_regs->pi_address = pi_address & 0x1FFFFFFF;
    PI_regs->write_length = length - 1;
    dma_wait();

    enable_interrupts();
}

void ed64_ll_dma_pi_write(void * ram_address, unsigned long pi_address, unsigned long length) {

    disable_interrupts();
    dma_wait();
    io_write(PI_STATUS_REG, 3);
    PI_regs->ram_address = ram_address;
    PI_regs->pi_address = pi_address & 0x1FFFFFFF;
    PI_regs->read_length = length - 1;
    dma_wait();
    enable_interrupts();
}

void ed64_ll_dma_read(void *ram, uint32_t address, uint32_t length) {


    if (((uint32_t) ram & 0xF0000000) == 0x80000000) {
        data_cache_hit_writeback_invalidate(ram, length);
        ed64_ll_dma_pi_read(ram, address, length);
    } else {
        ed64_ll_dma_pi_read(ram, address, length);
    }

}

void ed64_ll_dma_write(void *ram, uint32_t address, uint32_t length) {

    if (((uint32_t) ram & 0xF0000000) == 0x80000000)data_cache_hit_writeback(ram, length);
    ed64_ll_dma_pi_write(ram, address, length);
}

/** @brief Get the current FPGA version */
uint16_t ed64_ll_get_fpga_version() {

    return ed64_ll_reg_read(REG_FPGA_VERSION);
}

/** @brief Get the current CPLD version */
uint16_t ed64_ll_get_cpld_version() {

    uint16_t cpld_version;
    uint16_t cfg = ed64_ll_reg_read(REG_CFG);

    ed64_ll_reg_write(REG_CFG, 0);
    cpld_version = ed64_ll_reg_read(REG_FPGA_FW_VER);
    ed64_ll_reg_write(REG_CFG, cfg);

    return cpld_version;
}

/* Load the specified FPGA firmware */
void ed64_ll_load_firmware(uint8_t *firmware) {

    uint32_t i;
    uint16_t f_ctr = 0;
    uint16_t cfg = ed64_ll_reg_read(REG_CFG);

    cfg &= ~ED_CFG_SDRAM_ON;
    ed64_ll_reg_write(REG_CFG, cfg);

    ed64_ll_reg_write(REG_CFG_COUNT, 0);
    wait_ms(10);
    ed64_ll_reg_write(REG_CFG_COUNT, 1);
    wait_ms(10);

    i = 0;
    for (;;) {

        ed64_ll_reg_write(REG_CFG_DATA, *(uint16_t *) & firmware[i]);
        while ((ed64_ll_reg_read(REG_CFG_COUNT) & 8) != 0);

        f_ctr = firmware[i++] == 0xff ? f_ctr + 1 : 0;
        if (f_ctr >= 47)break;
        f_ctr = firmware[i++] == 0xff ? f_ctr + 1 : 0;
        if (f_ctr >= 47)break;
    }


    while ((ed64_ll_reg_read(REG_CFG_COUNT) & 4) == 0) {
        ed64_ll_reg_write(REG_CFG_DATA, 0xffff);
        while ((ed64_ll_reg_read(REG_CFG_COUNT) & 8) != 0);
    }


    wait_ms(20);
}


void ed64_ll_lock_regs() {
    ed64_ll_reg_write(REG_KEY, ED64_KEY_LOCK);
}

void ed64_ll_unlock_regs() {
    ed64_ll_reg_write(REG_KEY, ED64_KEY_UNLOCK);
}


/* GPIO functions */

// /* Set GPIO mode RTC */
// void ed64_ll_gpio_mode_rtc() {

//     uint16_t cfg = ed64_ll_reg_read(REG_CFG);
//     cfg &= ~ED_CFG_GPIO_ON;
//     cfg |= ED_CFG_RTC_ON;
//     ed64_ll_reg_write(REG_CFG, cfg);
// }

// /* Set GPIO mode ON */
// void ed64_ll_gpio_mode_io() {

//     uint16_t cfg = ed64_ll_reg_read(REG_CFG);
//     cfg |= ED_CFG_GPIO_ON;
//     ed64_ll_reg_write(REG_CFG, cfg);
// }

// /* Set GPIO mode OFF */
// void ed64_ll_gpio_mode_off() {

//     uint16_t cfg = ed64_ll_reg_read(REG_CFG);
//     cfg &= ~ED_CFG_GPIO_ON;
//     ed64_ll_reg_write(REG_CFG, cfg);
// }

// /* GPIO mode write */
// void ed64_ll_gpio_write(uint8_t data) {

//     ed64_ll_reg_write(REG_GPIO, data);
// }

// /* GPIO mode read */
// uint8_t ed64_ll_gpio_read() {

//     return ed64_ll_reg_read(REG_GPIO);
// }



// /* 64DD cart conversion save functions */

// /* Set 64DD ON and Enabled?! */
// void ed64_ll_64dd_ram_oe() {

//     uint16_t cfg = ed64_ll_reg_read(REG_CFG);
//     cfg &= ~ED_CFG_64DD_WRITE_ENABLED;
//     cfg |= ED_CFG_64DD_ON;
//     ed64_ll_reg_write(REG_CFG, cfg);
// }

// /* Set 64DD Write Enable?? */
// void ed64_ll_64dd_ram_we() {

//     uint16_t cfg = ed64_ll_reg_read(REG_CFG);
//     cfg |= ED_CFG_64DD_ON | ED_CFG_64DD_WRITE_ENABLED;
//     ed64_ll_reg_write(REG_CFG, cfg);
// }

// /* Set 64DD Disabled?? */
// void ed64_ll_64dd_ram_off() {

//     uint16_t cfg = ed64_ll_reg_read(REG_CFG);
//     cfg &= ~(ED_CFG_64DD_ON | ED_CFG_64DD_WRITE_ENABLED);
//     ed64_ll_reg_write(REG_CFG, cfg);
// }

// /* 64DD Save RAM Clear */
// void ed64_ll_64dd_ram_clear() {

//     uint16_t cfg = ed64_ll_reg_read(REG_CFG);
//     cfg |= ED_CFG_64DD_WRITE_ENABLED;
//     cfg &= ~ED_CFG_64DD_ON;
//     ed64_ll_reg_write(REG_CFG, cfg);
//     wait_ms(100);
// }

// /* 64DD Allowed on this cart?! */
// uint8_t ed64_ll_get_64dd_ram_supported() {

//     return (ed64_ll_reg_read(REG_STATUS) >> 15) & 1;

// }
