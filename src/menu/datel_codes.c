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

/** * @brief Populate a cheat code with a description.
 * 
 * @param code Pointer to the cheat code structure.
 * @param description The description string to set.
 */
void populate_cheat_code_description(cheat_file_code_t *code, const char *description) {
    if (code && description) {
        strncpy(code->description, description, sizeof(code->description) - 1);
        code->description[sizeof(code->description) - 1] = '\0'; // Ensure null-termination
    }
}

void load_cheats_from_file(char *path) {

    // We should be loading the cheat codes from a file here.

    debugf("Cheat Editor: Loading cheats from path %s.\n", path);

    // Currently we are just going to pre populate them for test purposes.
    debugf("Cheat Editor: Init debug codes MM USA.\n");
    // Activator code
    cheat_codes[0].address = 0xF1096820;
    cheat_codes[0].value = 0x2400;
    populate_cheat_code_description(&cheat_codes[0], "Activator code");
    cheat_codes[0].enabled = true;

    cheat_codes[1].address = 0xFF000220;
    cheat_codes[1].value = 0x0000;
    populate_cheat_code_description(&cheat_codes[1], "Activator code");
    cheat_codes[1].enabled = true;

    // Inventory Editor (assigned to L)
    cheat_codes[2].address = 0xD01F9B91;
    cheat_codes[2].value = 0x0020;
    populate_cheat_code_description(&cheat_codes[2], "Inventory Editor  (assigned to L)");
    cheat_codes[2].enabled = false;

    cheat_codes[3].address = 0x803FDA3F;
    cheat_codes[3].value = 0x0002;
    populate_cheat_code_description(&cheat_codes[3], "Inventory Editor  (assigned to L)");
    cheat_codes[3].enabled = false;

    // Complete Bomber's Notebook
    cheat_codes[4].address = 0x811F05AA;
    cheat_codes[4].value = 0xffff;
    populate_cheat_code_description(&cheat_codes[4], "Complete Bomber's Notebook");
    cheat_codes[4].enabled = true;

    cheat_codes[5].address = 0x811F05AC;
    cheat_codes[5].value = 0xffff;
    populate_cheat_code_description(&cheat_codes[5], "Complete Bomber's Notebook");
    cheat_codes[5].enabled = true;

    cheat_codes[6].address = 0x811F05AE;
    cheat_codes[6].value = 0xffff;
    populate_cheat_code_description(&cheat_codes[6], "Complete Bomber's Notebook");
    cheat_codes[6].enabled = true;

    cheat_codes[7].address = 0x811F05B0;
    cheat_codes[7].value = 0xffff;
    populate_cheat_code_description(&cheat_codes[7], "Complete Bomber's Notebook");
    cheat_codes[7].enabled = true;

    cheat_codes[8].address = 0x80146ACB;
    cheat_codes[8].value = 0x005A;
    populate_cheat_code_description(&cheat_codes[8], "Make A Debug Save File");
    cheat_codes[8].enabled = true;

    cheat_codes[9].address = 0x81146B18;
    cheat_codes[9].value = 0x1000;
    populate_cheat_code_description(&cheat_codes[9], "Make A Debug Save File");
    cheat_codes[9].enabled = true;

    cheat_codes[10].address = 0x81146B1A;
    cheat_codes[10].value = 0x0017;
    populate_cheat_code_description(&cheat_codes[10], "Make A Debug Save File");
    cheat_codes[10].enabled = true;

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
    // if (!file_exists(path)) {
    //     if (file_allocate(path, MAX_CHEAT_CODES * sizeof(cheat_file_code_t))) {
    //         debugf("Cheat Editor: Failed to allocate file for saving cheats.\n");
    //         return;
    //     }
    // }
}
