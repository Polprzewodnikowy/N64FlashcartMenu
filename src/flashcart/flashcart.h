#ifndef FLASHCART_H__
#define FLASHCART_H__


#include <stdbool.h>
#include <stdint.h>


typedef enum {
    FLASHCART_OK,
    FLASHCART_ERROR_ARGS,
    FLASHCART_ERROR_LOAD,
    FLASHCART_ERROR_INT,
} flashcart_error_t;

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

typedef struct {
    bool (*init) (void);
    void (*deinit) (void);
    flashcart_error_t (*load_rom) (char *rom_path);
    flashcart_error_t (*load_save) (char *save_path);
    flashcart_error_t (*set_save_type) (flashcart_save_type_t save_type);
    flashcart_error_t (*set_save_writeback) (uint32_t *sectors);
} flashcart_t;


bool flashcart_init (void);
void flashcart_deinit (void);
flashcart_error_t flashcart_load_rom (char *rom_path);
flashcart_error_t flashcart_load_save (char *save_path, flashcart_save_type_t save_type, bool save_writeback);


#endif
