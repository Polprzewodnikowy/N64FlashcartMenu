/**
 * @file cart_load.h
 * @brief ROM/save loading functions
 * @ingroup menu
 */

#ifndef CART_LOAD_H__
#define CART_LOAD_H__


#include "flashcart/flashcart.h"
#include "menu_state.h"
#include "rom_database.h"


typedef enum {
    CART_LOAD_OK,
    CART_LOAD_ERR_SAVES_SUBDIR,
    CART_LOAD_ERR_ROM,
    CART_LOAD_ERR_SAVE,
    CART_LOAD_ERR_EMU_NOT_FOUND,
    CART_LOAD_ERR_EMU,
    CART_LOAD_ERR_EMU_ROM,
} cart_load_err_t;

typedef enum {
    CART_LOAD_EMU_TYPE_NES,
    CART_LOAD_EMU_TYPE_SNES,
    CART_LOAD_EMU_TYPE_GAMEBOY,
    CART_LOAD_EMU_TYPE_GAMEBOY_COLOR,
    CART_LOAD_EMU_TYPE_SEGA_GENERIC_8BIT,
} cart_load_emu_type_t;


char *cart_load_convert_error_message (cart_load_err_t err);
cart_load_err_t cart_load_n64_rom_and_save (menu_t *menu, rom_header_t *header, flashcart_progress_callback_t progress);
cart_load_err_t cart_load_emulator (menu_t *menu, cart_load_emu_type_t emu_type, flashcart_progress_callback_t progress);


#endif
