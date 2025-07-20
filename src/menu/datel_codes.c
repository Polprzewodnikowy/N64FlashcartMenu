#include "datel_codes.h"
#include <string.h>
#include <libdragon.h> // only included for debugf

#define MAX_FILE_SIZE KiB(128)

/** @brief Text file structure */
typedef struct {
    FILE *f; /**< File pointer */
    char *contents; /**< File contents */
    size_t length; /**< File length */
    int lines; /**< Number of lines */
    int current_line; /**< Current line */
    int offset; /**< Offset in the file */
} cheat_file_t;

static cheat_file_t *cheat_file_text;

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

/**
 * @brief Deinitialize the cheat file.
 */
static void deinit (void) {
    if (cheat_file_text) {
        if (cheat_file_text->f) {
            fclose(cheat_file_text->f);
        }
        if (cheat_file_text->contents) {
            free(cheat_file_text->contents);
        }
        free(cheat_file_text);
        cheat_file_text = NULL;
    }
}

void load_cheats_from_file(char *path) {

    // We should be loading the cheat codes from a file here.
    // but only if the file exists. and its content is not zero.

    debugf("Cheat Editor: Loading cheats from path %s.\n", path);

    // if ((cheat_file_text = calloc(1, sizeof(cheat_file_t))) == NULL) {
    //     return menu_show_error(menu, "Couldn't allocate memory for the cheat file");
    // }

    // cheat_file_text->f = fopen(path, "r");

    // if (cheat_file_text->f == NULL) {
    //     deinit();
    //     return menu_show_error(menu, "Couldn't open cheat file");
    // }

    // struct stat st;
    // if (fstat(fileno(cheat_file_text->f), &st)) {
    //     deinit();
    //     return menu_show_error(menu, "Couldn't get cheat file size");
    // }
    // text->length = st.st_size;

    // if (cheat_file_text->length <= 0) {
    //     deinit();
    //     return menu_show_error(menu, "Cheat file is empty");
    // }

    // if (cheat_file_text->length > MAX_FILE_SIZE) {
    //     deinit();
    //     return menu_show_error(menu, "Cheat file is too big to be read");
    // }

    // if ((cheat_file_text->contents = malloc((cheat_file_text->length + 1) * sizeof(char))) == NULL) {
    //     deinit();
    //     return menu_show_error(menu, "Couldn't allocate memory for the cheat file contents");
    // }

    // if (fread(cheat_file_text->contents, cheat_file_text->length, 1, cheat_file_text->f) != 1) {
    //     deinit();
    //     return menu_show_error(menu, "Couldn't read cheat file contents");
    // }
    // cheat_file_text->contents[cheat_file_text->length] = '\0';

    // if (fclose(cheat_file_text->f)) {
    //     deinit();
    //     return menu_show_error(menu, "Couldn't close cheat file");
    // }
    // cheat_file_text->f = NULL;

    // cheat_file_text->lines = 1;
    // for (size_t i = 0; i < cheat_file_text->length; i++) {
    //     if (cheat_file_text->contents[i] == '\n') {
    //         cheat_file_text->lines += 1;
    //     }
    // }

    // int direction = (lines < 0) ? -1 : 1;
    // int next_offset = cheat_file_text->offset;

    // for (int i = 0; i < abs(lines); i++) {
    //     while (true) {
    //         next_offset += direction;
    //         if (next_offset <= 0) {
    //             cheat_file_text->current_line = 0;
    //             cheat_file_text->offset = 0;
    //             return;
    //         }
    //         if (next_offset > cheat_file_text->length) {
    //             return;
    //         }
    //         if (cheat_file_text->contents[next_offset - 1] == '\n') {
    //             break;
    //         }
    //     }
    //     cheat_file_text->current_line += direction;
    //     cheat_file_text->offset = next_offset;
    // }

    // Currently we are just going to pre populate them for test purposes.
    debugf("Cheat Editor: Init debug codes MM USA.\n");
    // Activator code
    parse_cheat_code_string(&cheat_codes[0],  "F1096820 2400 Activator code");
    parse_cheat_code_string(&cheat_codes[1],  "FF000220 0000 Activator code");

    // Inventory Editor (assigned to L)
    parse_cheat_code_string(&cheat_codes[2],  "D01F9B91 0020 Inventory Editor (assigned to L)");
    cheat_codes[2].enabled = false;

    parse_cheat_code_string(&cheat_codes[3],  "803FDA3F 0002 Inventory Editor (assigned to L)");
    cheat_codes[3].enabled = false;

    // Complete Bomber's Notebook
    parse_cheat_code_string(&cheat_codes[4],  "811F05AA FFFF Complete Bomber's Notebook");
    parse_cheat_code_string(&cheat_codes[5],  "811F05AC FFFF Complete Bomber's Notebook");
    parse_cheat_code_string(&cheat_codes[6],  "811F05AE FFFF Complete Bomber's Notebook");
    parse_cheat_code_string(&cheat_codes[7],  "811F05B0 FFFF Complete Bomber's Notebook");

    // Disable 3-day Timer
    parse_cheat_code_string(&cheat_codes[8],  "810F6C3C 2400 Disable 3-day Timer");
    cheat_codes[8].enabled = false;

    // Make A Debug Save File
    parse_cheat_code_string(&cheat_codes[9],  "80146ACB 005A Make A Debug Save File");
    parse_cheat_code_string(&cheat_codes[10], "81146B18 1000"); //  Make A Debug Save File
    parse_cheat_code_string(&cheat_codes[11], "81146B1A 0017 Make A Debug Save File");

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
