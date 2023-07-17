/**
 * @file flashcart.h
 * @brief ED64 Flashcart Internal Utilities
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

#define SAVE_TYPE_OFF 0
#define SAVE_TYPE_SRAM 1
#define SAVE_TYPE_SRAM128 2
#define SAVE_TYPE_EEP4k 3
#define SAVE_TYPE_EEP16k 4
#define SAVE_TYPE_FLASH 5
#define SAVE_TYPE_MPAK 8
#define SAVE_TYPE_DD64 16

#define NON_CACHE_RAM 0x20000000

#define ROM_LEN    0x4000000
#define ROM_ADDR  0xB0000000
#define ROM_BUFF (ROM_LEN / 512 - 4)
#define SRAM_ADDR 0xA8000000

#define EVD_ERROR_FIFO_TIMEOUT 0x90
#define EVD_ERROR_MMC_TIMEOUT 0x91

#define BOOT_UPD_ERR_WRONG_SIZE 0x95
#define BOOT_UPD_ERR_HDR 0x96
#define BOOT_UPD_ERR_CMP 0x97
#define BOOT_UPD_ERR_CIC_DTCT 0x98

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

#define PI_STATUS_REG		(PI_BASE_REG+0x10)
#define PI_BASE_REG		0x04600000

#define BI_SPI_SPD_LO 2
#define BI_SPI_SPD_25 1
#define BI_SPI_SPD_50 0

#define SPI_CFG_SPD0 1
#define SPI_CFG_SPD1 2
#define SPI_CFG_SS 4
#define SPI_CFG_RD 8
#define SPI_CFG_DAT 16
#define SPI_CFG_1BIT 32

#define SPI_MODE_DAT_R1 (SPI_CFG_DAT | SPI_CFG_RD | SPI_CFG_1BIT)
#define SPI_MODE_DAT_R4 (SPI_CFG_DAT | SPI_CFG_RD)
#define SPI_MODE_DAT_W1 (SPI_CFG_DAT | SPI_CFG_1BIT)
#define SPI_MODE_DAT_W4 (SPI_CFG_DAT)

#define SPI_MODE_CMD_R1 (SPI_CFG_RD | SPI_CFG_1BIT)
#define SPI_MODE_CMD_R4 (SPI_CFG_RD)
#define SPI_MODE_CMD_W1 (SPI_CFG_1BIT)
#define SPI_MODE_CMD_W4 (0)

#define DD64_SAV_TBL_LEN 2048
#define DD64_SAV_BLK_SIZ 64

void bi_init_boot(uint8_t *firmware);
uint8_t bi_init();
uint8_t bi_usb_rd_busy();
uint8_t bi_usb_rd(uint32_t saddr, uint32_t slen);
uint8_t bi_usb_wr(uint32_t saddr, uint32_t slen);

uint8_t bi_spi(uint8_t data);
void bi_spi_nr(uint8_t data);
void bi_set_spi_spd(uint16_t speed);
void bi_ss_off();
void bi_ss_on();
void bi_sd_mode(uint16_t mode);
uint8_t bi_spi_read_to_rom(uint32_t saddr, uint16_t slen);
void bi_swap_on();
void bi_swap_off();
uint8_t bi_get_save_type();
void bi_set_save_type(uint8_t type);
uint16_t bi_fpga_ver();
void bi_read_bios(void *dst, uint16_t saddr, uint16_t slen);
uint16_t bi_msg_rd();
void bi_msg_wr(uint16_t val);


void bi_dma_read_rom(void *ram, uint32_t saddr, uint32_t slen);
void bi_dma_write_rom(void *ram, uint32_t saddr, uint32_t slen);
void bi_dma_read_sram(void *ram, uint32_t addr, uint32_t len);
void bi_dma_write_sram(void *ram, uint32_t addr, uint32_t len);
void bi_load_firmware(uint8_t *firm);
void bi_sleep(uint32_t ms);
void bi_lock_regs();
void bi_unlock_regs();
void bi_init_v2();
void bi_init_v3();
uint16_t bi_cpld_ver();
void bi_gpio_mode_rtc();
void bi_gpio_mode_io();
void bi_gpio_off();
uint8_t bi_gpio_rd();
uint32_t bi_reg_rd(uint32_t reg);
void bi_reg_wr(uint32_t reg, uint32_t data);
void bi_reset_spx();

void bi_dd_ram_oe();
void bi_dd_ram_we();
void bi_dd_ram_off();
void bi_dd_ram_clr();
uint8_t bi_dd_ram_supported();


#endif


