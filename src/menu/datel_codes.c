#include <ctype.h>
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
 * Builds an array of enabled cheat entries as consecutive address/value uint32_t pairs and appends two trailing zeros.
 * @param cheats_in Input array of cheat_file_code_t entries to scan.
 * @param cheats_out Output buffer which will receive address/value pairs; must hold at least (MAX_CHEAT_CODES * 2 + 2) uint32_t elements.
 * @return Number of uint32_t values written into cheats_out, including the two trailing zeros.
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
}

/**
 * Provide access to the global cheat codes array.
 *
 * @return Pointer to the first element of the global `cheat_codes` array (type `cheat_file_code_t *`).
 */
cheat_file_code_t *get_cheat_codes(void) {
    return cheat_codes;
}

/**
 * Replace the global cheat codes array or clear it.
 *
 * If `cheats` is non-NULL, copies exactly MAX_CHEAT_CODES entries into the global
 * `cheat_codes` array. If `cheats` is NULL, clears the global `cheat_codes`
 * array by setting all entries to zero.
 *
 * @param cheats Pointer to an array of cheat_file_code_t to copy from, or NULL to clear the global array.
 */
void set_cheat_codes(cheat_file_code_t *cheats) {
    if (cheats) {
        memcpy(cheat_codes, cheats, sizeof(cheat_file_code_t) * MAX_CHEAT_CODES);
    } else {
        memset(cheat_codes, 0, sizeof(cheat_file_code_t) * MAX_CHEAT_CODES);
    }
}

/**
 * Set the cheat code's description to the provided string, truncating as needed.
 * @param code Pointer to the cheat code to update; if NULL the function does nothing.
 * @param description Null-terminated string to copy into the code's description; if NULL the function does nothing.
 */
void populate_cheat_code_description(cheat_file_code_t *code, const char *description) {
    if (code && description) {
        strncpy(code->description, description, sizeof(code->description) - 1);
        code->description[sizeof(code->description) - 1] = '\0'; // Ensure null-termination
    }
}

/**
 * Parse a single cheat code line and populate the provided cheat_file_code_t.
 *
 * Parses an input string containing a hexadecimal address and value with an optional
 * textual description, and writes the parsed fields into `code`. On successful parse
 * of address and value the entry is marked enabled; if a description is present it is
 * stored (truncated to 31 characters plus NUL). On parse failure the code is cleared
 * (address and value set to 0), disabled, and its description cleared.
 *
 * @param code Pointer to the cheat_file_code_t to populate; if NULL the function only logs an error.
 * @param code_str Input string in the form "address value [description]" where `address` and `value`
 *                 are hexadecimal numbers and `description` is optional text up to 31 characters.
 */
void parse_cheat_code_string(cheat_file_code_t *code, const char *code_str) {
    if (code && code_str) {
        unsigned int addr = 0, val = 0;
        char description[32] = {0};
        int parsed = sscanf(code_str, " %x %x %31[^\n]", &addr, &val, description);
        if (parsed == 3) {
            code->address = (uint32_t)addr;
            code->value   = (uint32_t)val;
            code->enabled = true;
            populate_cheat_code_description(code, description);
        } else if (parsed == 2) {
            code->address = (uint32_t)addr;
            code->value   = (uint32_t)val;
            code->enabled = true;
            code->description[0] = '\0';
        } else {
            debugf("Failed to parse cheat code string: %s\n", code_str);
            code->address = 0;
            code->value   = 0;
            code->enabled = false;
            code->description[0] = '\0'; // Clear description
        }
    } else {
        debugf("Invalid cheat code or code string provided.\n");
        if (code) {
            code->address = 0;
            code->value   = 0;
            code->enabled = false;
            code->description[0] = '\0'; // Clear description
        }
    }
}

/**
 * Release resources associated with the currently opened cheat file and reset internal state.
 *
 * Closes the open file (if any), frees the file contents buffer and the cheat_file_t structure,
 * and sets the internal cheat_file_text pointer to NULL.
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

/**
 * Open and load a cheat file into the internal cheat_file_text structure.
 *
 * @param path Filesystem path to the cheat file to open and read.
 * @returns CHEAT_FILE_LOAD_OK on success.
 *          CHEAT_FILE_LOAD_ERR_MEMORY_ALLOC_FAIL if allocation of the internal structure fails.
 *          CHEAT_FILE_LOAD_ERR_FILE_OPEN_FAIL if the file cannot be opened for reading.
 *          CHEAT_FILE_LOAD_ERR_FILE_STAT_FAIL if retrieving the file's metadata fails.
 *          CHEAT_FILE_LOAD_ERR_FILE_EMPTY if the file length is zero or negative.
 *          CHEAT_FILE_LOAD_ERR_FILE_TOO_BIG if the file exceeds MAX_FILE_SIZE.
 *          CHEAT_FILE_LOAD_ERR_FILE_CONTENTS_ALLOC_FAIL if allocation for file contents fails.
 *          CHEAT_FILE_LOAD_ERR_FILE_READ_FAIL if reading the file contents fails.
 *          CHEAT_FILE_LOAD_ERR_FILE_CLOSE_FAIL if closing the file fails after reading.
 */
