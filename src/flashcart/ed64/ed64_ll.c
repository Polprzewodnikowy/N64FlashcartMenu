#include <libdragon.h>

#include "../flashcart_utils.h"
#include "ed64_ll.h"

// NOTE: these are a near replica of libcart cart.c defines and functions!

#define PI_BASE_REG         (0x04600000)
#define PI_STATUS_REG       (PI_BASE_REG+0x10)

#define PI_BSD_DOM1_LAT_REG (PI_BASE_REG+0x14)

/* PI dom1 pulse width (R/W): [7:0] domain 1 device R/W strobe pulse width */
#define PI_BSD_DOM1_PWD_REG (PI_BASE_REG+0x18)

/* PI dom1 page size (R/W): [3:0] domain 1 device page size */
#define PI_BSD_DOM1_PGS_REG (PI_BASE_REG+0x1C)    /*   page size */

/* PI dom1 release (R/W): [1:0] domain 1 device R/W release duration */
#define PI_BSD_DOM1_RLS_REG (PI_BASE_REG+0x20)

/* PI dom2 latency (R/W): [7:0] domain 2 device latency */
#define PI_BSD_DOM2_LAT_REG (PI_BASE_REG+0x24)    /* Domain 2 latency */

/* PI dom2 pulse width (R/W): [7:0] domain 2 device R/W strobe pulse width */
#define PI_BSD_DOM2_PWD_REG (PI_BASE_REG+0x28)    /*   pulse width */

/* PI dom2 page size (R/W): [3:0] domain 2 device page size */
#define PI_BSD_DOM2_PGS_REG (PI_BASE_REG+0x2C)    /*   page size */

/* PI dom2 release (R/W): [1:0] domain 2 device R/W release duration */
#define PI_BSD_DOM2_RLS_REG (PI_BASE_REG+0x30)    /*   release duration */

#define	IO_READ(addr)       (*(volatile uint32_t *)PHYS_TO_K1(addr))
#define	IO_WRITE(addr,data) (*(volatile uint32_t *)PHYS_TO_K1(addr)=(uint32_t)(data))

#define	PHYS_TO_K1(x)       ((uint32_t)(x)|0xA0000000)	/* physical to kseg1 */

#define CART_ABORT()        {__cart_acs_rel(); return -1;}

/* Temporary buffer aligned for DMA */
//__attribute__((aligned(16))) static uint64_t __cart_buf[512/8];

static uint32_t __cart_dom1_rel;
static uint32_t __cart_dom2_rel;
uint32_t cart_dom1 = 0x8030FFFF; // FIXME: Added as should already be initialized.
uint32_t cart_dom2 = 0x80370404; // FIXME: Added as should already be initialized.

//uint32_t cart_size;

static void __cart_acs_get(void)
{
    /* Save PI BSD configuration and reconfigure */
    if (cart_dom1)
    {
        __cart_dom1_rel =
            IO_READ(PI_BSD_DOM1_LAT_REG) <<  0 |
            IO_READ(PI_BSD_DOM1_PWD_REG) <<  8 |
            IO_READ(PI_BSD_DOM1_PGS_REG) << 16 |
            IO_READ(PI_BSD_DOM1_RLS_REG) << 20 |
            1 << 31;
        IO_WRITE(PI_BSD_DOM1_LAT_REG, cart_dom1 >>  0);
        IO_WRITE(PI_BSD_DOM1_PWD_REG, cart_dom1 >>  8);
        IO_WRITE(PI_BSD_DOM1_PGS_REG, cart_dom1 >> 16);
        IO_WRITE(PI_BSD_DOM1_RLS_REG, cart_dom1 >> 20);
    }
    if (cart_dom2)
    {
        __cart_dom2_rel =
            IO_READ(PI_BSD_DOM2_LAT_REG) <<  0 |
            IO_READ(PI_BSD_DOM2_PWD_REG) <<  8 |
            IO_READ(PI_BSD_DOM2_PGS_REG) << 16 |
            IO_READ(PI_BSD_DOM2_RLS_REG) << 20 |
            1 << 31;
        IO_WRITE(PI_BSD_DOM2_LAT_REG, cart_dom2 >>  0);
        IO_WRITE(PI_BSD_DOM2_PWD_REG, cart_dom2 >>  8);
        IO_WRITE(PI_BSD_DOM2_PGS_REG, cart_dom2 >> 16);
        IO_WRITE(PI_BSD_DOM2_RLS_REG, cart_dom2 >> 20);
    }
}

