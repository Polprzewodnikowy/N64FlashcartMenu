/**
 * @file datel_codes.h
 * @brief Datel Codes
 * @ingroup menu 
 */

#ifndef DATEL_CODES_H__
#define DATEL_CODES_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define MAX_CHEAT_CODES (34) // Maximum number of cheat codes to display (currently hardcoded to 34 as per support for the ED64x, but can be changed later)
#define MAX_CHEAT_CODE_ARRAYLIST_SIZE (MAX_CHEAT_CODES * 2 + 2) // Maximum size of the cheat code list (address/value pairs + two trailing zeros)

/** @brief Cheat file code Structure. */
typedef struct {
    uint32_t address; /**< Cheat address */
    uint16_t value; /**< Cheat value */ // might need to be uint32_t for loading, though we should be able to convert.
    bool enabled; /**< Cheat enabled flag */
    //char description[32]; /**< Cheat description */
} cheat_file_code_t;



/**
 * @brief Generate a cheats array containing enabled cheats as address/value pairs.
 *        The last two entries will always be zero.
 * 
 * @param cheats_in Input array of type cheat_file_code_t.
 * @param cheats_out Output array (must be at least (MAX_CHEAT_CODES * 2 + 2) in size).
 * @return Number of address/value pairs written (including the trailing zeros).
 */
size_t generate_enabled_cheats_array(cheat_file_code_t *cheats_in, uint32_t *cheats_out);

#endif /* DATEL_CODES_H__ */