cheat_file_load_err_t open_cheat_file(char *path) {
    if ((cheat_file_text = calloc(1, sizeof(cheat_file_t))) == NULL) {
        return CHEAT_FILE_LOAD_ERR_MEMORY_ALLOC_FAIL;
    }

    cheat_file_text->f = fopen(path, "r");

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

/**
 * Print a human-readable debug message corresponding to a cheat file load result.
 *
 * @param res Result code returned by open_cheat_file; the function logs a descriptive message for each defined `cheat_file_load_err_t` value (or "Unknown error" for unrecognized codes).
 */
void cheat_file_open_res_debug(cheat_file_load_err_t res) {
    switch (res) {
        case CHEAT_FILE_LOAD_OK:
            debugf("Cheat file loaded successfully.\n");
            break;
        case CHEAT_FILE_LOAD_ERR_MEMORY_ALLOC_FAIL:
            debugf("Cheat file load error: Memory allocation failed.\n");
            break;
        case CHEAT_FILE_LOAD_ERR_FILE_OPEN_FAIL:
            debugf("Cheat file load error: File open failed.\n");
            break;
        case CHEAT_FILE_LOAD_ERR_FILE_STAT_FAIL:
            debugf("Cheat file load error: File stat failed.\n");
            break;
        case CHEAT_FILE_LOAD_ERR_FILE_EMPTY:
            debugf("Cheat file load error: File is empty.\n");
            break;
        case CHEAT_FILE_LOAD_ERR_FILE_TOO_BIG:
            debugf("Cheat file load error: File is too big.\n");
            break;
        case CHEAT_FILE_LOAD_ERR_FILE_CONTENTS_ALLOC_FAIL:
            debugf("Cheat file load error: File contents allocation failed.\n");
            break;
        case CHEAT_FILE_LOAD_ERR_FILE_READ_FAIL:
            debugf("Cheat file load error: File read failed.\n");
            break;
        case CHEAT_FILE_LOAD_ERR_FILE_CLOSE_FAIL:
            debugf("Cheat file load error: File close failed.\n");
            break;
        default:
            debugf("Cheat file load error: Unknown error.\n");
    }
}

/**
 * Load cheat codes from a text file and populate the global cheat_codes array.
 *
 * Parses the file at the given filesystem path, reading up to MAX_CHEAT_CODES entries.
 * Each non-empty, non-comment line is interpreted as a cheat entry in the form
 * "address value [description]". A leading ":" (optionally followed by whitespace)
 * marks the entry as disabled but still parsed. Lines beginning with '#', ';', or "//"
 * are ignored. After parsing, any remaining entries in the global array are zeroed.
 *
 * @param path Filesystem path to the cheat file to read.
 */
void load_cheats_from_file(char *path) {

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
            // Trim leading whitespace
            char *p = token;
            while (*p && isspace((unsigned char)*p)) p++;
            bool disabled = false;
            // ": " prefix means disabled code; still parse the code after the marker
            if (*p == ':') {
                disabled = true;
                p++;
                while (*p && isspace((unsigned char)*p)) p++;
            }
            // Skip empty or comment-only lines (#, ;, or //)
            if (*p && !(p[0] == '#' || p[0] == ';' || (p[0] == '/' && p[1] == '/'))) {
                parse_cheat_code_string(&cheat_codes[code_count], p);
                bool parsed_ok = (cheat_codes[code_count].address != 0)
                              || (cheat_codes[code_count].value   != 0);
                if (parsed_ok) {
                    if (disabled) 
                        cheat_codes[code_count].enabled = false;
                    code_count++;
                }
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
        cheat_file_open_res_debug(res_file_open);
        set_cheat_codes(NULL);
        deinit_cheat_file();
    }
}

/**
 * Write the in-memory cheat codes to a text file at the specified path.
 *
 * Each non-empty cheat is written as a single line. Disabled cheats are prefixed with ": ".
 * Lines use the format "AAAAAAAA VVVV [description]" where AAAAAAAA is the address as
 * eight uppercase hexadecimal digits and VVVV is the value as four uppercase hexadecimal digits.
 * If a cheat has a non-empty description it is appended after the address and value separated by a space.
 *
 * @param path Filesystem path to create or overwrite with the saved cheat list.
 */
void save_cheats_to_file(char *path) {

    FILE *f = fopen(path, "w");
    if (!f) {
        debugf("Cheat Editor: Failed to open file for writing: %s\n", path);
        return;
    }

    for (int i = 0; i < MAX_CHEAT_CODES; ++i) {
        cheat_file_code_t *code = &cheat_codes[i];
        if (code->address != 0) { //code->enabled &&
            if (!code->enabled) {
                fprintf(f, ": ");
            } 
            if (code->description[0] != '\0') {
                fprintf(f, "%08lX %04X %s\n", code->address, code->value, code->description);
            } else {
                fprintf(f, "%08lX %04X\n", code->address, code->value);
            }
        }
    }

    fclose(f);
    debugf("Cheat Editor: Cheats saved to %s.\n", path);

}