static void __cart_acs_rel(void)
{
    /* Restore PI BSD configuration */
    if (__cart_dom1_rel)
    {
        IO_WRITE(PI_BSD_DOM1_LAT_REG, __cart_dom1_rel >>  0);
        IO_WRITE(PI_BSD_DOM1_PWD_REG, __cart_dom1_rel >>  8);
        IO_WRITE(PI_BSD_DOM1_PGS_REG, __cart_dom1_rel >> 16);
        IO_WRITE(PI_BSD_DOM1_RLS_REG, __cart_dom1_rel >> 20);
        __cart_dom1_rel = 0;
    }
    if (__cart_dom2_rel)
    {
        IO_WRITE(PI_BSD_DOM2_LAT_REG, __cart_dom2_rel >>  0);
        IO_WRITE(PI_BSD_DOM2_PWD_REG, __cart_dom2_rel >>  8);
        IO_WRITE(PI_BSD_DOM2_PGS_REG, __cart_dom2_rel >> 16);
        IO_WRITE(PI_BSD_DOM2_RLS_REG, __cart_dom2_rel >> 20);
        __cart_dom2_rel = 0;
    }
}

static void __cart_dma_rd(void *dram, uint32_t cart, uint32_t size)
{
    data_cache_hit_writeback_invalidate(dram, size);
    dma_read_raw_async(dram, cart, size);
    dma_wait();
}

static void __cart_dma_wr(const void *dram, uint32_t cart, uint32_t size)
{
    data_cache_hit_writeback((void *)dram, size);
    dma_write_raw_async(dram, cart, size);
    dma_wait();
}

/* ED64 registers base address  */
#define ED_BASE_REG             (0xA8040000) // FIXME: Or should it be `0x08040000` like libcart?

/* ED64 config register */
#define ED_CFG_REG              (ED_BASE_REG+0x00)
/* ED64 status register */
#define ED_STATUS_REG           (ED_BASE_REG+0x04)
/* ED64 DMA Length register */
#define ED_DMA_LEN_REG          (ED_BASE_REG+0x08)
/* ED64 DMA Address register */
#define ED_DMA_ADDR_REG         (ED_BASE_REG+0x0C)
/* ED64 Message/Data register */
#define ED_MSG_REG              (ED_BASE_REG+0x10)
/* ED64 DMA Config register */
#define ED_DMA_CFG_REG          (ED_BASE_REG+0x14)
/* ED64 SPI register */
#define ED_SPI_REG              (ED_BASE_REG+0x18)
/* ED64 SPI Config register */
#define ED_SPI_CFG_REG          (ED_BASE_REG+0x1C)
/* ED64 Unlock register */
#define ED_KEY_REG              (ED_BASE_REG+0x20)
/* ED64 Save Config register */
#define ED_SAV_CFG_REG          (ED_BASE_REG+0x24)
#define ED_SEC_REG              (ED_BASE_REG+0x28) /* Sectors?? */
#define ED_VER_REG              (ED_BASE_REG+0x2C) /* Hardware series version + firmware rev */
/* ED64 GPIO register */
#define ED_GPIO_REG             (ED_BASE_REG+0x30)

// Unknown regs (if any)

/* ED64 Config Count register */
#define ED_CFG_CNT_REG          (ED_BASE_REG+0x40)
/* ED64 Config Data register */
#define ED_CFG_DAT_REG          (ED_BASE_REG+0x44)
#define ED_CPLD_MSG_REG         (ED_BASE_REG+0x48) // NOTE: was `ED_MAX_MSG_REG` /* Altera (Intel) MAX */
#define ED_CPLD_VER_REG         (ED_BASE_REG+0x4C) // NOTE: Was `ED_CRC_REG`
/* ED64 FlashRAM Address register (V3 series only) */
#define ED_FLASHRAM_ADDR_REG    (ED_BASE_REG+0x50)
/* ED64 FlashRAM Message/Data register (V3 series only) */
#define ED_FLASHRAM_MSG_REG     (ED_BASE_REG+0x54)

// END NOTE.


/* Locks the ED64 registers  */
#define ED64_KEY_LOCK   0x0000
/* Unlocks the ED64 registers  */
#define ED64_KEY_UNLOCK 0x1234

