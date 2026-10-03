/**
 * @file ed64_bios_ll.h
 * @brief Shared low level register access common to all EverDrive-64 models
 *        (V2/V2.5/V3/X5/X7) once their full-feature CPLD/FPGA personality is active.
 * @ingroup flashcart
 */

#ifndef FLASHCART_ED64_BIOS_LL_H__
#define FLASHCART_ED64_BIOS_LL_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @addtogroup ed64_bios_ll
 * @{
 */

/** @brief Largest possible save size (SRAM_128K/FlashRAM). */
#define ED64_BIOS_MAX_SAVE_SIZE  (128 * 1024)

/**
 * @brief Shared save-transfer scratch buffer, usable by V series and X series alike (only
 *        one flashcart driver is ever active at a time on real hardware, and within a
 *        single driver its own save-writeback and save-load paths never run concurrently
 *        either) - avoids reserving 2 separate 128 KiB buffers per series (4 total, 512
 *        KiB combined) for what is always just one save transfer at a time.
 */
extern uint8_t ed64_bios_save_buffer[ED64_BIOS_MAX_SAVE_SIZE];

/** @brief Cart hardware model IDs, as reported by the shared EDID register. */
typedef enum {
    ED64_CART_ID_UNKNOWN = 0,
    ED64_CART_ID_V2 = 0x07, /**< Also covers V2.5 boards; hardware doesn't distinguish them. */
    ED64_CART_ID_V3 = 0x08,
    ED64_CART_ID_X7 = 0x13,
    ED64_CART_ID_X5 = 0x14,
} ed64_cart_id_t;

/** @brief Cart-side save type, as understood by the shared ROM config register. */
typedef enum {
    ED64_SAVE_TYPE_NONE = 0,
    ED64_SAVE_TYPE_EEPROM_4K = 1,
    ED64_SAVE_TYPE_EEPROM_16K = 2,
    ED64_SAVE_TYPE_SRAM_32K = 3,
    ED64_SAVE_TYPE_SRAM_96K_BANKED = 4,
    ED64_SAVE_TYPE_FLASHRAM = 5,
    ED64_SAVE_TYPE_SRAM_128K = 6,
} ed64_save_type_t;

/**
 * @brief Read the cart's hardware model ID. Available from cold boot on every model,
 *        before any full-feature personality has been loaded.
 * 
 * @return ed64_cart_id_t The detected cart model.
 */
ed64_cart_id_t ed64_bios_get_cart_id (void);

/**
 * @brief Configure the cart's save type (selects what the N64-facing save window emulates).
 * 
 * @param save_type The save type to configure.
 */
void ed64_bios_set_save_type (ed64_save_type_t save_type);

/**
 * @brief Read the cart save RAM/EEPROM contents into a buffer.
 * 
 * @param save_type The save type to read as.
 * @param buffer Destination buffer (must be 8-byte aligned for SRAM save types).
 * @param size Number of bytes to read.
 */
void ed64_bios_read_save (ed64_save_type_t save_type, void *buffer, size_t size);

/**
 * @brief Write a buffer into the cart save RAM/EEPROM.
 * 
 * @param save_type The save type to write as.
 * @param buffer Source buffer (must be 8-byte aligned for SRAM save types).
 * @param size Number of bytes to write.
 */
void ed64_bios_write_save (ed64_save_type_t save_type, void *buffer, size_t size);

/**
 * @brief Check whether a save buffer is still at its untouched/blank-file fill value.
 *        Mirrors the official OS's own SRM_NULL_VAL check (bramBackup() in bram.c), which
 *        skips writing a save back to the SD card if the game never touched its save RAM
 *        this session - adapted to this project's own blank-save fill value (0xFF, see
 *        flashcart.c's file_fill() call when first allocating a save file) rather than the
 *        official OS's distinct 0xAA sentinel.
 * 
 * @param buffer Buffer to check.
 * @param size Number of bytes to check.
 * @return true if every byte equals 0xFF.
 */
bool ed64_bios_save_is_blank (const void *buffer, size_t size);

/**
 * @brief Set the cart's persistent "handed off to a game" flag (the shared boot-config
 *        register's GAMEMOD bit), to be read back via ed64_bios_take_game_mode_flag() on
 *        the next boot. Survives the RESET button (cleared only by power-off), same as the
 *        official OS's own use of this exact bit around launching a game.
 */
void ed64_bios_set_game_mode (void);

/**
 * @brief Check and clear the cart's persistent "handed off to a game" flag. Call once at
 *        the start of boot: true means the menu regained control via RESET after a game
 *        ran (so a save-writeback flush may be pending), false means a true cold power-on
 *        (nothing to flush, and the SD-card-backed pending-writeback state doesn't even
 *        need to be read). Matches the official OS's own GAMEMOD check in edBramBackup().
 * 
 * @return true if the flag was set (and has now been cleared), false otherwise.
 */
bool ed64_bios_take_game_mode_flag (void);

/** @} */ /* ed64_bios_ll */

#endif
