/**
 * @file flashcart.h
 * @brief ED64 V-Series Flashcart Internal Utilities
 * @ingroup flashcart 
 */

#ifndef FLASHCART_ED64_INTERNAL_H__
#define FLASHCART_ED64_INTERNAL_H__


#include <stddef.h>
#include <stdint.h>

/**
 * @addtogroup ed64
 * @{
 */


typedef enum {
    SAVE_TYPE_OFF = 0,
    SAVE_TYPE_SRAM = 1,
    SAVE_TYPE_SRAM128 = 2,
    SAVE_TYPE_EEP4k = 3,
    SAVE_TYPE_EEP16k = 4,
    SAVE_TYPE_FLASH = 5,
    SAVE_TYPE_CPAK = 8,
    SAVE_TYPE_DD64 = 16,
} ed64_save_type_t;

#define ROM_ADDR  0xB0000000
#define SRAM_ADDR 0xA8000000

#define EVD_ERROR_FIFO_TIMEOUT 0x90
#define EVD_ERROR_MMC_TIMEOUT 0x91

#define BOOT_UPD_ERR_WRONG_SIZE 0x95
#define BOOT_UPD_ERR_HDR 0x96
#define BOOT_UPD_ERR_CMP 0x97
#define BOOT_UPD_ERR_CIC_DTCT 0x98


#define PI_BASE_REG		    0x04600000
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

#define	PHYS_TO_K1(x)	((uint32_t)(x)|0xA0000000)	/* physical to kseg1 */
#define	IO_WRITE(addr,data)	(*(volatile uint32_t *)PHYS_TO_K1(addr)=(uint32_t)(data))
#define	IO_READ(addr)		(*(volatile uint32_t *)PHYS_TO_K1(addr))

typedef enum {
    BI_SPI_SPD_50 = 0,
    BI_SPI_SPD_25 = 1,
    BI_SPI_SPD_LO = 2,
} ed64_spi_speed_t;

#define SPI_CFG_SPD0 1
#define SPI_CFG_SPD1 2
#define SPI_CFG_SS 4
#define SPI_CFG_RD 8
#define SPI_CFG_DAT 16
#define SPI_CFG_1BIT 32

/* Initialization functions */
uint8_t ed64_bios_init();
void ed64_bios_init_v2();
void ed64_bios_init_v3();

/* Device information functions */
uint16_t ed64_bios_get_fpga_ver();
uint16_t ed64_bios_get_cpld_ver();

/* Firmware update functions */
void ed64_bios_load_firmware(uint8_t *firm);

/* USB functions */
uint8_t ed64_bios_usb_rd_busy();
uint8_t ed64_bios_usb_rd(uint32_t saddr, uint32_t slen);
uint8_t ed64_bios_usb_wr(uint32_t saddr, uint32_t slen);

/* SPI functions */
uint8_t ed64_bios_spi(uint8_t data);
void ed64_bios_spi_nr(uint8_t data);
void ed64_bios_set_spi_spd(uint16_t speed);

/* SD Card functions */
void ed64_bios_sd_mode(uint16_t mode);
uint8_t ed64_bios_spi_read_to_rom(uint32_t saddr, uint16_t slen);
void ed64_bios_swap_on();
void ed64_bios_swap_off();

/* Save functions */
uint8_t ed64_bios_get_save_type();
void ed64_bios_set_save_type(uint8_t type);

void ed64_bios_read_bios(void *dst, uint16_t saddr, uint16_t slen);
uint16_t ed64_bios_msg_rd();
void ed64_bios_msg_wr(uint16_t val);


void ed64_bios_dma_read_rom(void *ram, uint32_t saddr, uint32_t slen);
void ed64_bios_dma_write_rom(void *ram, uint32_t saddr, uint32_t slen);
void ed64_bios_dma_read_sram(void *ram, uint32_t addr, uint32_t len);
void ed64_bios_dma_write_sram(void *ram, uint32_t addr, uint32_t len);

void ed64_bios_lock_regs();
void ed64_bios_unlock_regs();

uint32_t ed64_bios_reg_rd(uint32_t reg);
void ed64_bios_reg_wr(uint32_t reg, uint32_t data);
void ed64_bios_reset_spx();

/* GPIO functions */
void ed64_bios_gpio_mode_rtc();
void ed64_bios_gpio_mode_io();
void ed64_bios_gpio_off();
uint8_t ed64_bios_gpio_rd();

/* 64DD functions */
void ed64_bios_dd_ram_oe();
void ed64_bios_dd_ram_we();
void ed64_bios_dd_ram_off();
void ed64_bios_dd_ram_clr();
uint8_t ed64_bios_dd_ram_supported();

/** @} */ /* ed64 */

#endif