// #define ED_STATE_DMA_BUSY       (1 << 0)
// #define ED_STATE_DMA_TOUT       (1 << 1)
// #define ED_STATE_TXE            (1 << 2)
// #define ED_STATE_RXF            (1 << 3)
// #define ED_STATE_SPI            (1 << 4)
// typedef enum {
//     STATE_DMA_BUSY = 0x01,
//     STATE_DMA_TOUT = 0x02,
//     STATE_USB_TXE = 0x04,
//     STATE_USB_RXF = 0x08,
//     STATE_SPI = 0x10,

// } ed64_dma_state_t;

// #define ED_DMA_SD_TO_RAM        1
// #define ED_DMA_RAM_TO_SD        2
// #define ED_DMA_FIFO_TO_RAM      3 // USB
// #define ED_DMA_RAM_TO_FIFO      4 // USB

// #define ED_SAV_EEP_OFF          (0 << 0)
// #define ED_SAV_EEP_ON           (1 << 0)
// #define ED_SAV_SRM_OFF          (0 << 1)
// #define ED_SAV_SRM_ON           (1 << 1)
// #define ED_SAV_EEP_SMALL        (0 << 2)
// #define ED_SAV_EEP_BIG          (1 << 2)
// #define ED_SAV_SRM_SMALL        (0 << 3)
// #define ED_SAV_SRM_BIG          (1 << 3)
#define SAV_EEP_ON 1
#define SAV_SRM_ON 2
#define SAV_EEP_BIG 4
#define SAV_SRM_BIG 8

#define SAV_RAM_BANK 128
#define SAV_RAM_BANK_APPLY 32768


// #define ED_CFG_SDRAM_OFF        (0 << 0)
// #define ED_CFG_SDRAM_ON         (1 << 0)
// #define ED_CFG_BYTESWAP_OFF     (0 << 1)
// #define ED_CFG_BYTESWAP_ON      (1 << 1)
typedef enum {
    ED_CFG_SDRAM_ON = 0x01,
    ED_CFG_BYTESWAP_ON = 0x02,
    ED_CFG_WRITE_MODE_ON = 0x04,
    ED_CFG_WRITE_ADDR_MASK = 0x08,
    ED_CFG_RTC_ON = 0x20, //32,
    ED_CFG_GPIO_ON = 0x60, //96,
    ED_CFG_64DD_ON = 0x0100, //256,
    ED_CFG_64DD_WRITE_ENABLED = 0x0200, //512,
} ed64_config_t;


// #define FPGA_FW_DATA_SKIP_FW_INIT   (1 << 8)
// #define FPGA_FW_DATA_SKIP_TV_INIT   (1 << 9)
// #define FPGA_FW_DATA_TV_TYPE1       (1 << 10)
// #define FPGA_FW_DATA_TV_TYPE2       (1 << 11)
// #define FPGA_FW_DATA_SKIP_SD_INIT   (1 << 12)
// #define FPGA_FW_DATA_SD_TYPE        (1 << 13)
// #define FPGA_FW_DATA_HOT_START      (1 << 14)


// static uint16_t spi_cfg;
uint8_t ed64_ll_sram_bank;
ed64_save_type_t ed64_ll_save_type;

// #define REG_LAT 0x04
// #define REG_PWD 0x04

// #define ROM_LAT 0x40
// #define ROM_PWD 0x12

/* register functions (dependent on flashcart version) */

/* register functions for V2 & V2.5  (These carts do not support FLASHRAM)*/
/* The end of SDRAM is used for SRAM or FlashRAM save types */
// void ed64_ll_io_reg_v2(uint32_t address, uint32_t data) {

//     *(volatile uint32_t *) (ROM_ADDRESS);
//     *(volatile uint32_t *) (ROM_ADDRESS + address) = data;
// }

// /* register functions for V3 */
// void ed64_ll_io_reg_v3(uint32_t address, uint16_t data) {

//     io_write(ED_FLASHRAM_ADDR_REG, address);
//     io_write(ED_FLASHRAM_MSG_REG, data);
// }

/* initialize functions (dependent on flashcart version) */

// /* Initilize V2 & 2.5 cart */
// void ed64_ll_init_v2() {

//     uint8_t buff[512];
//     uint16_t cfg = io_read(ED_CFG_REG);

//     io_write(ED_CFG_REG, 0);
//     ed64_ll_io_reg_v2(0xaa / 4 * 4, 0x00980098);
//     ed64_ll_dma_read_rom(buff, 0, 1);
//     ed64_ll_io_reg_v2(0xaa / 4 * 4, 0x00f000f0);
//     ed64_ll_dma_read_rom(buff, 0, 1);
//     io_write(ED_CFG_REG, cfg);

