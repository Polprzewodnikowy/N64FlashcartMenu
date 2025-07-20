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

/** * @brief Parse a cheat code string and populate the cheat code structure.
 * 
 * @param code Pointer to the cheat code structure to populate.
 * @param code_str The cheat code string in the format "address value description".
 */
void parse_cheat_code_string(cheat_file_code_t *code, const char *code_str) {
    if (code && code_str) {
        char address_str[10];
        char value_str[6];
        char description[32] = {0};
        int parsed = sscanf(code_str, "%9s %5s %31[^\n]", address_str, value_str, description);
        if (parsed == 3) {
            code->address = strtoul(address_str, NULL, 16);
            code->value = (uint16_t)strtoul(value_str, NULL, 16);
            code->enabled = true; // Assuming the code is enabled by default
            populate_cheat_code_description(code, description);
        } else if (parsed == 2) {
            code->address = strtoul(address_str, NULL, 16);
            code->value = (uint16_t)strtoul(value_str, NULL, 16);
            code->enabled = true;
            code->description[0] = '\0'; // No description provided
        } else {
            debugf("Failed to parse cheat code string: %s\n", code_str);
            code->address = 0;
            code->value = 0;
            code->enabled = false;
            code->description[0] = '\0'; // Clear description
        }
    } else {
        debugf("Invalid cheat code or code string provided.\n");
        if (code) {
            code->address = 0;
            code->value = 0;
            code->enabled = false;
            code->description[0] = '\0'; // Clear description
        }
    }
}

void load_cheats_from_file(char *path) {

    // We should be loading the cheat codes from a file here.

    debugf("Cheat Editor: Loading cheats from path %s.\n", path);

    // Currently we are just going to pre populate them for test purposes.
    debugf("Cheat Editor: Init debug codes MM USA.\n");
    // Activator code
    parse_cheat_code_string(&cheat_codes[0],  "F1096820 2400 Activator code");
    cheat_codes[0].enabled = true;

    parse_cheat_code_string(&cheat_codes[1],  "FF000220 0000 Activator code");
    cheat_codes[1].enabled = true;

    // Inventory Editor (assigned to L)
    parse_cheat_code_string(&cheat_codes[2],  "D01F9B91 0020 Inventory Editor (assigned to L)");
    cheat_codes[2].enabled = false;

    parse_cheat_code_string(&cheat_codes[3],  "803FDA3F 0002 Inventory Editor (assigned to L)");
    cheat_codes[3].enabled = false;

    // Complete Bomber's Notebook
    parse_cheat_code_string(&cheat_codes[4],  "811F05AA FFFF Complete Bomber's Notebook");
    cheat_codes[4].enabled = true;

    parse_cheat_code_string(&cheat_codes[5],  "811F05AC FFFF Complete Bomber's Notebook");
    cheat_codes[5].enabled = true;

    parse_cheat_code_string(&cheat_codes[6],  "811F05AE FFFF Complete Bomber's Notebook");
    cheat_codes[6].enabled = true;

    parse_cheat_code_string(&cheat_codes[7],  "811F05B0 FFFF Complete Bomber's Notebook");
    cheat_codes[7].enabled = true;

    // Disable 3-day Timer
    parse_cheat_code_string(&cheat_codes[8],  "810F6C3C 2400 Disable 3-day Timer");
    cheat_codes[8].enabled = false;

    // Make A Debug Save File
    parse_cheat_code_string(&cheat_codes[9],  "80146ACB 005A Make A Debug Save File");
    cheat_codes[9].enabled = true;

    parse_cheat_code_string(&cheat_codes[10], "81146B18 1000"); //  Make A Debug Save File
    cheat_codes[10].enabled = true;

    parse_cheat_code_string(&cheat_codes[11], "81146B1A 0017 Make A Debug Save File");
    cheat_codes[11].enabled = true;

    // Enable All Owl Statues
    parse_cheat_code_string(&cheat_codes[12], "811EF6B6 FFFF Enable All Owl Statues");
    cheat_codes[12].enabled = false;

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
