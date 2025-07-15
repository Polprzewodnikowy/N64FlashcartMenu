#include "datel_codes.h"
#include <string.h>

cheat_file_code_t cheat_codes[MAX_CHEAT_CODES]; // Global array to hold cheat codes

/**
 * @brief Generate a cheats array containing enabled cheats as address/value pairs.
 *        The last two entries will always be zero.
 * 
 * @param cheats_in Input array of type cheat_file_code_t.
 * @param cheats_out Output array (must be at least (MAX_CHEAT_CODES * 2 + 2) in size).
 * @return Number of address/value pairs written (including the trailing zeros).
 */
size_t generate_enabled_cheats_array(cheat_file_code_t *cheats_in, uint32_t *cheats_out) {
    size_t count = 0;
    for (int i = 0; i < MAX_CHEAT_CODES; ++i) {
        if (cheats_in[i].enabled) {
            cheats_out[count++] = cheats_in[i].address;
            cheats_out[count++] = (uint32_t)cheats_in[i].value;
        }
    }
    // Ensure the last two entries are zero
    cheats_out[count++] = 0;
    cheats_out[count++] = 0;

    return count;
    //return count - 2; // Number of address/value pairs (excluding trailing zeros)
}

/** * @brief Get the cheat codes.
 * 
 * @return Pointer to the array of cheat codes.
 */
cheat_file_code_t *get_cheat_codes(void) {
    return cheat_codes;
}

/** * @brief Set the cheat codes.
 * 
 * @param cheats Pointer to the array of cheat codes.
 */
void set_cheat_codes(cheat_file_code_t *cheats) {
    if (cheats) {
        memcpy(cheat_codes, cheats, sizeof(cheat_file_code_t) * MAX_CHEAT_CODES);
    } else {
        memset(cheat_codes, 0, sizeof(cheat_file_code_t) * MAX_CHEAT_CODES);
    }
}