// }

// /* Initilize V3 cart */
// void ed64_ll_init_v3() {

//     uint8_t buff[1024];
//     uint16_t cfg = ed64_ll_reg_read(ED_CFG_REG);

//     io_write(ED_CFG_REG, 0);
//     io_write(ED_CFG_CNT_REG, 161);
//     ed64_ll_io_reg_v3(0x55, 0x98);
//     ed64_ll_dma_read_rom(buff, 0, 2);
//     ed64_ll_io_reg_v3(0x55, 0xF0);
//     ed64_ll_dma_read_rom(buff, 0, 2);
//     ed64_ll_dma_read_rom(buff, 1024, 2);
//     ed64_ll_dma_read_rom(buff, 1024 + 256 - 2, 2);
//     io_write(ED_CFG_CNT_REG, 1);

//     io_write(ED_CFG_REG, cfg);
// }

// /* Initialize cart */
// int ed64_ll_init() {

//     uint16_t firmware_ver;
//     uint16_t firmware_msg;
//     uint8_t cold_start;

//     // TODO: take into account libCart!
//     io_write(PI_BSD_DOM2_LAT_REG, REG_LAT);
//     io_write(PI_BSD_DOM2_PWD_REG, REG_PWD);
//     dma_wait(); // Make sure the libdragon Async io methods have been performed.


//     ed64_ll_reg_write(REG_KEY, ED64_KEY_UNLOCK);
//     ed64_ll_reg_write(REG_CFG, 0x0000);


//     firmware_msg = ed64_ll_reg_read(REG_FPGA_FW_DATA);
//     cold_start = (firmware_msg & FPGA_FW_DATA_HOT_START) == 0 ? 1 : 0;
//     if (cold_start) {
//         firmware_msg |= FPGA_FW_DATA_HOT_START;
//         ed64_ll_reg_write(REG_FPGA_FW_DATA, firmware_msg);
//     }

//     firmware_ver = ed64_ll_reg_read(REG_FPGA_FW_VER);
//     if ((firmware_ver & 0xf000) >= 0x2000) {
//         ed64_ll_init_v3();
//     } else {
//         ed64_ll_init_v2();
//     }



//     //spi_cfg = SPI_CFG_SS | BI_SPI_SPD_LO;
//     ed64_ll_reg_write(REG_CFG, ED_CFG_SDRAM_ON);
//     //ed64_ll_reg_write(REG_SPI_CFG, spi_cfg);
//     ed64_ll_save_type = SAVE_TYPE_NONE;


//     return cold_start;
// }

// void ed64_ll_reset_spx() {

//     uint16_t cfg = io_read(ED_CFG_REG);

//     io_write(ED_CFG_REG, 0x8000);
//     wait_ms(100);
//     io_write(ED_CFG_REG, cfg);
//     wait_ms(100);
// }


// /*  Used for USB and SPI functions */
// uint8_t ed64_ll_dma_busy() {

//     while ((io_read(ED_STATUS_REG) & STATE_DMA_BUSY) != 0);
//     return io_read(ED_STATUS_REG) & STATE_DMA_TOUT;
// }


// /* USB functions */

// /* USB read is busy */
// uint8_t ed64_ll_usb_read_busy() {

//     return io_read(ED_STATUS_REG) & STATE_USB_RXF;
// }

// /* USB write is busy */
// uint8_t ed64_ll_usb_write_busy() {

//     return io_read(ED_STATUS_REG) & STATE_USB_TXE;
// }

// /* USB read */
// uint8_t ed64_ll_usb_read(uint32_t start_address, uint32_t slen) {

//     start_address /= 4;
//     while (ed64_ll_usb_read_busy() != 0);

//     io_write(ED_DMA_LEN_REG, slen - 1);
//     io_write(ED_DMA_ADDR_REG, start_address);
//     io_write(ED_DMA_CFG_REG, ED_DMA_FIFO_TO_RAM);

//     if (ed64_ll_dma_busy() != 0)return USB_ERROR_FIFO_TIMEOUT;

//     return 0;
// }

// /* USB write */
// uint8_t ed64_ll_usb_write(uint32_t start_address, uint32_t slen) {

//     start_address /= 4;
//     while (ed64_ll_usb_write_busy() != 0);

