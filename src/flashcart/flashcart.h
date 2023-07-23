/**
 * @file flashcart.h
 * @brief Flashcart Subsystem
 * @ingroup flashcart 
 */

#ifndef FLASHCART_H__
#define FLASHCART_H__


#include <stdbool.h>
#include <stdint.h>


/** @brief Flashcart error enumeration */
typedef enum {
    FLASHCART_OK,
    FLASHCART_ERROR_NOT_DETECTED,
    FLASHCART_ERROR_SD_CARD_ERROR,
    FLASHCART_ERROR_UNSUPPORTED,
    FLASHCART_ERROR_OUTDATED,
    FLASHCART_ERROR_ARGS,
    FLASHCART_ERROR_LOAD,
    FLASHCART_ERROR_INT,
} flashcart_error_t;

/** @brief Flashcart save type enumeration */
typedef enum {
    FLASHCART_SAVE_TYPE_NONE,
    FLASHCART_SAVE_TYPE_EEPROM_4K,
    FLASHCART_SAVE_TYPE_EEPROM_16K,
    FLASHCART_SAVE_TYPE_SRAM,
    FLASHCART_SAVE_TYPE_SRAM_BANKED,
    FLASHCART_SAVE_TYPE_SRAM_128K,
    FLASHCART_SAVE_TYPE_FLASHRAM,
    __FLASHCART_SAVE_TYPE_END
} flashcart_save_type_t;

/** @brief Flashcart Structure */
typedef struct {
    flashcart_error_t (*init) (void);
    flashcart_error_t (*deinit) (void);
    flashcart_error_t (*load_rom) (char *rom_path, bool byte_swap);
    flashcart_error_t (*load_emulator_rom) (char *rom_path);
    flashcart_error_t (*load_save) (char *save_path);
    flashcart_error_t (*set_save_type) (flashcart_save_type_t save_type);
    flashcart_error_t (*set_save_writeback) (uint32_t *sectors);
} flashcart_t;


flashcart_error_t flashcart_init (void);
flashcart_error_t flashcart_deinit (void);
flashcart_error_t flashcart_load_rom (char *rom_path, bool byte_swap);
flashcart_error_t flashcart_load_emulator_rom (char *rom_path);
flashcart_error_t flashcart_load_save (char *save_path, flashcart_save_type_t save_type);


#endif
