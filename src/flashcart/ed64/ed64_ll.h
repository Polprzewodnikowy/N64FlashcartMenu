/**
 * @file flashcart.h
 * @brief ED64 V-Series Flashcart Internal Utilities
 * @ingroup flashcart 
 */

#ifndef FLASHCART_ED64_LL_H__
#define FLASHCART_ED64_LL_H__


#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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


#define SRAM_ADDRESS (0x1FFE2000)
#define ROM_ADDRESS  (0xB0000000)

/* Save functions */
void ed64_ll_set_ram_bank(uint8_t bank);
ed64_save_type_t ed64_ll_get_save_type();
void ed64_ll_set_save_type(ed64_save_type_t type);

/** @} */ /* ed64 */

#endif