//     io_write(ED_DMA_LEN_REG, slen - 1);
//     io_write(ED_DMA_ADDR_REG, start_address);
//     io_write(ED_DMA_CFG_REG, ED_DMA_RAM_TO_FIFO);

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
    ed64_ll_save_type = type; // So we can retrive it later with `ed64_ll_get_save_type`
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
    if (eeprom_size_16k)save_cfg |= SAV_EEP_BIG;
    if (ram_size_large)save_cfg |= SAV_SRM_BIG;
    if (ram_bank)save_cfg |= SAV_RAM_BANK;
    save_cfg |= SAV_RAM_BANK_APPLY;

    __cart_acs_get();
    io_write(ED_SAV_CFG_REG, save_cfg);
    __cart_acs_rel();
}

void ed64_ll_set_sram_bank(uint8_t bank) {

    ed64_ll_sram_bank = bank == 0 ? 0 : 1;

}

// /* reads metadata related to the manufacturing date and cart capabilities */
// ed64_cart_metadata_t ed64_ll_read_cart_metadata(void *dest) {

//     ed64_cart_metadata_t cart_info;
//     __cart_acs_get();
    
//     uint16_t cfg = io_read(ED_CFG_REG);

//     cfg &= ~ED_CFG_SDRAM_ON;
//     io_write(ED_CFG_REG, cfg);

//     ed64_ll_dma_read_rom(dest, 0, 1);

//     // cart_info.production_date = (buff[0x38] << 8) | buff[0x39];
//     // cart_info.production_time = (buff[0x3A] << 8) | buff[0x3B];
//     // cart_info.serial_number = (buff[0x3C] << 8) | buff[0x3D];
//     // cart_info.cic_6105_compatible = buff[0x40] == 0x03 ? true : false; // CIC_6105 : CIC_6102;


//     cfg |= ED_CFG_SDRAM_ON;
//     io_write(ED_CFG_REG, cfg);
//     __cart_acs_rel();
//     return cart_info;
// }

void ed64_ll_dma_read_rom(void *ram, uint32_t address_offset, uint32_t length) {
    __cart_acs_get();
    __cart_dma_rd(ram, ROM_ADDRESS + address_offset, length);
    __cart_acs_rel();

}

void ed64_ll_dma_write_rom(void *ram, uint32_t address_offset, uint32_t length) {
    __cart_acs_get();
    __cart_dma_wr(ram, ROM_ADDRESS + address_offset, length);
    __cart_acs_rel();

}

/* Read from SRAM over DMA */
void ed64_ll_dma_read_sram(void *ram, uint32_t address_offset, uint32_t length) {
    __cart_acs_get();

    // Read SRAM
    __cart_dma_rd(ram, SRAM_ADDRESS + address_offset, length);

    __cart_acs_rel();
}

/* Write to SRAM over DMA */
void ed64_ll_dma_write_sram(void *ram, uint32_t address_offset, uint32_t length) {
    __cart_acs_get();

    // Write SRAM
    __cart_dma_wr(ram, SRAM_ADDRESS + address_offset, length);

    __cart_acs_rel();
}

/** @brief Get the current FPGA version */
uint16_t ed64_ll_get_fpga_version() {
    __cart_acs_get();
    return io_read(ED_VER_REG);
    __cart_acs_rel();
}

/** @brief Get the current CPLD version */
uint16_t ed64_ll_get_cpld_version() {
    __cart_acs_get();
    uint16_t cpld_version;
    uint16_t cfg = io_read(ED_CFG_REG);

    io_write(ED_CFG_REG, 0);
    cpld_version = io_read(ED_CPLD_VER_REG);
    io_write(ED_CFG_REG, cfg);
    __cart_acs_rel();
    return cpld_version;
}

/** @brief Load the specified FPGA firmware */
void ed64_ll_load_firmware(uint8_t *firmware) {
    __cart_acs_get();
    uint32_t i = 0;
    uint16_t f_ctr = 0;
    uint16_t cfg = io_read(ED_CFG_REG);

    cfg &= ~ED_CFG_SDRAM_ON;
    io_write(ED_CFG_REG, cfg);
    wait_ms(10);
    io_write(ED_CFG_CNT_REG, 0);
    wait_ms(10);
    io_write(ED_CFG_CNT_REG, 1);
    wait_ms(10);

    for (;;) {
        io_write(ED_CFG_DAT_REG, *(uint16_t *) & firmware[i]);
        while ((io_read(ED_CFG_CNT_REG) & 8) != 0);

        f_ctr = firmware[i++] == 0xff ? f_ctr + 1 : 0;
        if (f_ctr >= 47) break;
        f_ctr = firmware[i++] == 0xff ? f_ctr + 1 : 0;
        if (f_ctr >= 47) break;
    }

    while ((io_read(ED_CFG_CNT_REG) & 4) == 0) {
        io_write(ED_CFG_DAT_REG, 0xffff);
        while ((io_read(ED_CFG_CNT_REG) & 8) != 0);
    }

    wait_ms(20);
    __cart_acs_rel();
}


