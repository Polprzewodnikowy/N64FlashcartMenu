/**
 * @file flashcart.h
 * @brief ED64 V-Series Flashcart Internal Utilities
 * @ingroup flashcart 
 */

#ifndef FLASHCART_ED64_LL_H__
#define FLASHCART_ED64_LL_H__

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// FIXME: redefined because its in a .c instead of a .h
#define PI_BASE_REG		    0x04600000
///////////////////////////////////////////////////////

#define PIF_RAM_START		0x1FC007C0
#define	PI_STATUS_ERROR		0x04
#define	PI_STATUS_IO_BUSY	0x02
#define	PI_STATUS_DMA_BUSY	0x01

#define PI_STATUS_REG		(PI_BASE_REG+0x10)
#define PI_DRAM_ADDR_REG	(PI_BASE_REG+0x00)	/* DRAM address */
#define PI_CART_ADDR_REG	(PI_BASE_REG+0x04)
#define PI_RD_LEN_REG		(PI_BASE_REG+0x08)
#define PI_WR_LEN_REG		(PI_BASE_REG+0x0C)

// FIXME: redefined because its in a .c instead of a .h
#define PI_BSD_DOM1_LAT_REG     (PI_BASE_REG+0x14)
#define PI_BSD_DOM1_PWD_REG     (PI_BASE_REG+0x18)
#define PI_BSD_DOM1_PGS_REG     (PI_BASE_REG+0x1C)
#define PI_BSD_DOM1_RLS_REG     (PI_BASE_REG+0x20)
#define PI_BSD_DOM2_LAT_REG     (PI_BASE_REG+0x24)
#define PI_BSD_DOM2_PWD_REG     (PI_BASE_REG+0x28)
#define PI_BSD_DOM2_PGS_REG     (PI_BASE_REG+0x2C)
#define PI_BSD_DOM2_RLS_REG     (PI_BASE_REG+0x30)
///////////////////////////////////////////////////////

#define	PHYS_TO_K0(x)	((unsigned long)(x)|0x80000000)	/* physical to kseg0 */
#define	K0_TO_PHYS(x)	((unsigned long)(x)&0x1FFFFFFF)	/* kseg0 to physical */
#define	PHYS_TO_K1(x)	((unsigned long)(x)|0xA0000000)	/* physical to kseg1 */
#define	K1_TO_PHYS(x)	((unsigned long)(x)&0x1FFFFFFF)	/* kseg1 to physical */



/**
 * @addtogroup ed64
 * @{
 */


typedef enum {
    SAVE_TYPE_NONE = 0,
    SAVE_TYPE_SRAM = 1,
    SAVE_TYPE_SRAM_128K = 2,
    SAVE_TYPE_EEPROM_4K = 3,
    SAVE_TYPE_EEPROM_16K = 4,
    SAVE_TYPE_FLASHRAM = 5,
    SAVE_TYPE_CPAK = 8,
    SAVE_TYPE_DD64 = 16,
} ed64_save_type_t;

#define ROM_ADDRESS  (0xB0000000)

/* Save functions */
void ed64_ll_set_ram_bank(uint8_t bank);
ed64_save_type_t ed64_ll_get_save_type();
void ed64_ll_set_save_type(ed64_save_type_t type);

void ed64_ll_set_sdcard_timing(void);

int ed64_ll_get_sram( uint8_t *buffer, int size);
int ed64_ll_get_eeprom(  uint8_t *buffer, int size);
int ed64_ll_get_fram(  uint8_t *buffer, int size);

int ed64_ll_set_sram( uint8_t *buffer, int size);
int ed64_ll_set_eeprom( uint8_t *buffer, int size);
int ed64_ll_set_fram( uint8_t *buffer, int size);

/** @} */ /* ed64 */
#endif
