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

/* ED64 save location base address  */
#define PI_SAVE_ADDR        0xA8000000

// FIXME: redefined because its in a .c instead of a .h
#define PI_BASE_REG		    0x04600000

///////////////////////////////////////////////////////
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
    SAVE_TYPE_NONE = 0,        // No save type
    SAVE_TYPE_SRAM = 1,        // SRAM save type
    SAVE_TYPE_SRAM_128K = 2,   // 128K SRAM save type
    SAVE_TYPE_EEPROM_4K = 3,   // 4K EEPROM save type
    SAVE_TYPE_EEPROM_16K = 4,  // 16K EEPROM save type
    SAVE_TYPE_FLASHRAM = 5,    // FlashRAM save type
    SAVE_TYPE_CPAK = 8,        // Controller Pak save type
    SAVE_TYPE_DD64_CART_PORT = 16, // 64DD cart port save type
} ed64_save_type_t;

#define ROM_ADDRESS  (0xB0000000)

/* Save functions */

/**
 * @brief Initialize SRAM.
 */
void pi_initialize_sram (void);

/**
 * @brief Invalidate all data caches.
 */
void _data_cache_invalidate_all (void);

/**
 * @brief Set the SRAM bank.
 * @param bank The SRAM bank to set.
 */
void ed64_ll_set_sram_bank (uint8_t bank);

/**
 * @brief Get the current save type.
 * @return The current save type.
 */
ed64_save_type_t ed64_ll_get_save_type (void);

/**
 * @brief Set the save type.
 * @param type The save type to set.
 */
void ed64_ll_set_save_type (ed64_save_type_t type);

/**
 * @brief Get SRAM data.
 * @param buffer The buffer to store the data.
 * @param size The size of the buffer.
 */
void ed64_ll_get_sram (uint8_t *buffer, int size);

/**
 * @brief Get EEPROM data.
 * @param buffer The buffer to store the data.
 * @param size The size of the buffer.
 */
void ed64_ll_get_eeprom (uint8_t *buffer, int size);

/**
 * @brief Get FRAM data.
 * @param buffer The buffer to store the data.
 * @param size The size of the buffer.
 */
void ed64_ll_get_fram (uint8_t *buffer, int size);

/**
 * @brief Set SRAM data.
 * @param buffer The buffer containing the data.
 * @param size The size of the buffer.
 */
void ed64_ll_set_sram (uint8_t *buffer, int size);

/**
 * @brief Set EEPROM data.
 * @param buffer The buffer containing the data.
 * @param size The size of the buffer.
 */
void ed64_ll_set_eeprom (uint8_t *buffer, int size);

/**
 * @brief Set FRAM data.
 * @param buffer The buffer containing the data.
 * @param size The size of the buffer.
 */
void ed64_ll_set_fram (uint8_t *buffer, int size);


/** @} */ /* ed64 */
#endif
