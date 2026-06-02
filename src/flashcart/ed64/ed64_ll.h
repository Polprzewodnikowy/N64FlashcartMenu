/**
 * @file ed64_ll.h
 * @brief EverDrive-64 low-level cart register and save access
 * @ingroup flashcart
 */

#ifndef FLASHCART_ED64_LL_H__
#define FLASHCART_ED64_LL_H__

#include <stdint.h>
#include <stdbool.h>

#define PI_SAVE_ADDR    0xA8000000UL
#define ROM_ADDRESS     0xB0000000UL

typedef enum {
    SAVE_TYPE_NONE = 0,
    SAVE_TYPE_SRAM = 1,
    SAVE_TYPE_SRAM_128K = 2,
    SAVE_TYPE_EEPROM_4K = 3,
    SAVE_TYPE_EEPROM_16K = 4,
    SAVE_TYPE_FLASHRAM = 5,
} ed64_save_type_t;

/** @brief EverDrive V-series hardware revision (from cart version register). */
typedef enum {
    ED64_HW_UNKNOWN = 0,
    ED64_HW_V1,
    ED64_HW_V2,
    ED64_HW_V2_5,
    ED64_HW_V3,
} ed64_hw_model_t;

/** V3 and later use battery-backed save RAM (see libdragon ed_init). */
#define ED64_HW_BATTERY_SAVE_THRESHOLD  0x300

void ed64_ll_detect_hardware (void);
uint16_t ed64_ll_get_hw_version (void);
ed64_hw_model_t ed64_ll_get_hw_model (void);
bool ed64_ll_has_battery_backed_save (void);
bool ed64_ll_needs_reset_for_save (void);

void ed64_ll_set_sdcard_timing (void);
void ed64_ll_refresh_sd_speed (void);
ed64_save_type_t ed64_ll_get_save_type (void);
void ed64_ll_set_save_type (ed64_save_type_t type);
void ed64_ll_get_sram (uint8_t *buffer, int size);
void ed64_ll_get_eeprom (uint8_t *buffer, int size);
void ed64_ll_get_fram (uint8_t *buffer, int size);
void ed64_ll_set_sram (uint8_t *buffer, int size);
void ed64_ll_set_eeprom (uint8_t *buffer, int size);
void ed64_ll_set_fram (uint8_t *buffer, int size);

#endif
