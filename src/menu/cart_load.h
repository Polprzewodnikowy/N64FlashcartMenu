/**
 * @file cart_load.h
 * @brief ROM/save loading functions
 * @ingroup menu
 */

#ifndef CART_LOAD_H__
#define CART_LOAD_H__


#include "disk_info.h"
#include "flashcart/flashcart.h"
#include "menu_state.h"
#include "rom_info.h"


typedef enum {
    CART_LOAD_OK,
    CART_LOAD_ERR_ROM_LOAD_FAIL,
    CART_LOAD_ERR_SAVE_LOAD_FAIL,
    CART_LOAD_ERR_64DD_PRESENT,
    CART_LOAD_ERR_64DD_IPL_NOT_FOUND,
    CART_LOAD_ERR_64DD_IPL_LOAD_FAIL,
    CART_LOAD_ERR_64DD_DISK_LOAD_FAIL,
    CART_LOAD_ERR_EMU_NOT_FOUND,
    CART_LOAD_ERR_EMU_LOAD_FAIL,
    CART_LOAD_ERR_EMU_ROM_LOAD_FAIL,
    CART_LOAD_ERR_CREATE_SAVES_SUBDIR_FAIL,
    CART_LOAD_ERR_EXP_PAK_NOT_FOUND,
    CART_LOAD_ERR_FUNCTION_NOT_SUPPORTED,
} cart_load_err_t;

typedef enum {
    CART_LOAD_EMU_TYPE_NES,
    CART_LOAD_EMU_TYPE_SNES,
    CART_LOAD_EMU_TYPE_GAMEBOY,
    CART_LOAD_EMU_TYPE_GAMEBOY_COLOR,
    CART_LOAD_EMU_TYPE_SEGA_GENERIC_8BIT,
} cart_load_emu_type_t;


char *cart_load_convert_error_message (cart_load_err_t err);
cart_load_err_t cart_load_n64_rom_and_save (menu_t *menu, flashcart_progress_callback_t progress);
cart_load_err_t cart_load_64dd_ipl_and_disk (menu_t *menu, flashcart_progress_callback_t progress);
cart_load_err_t cart_load_emulator (menu_t *menu, cart_load_emu_type_t emu_type, flashcart_progress_callback_t progress);


#endif
