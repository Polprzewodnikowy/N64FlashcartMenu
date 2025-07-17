#include "datel_codes.h"
#include <string.h>
#include <libdragon.h> // only included for debugf

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

void load_cheats_from_file(char *path) {

    // We should be loading the cheat codes from a file here.

    debugf("Cheat Editor: Loading cheats from path %s.\n", path);

    // Currently we are just going to pre populate them for test purposes.
    debugf("Cheat Editor: Init debug codes MM USA.\n");
    // Enable code
    cheat_codes[0].address = 0xF1096820;
    cheat_codes[0].value = 0x2400;
    strncpy(cheat_codes[0].description, "Enable code 1", sizeof(cheat_codes[0].description) - 1);
    cheat_codes[0].description[sizeof(cheat_codes[0].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[0].enabled = true;

    cheat_codes[1].address = 0xFF000220;
    cheat_codes[1].value = 0x0000;
    strncpy(cheat_codes[1].description, "Enable code 2", sizeof(cheat_codes[1].description) - 1);
    cheat_codes[1].description[sizeof(cheat_codes[1].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[1].enabled = true;

    // Inventory Editor (assigned to L)
    cheat_codes[2].address = 0xD01F9B91;
    cheat_codes[2].value = 0x0020;
    strncpy(cheat_codes[2].description, "Inventory Editor 1", sizeof(cheat_codes[2].description) - 1);
    cheat_codes[2].description[sizeof(cheat_codes[2].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[2].enabled = false;

    cheat_codes[3].address = 0x803FDA3F;
    cheat_codes[3].value = 0x0002;
    strncpy(cheat_codes[3].description, "Inventory Editor 2", sizeof(cheat_codes[3].description) - 1);
    cheat_codes[3].description[sizeof(cheat_codes[3].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[3].enabled = false;

    // Complete Bomber's Notebook
    cheat_codes[4].address = 0x811F05AA;
    cheat_codes[4].value = 0xffff;
    strncpy(cheat_codes[4].description, "Complete Bomber's Notebook 1", sizeof(cheat_codes[4].description) - 1);
    cheat_codes[4].description[sizeof(cheat_codes[4].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[4].enabled = true;

    cheat_codes[5].address = 0x811F05AC;
    cheat_codes[5].value = 0xffff;
    strncpy(cheat_codes[5].description, "Complete Bomber's Notebook 2", sizeof(cheat_codes[5].description) - 1);
    cheat_codes[5].description[sizeof(cheat_codes[5].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[5].enabled = true;

    cheat_codes[6].address = 0x811F05AE;
    cheat_codes[6].value = 0xffff;
    strncpy(cheat_codes[6].description, "Complete Bomber's Notebook 3", sizeof(cheat_codes[6].description) - 1);
    cheat_codes[6].description[sizeof(cheat_codes[6].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[6].enabled = true;

    cheat_codes[7].address = 0x811F05B0;
    cheat_codes[7].value = 0xffff;
    strncpy(cheat_codes[7].description, "Complete Bomber's Notebook 4", sizeof(cheat_codes[7].description) - 1);
    cheat_codes[7].description[sizeof(cheat_codes[7].description) - 1] = '\0'; // Ensure null-termination
    cheat_codes[7].enabled = true;

    set_cheat_codes(cheat_codes);
    uint32_t cheats[MAX_CHEAT_CODE_ARRAYLIST_SIZE];
    size_t cheat_item_count = generate_enabled_cheats_array(get_cheat_codes(), cheats);
    // cheats[] now contains address/value pairs for enabled cheats, ending with two zeros.
    debugf("Cheat Editor: Generated %u cheat items.\n", cheat_item_count);
}

void save_cheats_to_file(char *path) {
    // This function should save the cheat codes to a file.
    // For now, we will just print them to the debug log.
    debugf("Cheat Editor: Saving cheats to path %s (not implemented).\n", path);
}
