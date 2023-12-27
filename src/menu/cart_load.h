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

/** @brief Cart load state enumeration. */
typedef enum {
    /** @brief Returned no error. */
    CART_LOAD_OK,
    /** @brief Failed to load the ROM correctly. */
    CART_LOAD_ERR_ROM_LOAD_FAIL,
    /** @brief Failed to load the save correctly. */
    CART_LOAD_ERR_SAVE_LOAD_FAIL,
    /** @brief The 64DD is available for use. */
    CART_LOAD_ERR_64DD_PRESENT,
    /** @brief Failed to find the 64DD IPL (BIOS) file. */
    CART_LOAD_ERR_64DD_IPL_NOT_FOUND,
    /** @brief Failed to load the 64DD IPL (BIOS) file. */
    CART_LOAD_ERR_64DD_IPL_LOAD_FAIL,
    /** @brief Failed to find the 64DD disk. */
    CART_LOAD_ERR_64DD_DISK_LOAD_FAIL,
    /** @brief Failed to find the emulator required. */
    CART_LOAD_ERR_EMU_NOT_FOUND,
    /** @brief Failed to load the emulator required. */
    CART_LOAD_ERR_EMU_LOAD_FAIL,
    CART_LOAD_ERR_EMU_ROM_LOAD_FAIL,
    /** @brief Failed to create the save sub-directory. */
    CART_LOAD_ERR_CREATE_SAVES_SUBDIR_FAIL,
    /** @brief There was not enough system memory available (expected an Expansion PAK). */
    CART_LOAD_ERR_EXP_PAK_NOT_FOUND,
    /** @brief An unexpected response. */
    CART_LOAD_ERR_FUNCTION_NOT_SUPPORTED,
} cart_load_err_t;

/** @brief Cart load type enumeration */
typedef enum {
    /** @brief The ROM is designed for a Nintendo Entertainment System or Famicom. */
    CART_LOAD_EMU_TYPE_NES,
    /** @brief The ROM is designed for a Super Nintendo Entertainment System or Super Famicom. */
    CART_LOAD_EMU_TYPE_SNES,
    /** @brief The ROM is designed for a Nintendo Gameboy. */
    CART_LOAD_EMU_TYPE_GAMEBOY,
    /** @brief The ROM is designed for a Nintendo Gameboy Color. */
    CART_LOAD_EMU_TYPE_GAMEBOY_COLOR,
    /** @brief The ROM is designed for a Sega 8Bit system (Game Gear or Master System). */
    CART_LOAD_EMU_TYPE_SEGA_GENERIC_8BIT,
} cart_load_emu_type_t;


char *cart_load_convert_error_message (cart_load_err_t err);
cart_load_err_t cart_load_n64_rom_and_save (menu_t *menu, flashcart_progress_callback_t progress);
cart_load_err_t cart_load_64dd_ipl_and_disk (menu_t *menu, flashcart_progress_callback_t progress);
cart_load_err_t cart_load_emulator (menu_t *menu, cart_load_emu_type_t emu_type, flashcart_progress_callback_t progress);


#endif
