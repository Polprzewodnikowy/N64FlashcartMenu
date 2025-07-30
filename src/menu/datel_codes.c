#include "datel_codes.h"
#include <string.h>
#include <libdragon.h> // only included for debugf
#include <sys/stat.h>
#include "utils/utils.h"


#define MAX_FILE_SIZE KiB(128)

/** @brief Text file structure */
typedef struct {
    FILE *f; /**< File pointer */
    char *contents; /**< File contents */
    size_t length; /**< File length */
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
static void deinit_cheat_file (void) {
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

cheat_file_load_err_t open_cheat_file(char *path) {
    if ((cheat_file_text = calloc(1, sizeof(cheat_file_t))) == NULL) {
        return CHEAT_FILE_LOAD_ERR_MEMORY_ALLOC_FAIL;
    }

    cheat_file_text->f = fopen(path, "rw");

    if (cheat_file_text->f == NULL) {
        deinit_cheat_file();
        return CHEAT_FILE_LOAD_ERR_FILE_OPEN_FAIL;
    }

    struct stat st;
    if (fstat(fileno(cheat_file_text->f), &st)) {
        deinit_cheat_file();
        return CHEAT_FILE_LOAD_ERR_FILE_STAT_FAIL;
    }
    cheat_file_text->length = st.st_size;

    if (cheat_file_text->length <= 0) {
        deinit_cheat_file();
        return CHEAT_FILE_LOAD_ERR_FILE_EMPTY;
    }

    if (cheat_file_text->length > MAX_FILE_SIZE) {
        deinit_cheat_file();
        return CHEAT_FILE_LOAD_ERR_FILE_TOO_BIG;
    }

    if ((cheat_file_text->contents = malloc((cheat_file_text->length + 1) * sizeof(char))) == NULL) {
        deinit_cheat_file();
        return CHEAT_FILE_LOAD_ERR_FILE_CONTENTS_ALLOC_FAIL;
    }

    if (fread(cheat_file_text->contents, cheat_file_text->length, 1, cheat_file_text->f) != 1) {
        deinit_cheat_file();
        return CHEAT_FILE_LOAD_ERR_FILE_READ_FAIL;
    }
    cheat_file_text->contents[cheat_file_text->length] = '\0';

    if (fclose(cheat_file_text->f)) {
        deinit_cheat_file();
        return CHEAT_FILE_LOAD_ERR_FILE_CLOSE_FAIL;
    }
    cheat_file_text->f = NULL;

    return CHEAT_FILE_LOAD_OK;
}

void load_cheats_from_file(char *path) {

    // We should be loading the cheat codes from a file here.
    // but only if the file exists. and its content is not zero.

    debugf("Cheat Editor: Loading cheats from path %s.\n", path);

    set_cheat_codes(NULL);
    cheat_file_load_err_t res_file_open = open_cheat_file(path);

    if (res_file_open == CHEAT_FILE_LOAD_OK) {
        debugf("Cheat Editor: Cheat file loaded successfully.\n");

        // Parse each line in the file as a cheat code string
        int code_count = 0;
        char *line = cheat_file_text->contents;
        char *saveptr = NULL;
        char *token = strtok_r(line, "\n", &saveptr);

        while (token && code_count < MAX_CHEAT_CODES) {
            // Skip empty lines
            if (token[0] != '\0') {
                parse_cheat_code_string(&cheat_codes[code_count], token);
                code_count++;
            }
            token = strtok_r(NULL, "\n", &saveptr);
        }

        // Zero out any remaining cheat codes
        for (int i = code_count; i < MAX_CHEAT_CODES; ++i) {
            memset(&cheat_codes[i], 0, sizeof(cheat_file_code_t));
        }

        set_cheat_codes(cheat_codes);

        deinit_cheat_file();

        // --DEBUG CODE
        uint32_t cheats[MAX_CHEAT_CODE_ARRAYLIST_SIZE];
        size_t cheat_item_count = generate_enabled_cheats_array(get_cheat_codes(), cheats);
        debugf("Cheat Editor: Parsed and generated %u cheat items from file.\n", cheat_item_count);
        // --END DEBUG CODE

    }
    else {
        switch (res_file_open) {
            case CHEAT_FILE_LOAD_ERR_MEMORY_ALLOC_FAIL:
                debugf("Cheat Editor: Failed to allocate memory for cheat file.\n");
                break;
            case CHEAT_FILE_LOAD_ERR_FILE_OPEN_FAIL:
                debugf("Cheat Editor: Failed to open cheat file.\n");
                break;
            case CHEAT_FILE_LOAD_ERR_FILE_STAT_FAIL:
                debugf("Cheat Editor: Failed to get cheat file size.\n");
                break;
            case CHEAT_FILE_LOAD_ERR_FILE_EMPTY:
                debugf("Cheat Editor: Cheat file is empty.\n");
                break;
            case CHEAT_FILE_LOAD_ERR_FILE_TOO_BIG:
                debugf("Cheat Editor: Cheat file is too big to be read.\n");
                break;
            case CHEAT_FILE_LOAD_ERR_FILE_CONTENTS_ALLOC_FAIL:
                debugf("Cheat Editor: Failed to allocate memory for cheat file contents.\n");
                break;
            case CHEAT_FILE_LOAD_ERR_FILE_READ_FAIL:
                debugf("Cheat Editor: Failed to read cheat file contents.\n");
                break;
            case CHEAT_FILE_LOAD_ERR_FILE_CLOSE_FAIL:
                debugf("Cheat Editor: Failed to close cheat file.\n");
                break;
            default:
                debugf("Cheat Editor: Unknown error occurred while loading cheat file.\n");
                break;
        }
        set_cheat_codes(NULL);
        deinit_cheat_file();

    }
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