void ed64_ll_lock_regs() {
    __cart_acs_get();
    io_write(ED_KEY_REG, ED64_KEY_LOCK);
    __cart_acs_rel();
}

void ed64_ll_unlock_regs() {
    __cart_acs_get();
    io_write(ED_KEY_REG, ED64_KEY_UNLOCK);
    __cart_acs_rel();
}


/* GPIO functions */

/** @brief Set GPIO mode RTC */
void ed64_ll_gpio_mode_rtc() {
    __cart_acs_get();
    uint16_t cfg = io_read(ED_CFG_REG);
    cfg &= ~ED_CFG_GPIO_ON;
    cfg |= ED_CFG_RTC_ON;
    io_write(ED_CFG_REG, cfg);
    __cart_acs_rel();
}

/** @brief Set GPIO mode ON */
void ed64_ll_gpio_mode_io() {
    __cart_acs_get();
    uint16_t cfg = io_read(ED_CFG_REG);
    cfg |= ED_CFG_GPIO_ON;
    io_write(ED_CFG_REG, cfg);
    __cart_acs_rel();
}

/** @brief Set GPIO mode OFF */
void ed64_ll_gpio_mode_off() {
    __cart_acs_get();
    uint16_t cfg = io_read(ED_CFG_REG);
    cfg &= ~ED_CFG_GPIO_ON;
    io_write(ED_CFG_REG, cfg);
    __cart_acs_rel();
}

/** @brief Set GPIO mode write */
void ed64_ll_gpio_write(uint8_t data) {
    __cart_acs_get();
    io_write(ED_GPIO_REG, data);
    __cart_acs_rel();
}

/** @brief Set GPIO mode read */
uint8_t ed64_ll_gpio_read() {
    __cart_acs_get();
    uint8_t gpio_state = io_read(ED_GPIO_REG);
    __cart_acs_rel();
    return gpio_state;
}



/* 64DD cart conversion save functions */

/* Set 64DD ON and Enabled?! */
void ed64_ll_64dd_ram_oe() {
    __cart_acs_get();
    uint16_t cfg = io_read(ED_CFG_REG);
    cfg &= ~ED_CFG_64DD_WRITE_ENABLED;
    cfg |= ED_CFG_64DD_ON;
    io_write(ED_CFG_REG, cfg);
    __cart_acs_rel();
}

/* Set 64DD Write Enable?? */
void ed64_ll_64dd_ram_we() {
    __cart_acs_get();
    uint16_t cfg = io_read(ED_CFG_REG);
    cfg |= ED_CFG_64DD_ON | ED_CFG_64DD_WRITE_ENABLED;
    io_write(ED_CFG_REG, cfg);
    __cart_acs_rel();
}

/* Set 64DD Disabled?? */
void ed64_ll_64dd_ram_off() {
    __cart_acs_get();
    uint16_t cfg = io_read(ED_CFG_REG);
    cfg &= ~(ED_CFG_64DD_ON | ED_CFG_64DD_WRITE_ENABLED);
    io_write(ED_CFG_REG, cfg);
    __cart_acs_rel();
}

/* 64DD Save RAM Clear */
void ed64_ll_64dd_ram_clear() {
    __cart_acs_get();
    uint16_t cfg = io_read(ED_CFG_REG);
    cfg |= ED_CFG_64DD_WRITE_ENABLED;
    cfg &= ~ED_CFG_64DD_ON;
    io_write(ED_CFG_REG, cfg);
    wait_ms(100);
    __cart_acs_rel();
}

/** @brief Check if 64DD Cart conversions allowed on this cart?! */
uint8_t ed64_ll_get_64dd_ram_supported() {
    __cart_acs_get();
    return (io_read(ED_STATUS_REG) >> 15) & 1;
    __cart_acs_rel();
}
