/**
 * @file ini_parser.c
 * @brief Lightweight INI file parser implementation
 * @ingroup menu
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <limits.h>
#include <libdragon.h>
#include <errno.h>

#include "ini_parser.h"


/** @brief Maximum number of key-value pairs (logical cap, not pre-allocated) */
#define INI_MAX_PAIRS 64

/** @brief Maximum number of sections (logical cap, not pre-allocated) */
#define INI_MAX_SECTIONS 8

/** @brief Maximum length of a key or section name (parse-time stack buffer) */
#define INI_MAX_NAME_LENGTH 64

/** @brief Maximum length of a value (parse-time stack buffer) */
#define INI_MAX_VALUE_LENGTH 256

/** `@brief` Initial allocated capacity for the sections array */
#define INI_INITIAL_SECTION_CAPACITY 2

/** `@brief` Initial allocated capacity for the pairs array in each section */
#define INI_INITIAL_PAIR_CAPACITY    16


/** @brief Key-value pair structure */
typedef struct {
    char *key;    /**< Heap-allocated key string */
    char *value;  /**< Heap-allocated value string; NULL when deleted/unset */
} ini_pair_t;


/** @brief Section structure */
typedef struct {
    char       *name;          /**< Heap-allocated section name */
    ini_pair_t *pairs;         /**< Heap-allocated array of key-value pairs */
    int         pair_count;    /**< Current number of pairs */
    int         pair_capacity; /**< Allocated capacity of pairs array */
} ini_section_t;


/** @brief INI structure */
struct ini_s {
    ini_section_t *sections;         /**< Heap-allocated array of sections */
    int            section_count;    /**< Current number of sections */
    int            section_capacity; /**< Allocated capacity of sections array */
};

static ini_section_t* find_or_create_section(ini_t *ini, const char *section_name);
static ini_pair_t* find_pair(ini_section_t *section, const char *key);
static ini_pair_t* find_or_create_pair(ini_section_t *section, const char *key);

/**
 * @brief Parse an INI document from a mutable, null-terminated buffer.
 */
static ini_t* ini_parse_mutable_content(char *content, size_t size) {
    if (!content || size == 0) return ini_create();

    ini_t *ini = ini_create();
    if (!ini) return NULL;

    // Parse sections and key=value pairs
    const char *pos = content;
    ini_section_t *section = NULL;
    bool allow_global_keys = true;

    while (*pos) {
        // Skip whitespace and newlines
        while (*pos && (*pos == ' ' || *pos == '\t' || *pos == '\n' || *pos == '\r')) {
            pos++;
        }

        if (!*pos) break;

        // Skip comment lines
        if (*pos == ';' || *pos == '#') {
            while (*pos && *pos != '\n') pos++;
            continue;
        }

        // Find end of current line for bounded searches
        const char *line_end = pos;
        while (*line_end && *line_end != '\n' && *line_end != '\r') {
            line_end++;
        }

        // Check for section header
        if (*pos == '[') {
            allow_global_keys = false;
            const char *section_start = pos + 1;
            ptrdiff_t section_avail = line_end - section_start;
            const char *section_end = (section_avail > 0)
                ? (const char *)memchr(section_start, ']', (size_t)section_avail)
                : NULL;
            if (section_end && section_end > section_start) {
                size_t section_name_len = section_end - section_start;
                if (section_name_len >= INI_MAX_NAME_LENGTH) {
                    section_name_len = INI_MAX_NAME_LENGTH - 1;
                }
                char section_name[INI_MAX_NAME_LENGTH];
                strncpy(section_name, section_start, section_name_len);
                section_name[section_name_len] = '\0';

                section = find_or_create_section(ini, section_name);
                pos = section_end + 1;
                continue;
            }
            // Malformed section line — skip to next line
            pos = (*line_end) ? line_end + 1 : line_end;
            continue;
        }

        // Parse key=value pair
        const char *key_start = pos;
        ptrdiff_t key_avail = line_end - key_start;
        const char *eq_pos = (key_avail > 0)
            ? (const char *)memchr(key_start, '=', (size_t)key_avail)
            : NULL;
        if (eq_pos && eq_pos > key_start) {
            // Route pre-section keys to a "" sentinel.
            // find_or_create_section returns the existing sentinel if already present.
            ini_section_t *target_section = section
                ? section
                : (allow_global_keys ? find_or_create_section(ini, "") : NULL);
            if (!target_section) {
                pos = (*line_end) ? line_end + 1 : line_end;
                continue;
            }
            size_t key_len = eq_pos - key_start;

            // Trim trailing whitespace from key
            while (key_len > 0 && (key_start[key_len - 1] == ' ' || key_start[key_len - 1] == '\t')) {
                key_len--;
            }

            if (key_len >= INI_MAX_NAME_LENGTH) {
                key_len = INI_MAX_NAME_LENGTH - 1;
            }

            char key[INI_MAX_NAME_LENGTH];
            strncpy(key, key_start, key_len);
            key[key_len] = '\0';

            // Get value – handle quoted and unquoted forms
            const char *value_start = eq_pos + 1;
            while (*value_start && (*value_start == ' ' || *value_start == '\t')) {
                value_start++;
            }
            char parsed_value[INI_MAX_VALUE_LENGTH];
            size_t parsed_len = 0;
            const char *new_pos;

            if (*value_start == '"' || *value_start == '\'') {
                // Quoted value: scan until matching closing quote,
                // honouring \\ and \" / \' escape sequences.
                char quote = *value_start;
                const char *vp = value_start + 1;
                while (*vp && *vp != quote && *vp != '\n' && *vp != '\r') {
                    if (*vp == '\\' && *(vp + 1) != '\0') {
                        char next = *(vp + 1);
                        if (next == quote || next == '\\') {
                            if (parsed_len < INI_MAX_VALUE_LENGTH - 1) {
                                parsed_value[parsed_len++] = next;
                            }
                            vp += 2;
                            continue;
                        }
                    }
                    if (parsed_len < INI_MAX_VALUE_LENGTH - 1) {
                        parsed_value[parsed_len++] = *vp;
                    }
                    vp++;
                }
                parsed_value[parsed_len] = '\0';
                // Advance past closing quote when present
                new_pos = (*vp == quote) ? vp + 1 : vp;
            } else {
                // Unquoted value: stop at comment character or end-of-line
                const char *value_end = value_start;
                while (*value_end && *value_end != '\n' && *value_end != '\r' &&
                       *value_end != ';' && *value_end != '#') {
                    value_end++;
                }
                // Trim trailing whitespace
                while (value_end > value_start &&
                       (*(value_end - 1) == ' ' || *(value_end - 1) == '\t')) {
                    value_end--;
                }
                parsed_len = (size_t)(value_end - value_start);
                if (parsed_len >= INI_MAX_VALUE_LENGTH) {
                    parsed_len = INI_MAX_VALUE_LENGTH - 1;
                }
                strncpy(parsed_value, value_start, parsed_len);
                parsed_value[parsed_len] = '\0';
                new_pos = value_end;
            }

            ini_pair_t *pair = find_or_create_pair(target_section, key);
            if (pair) {
                char *new_value = strdup(parsed_value);
                if (new_value) {
                    free(pair->value);
                    pair->value = new_value;
                } else {
                    debugf("[INI] failed to allocate pair value\n");
                }
            }

            pos = new_pos;
            continue;
        }

        // Skip to next line
        while (*pos && *pos != '\n') pos++;
    }

    return ini;
}


/**
 * @brief Find or create a section
 */
static ini_section_t* find_or_create_section(ini_t *ini, const char *section_name) {
    if (!ini || !section_name) return NULL;

    size_t section_name_len = strnlen(section_name, INI_MAX_NAME_LENGTH);
    if (section_name_len == INI_MAX_NAME_LENGTH) {
        debugf("[INI] unterminated section name\n");
        return NULL;
    }
    
    // Try to find existing section
    for (int i = 0; i < ini->section_count; i++) {
        const char *existing_name = ini->sections[i].name;
        if (existing_name && (strncmp(existing_name, section_name, section_name_len + 1) == 0)) {
            return &ini->sections[i];
        }
    }
    
    // Enforce logical cap on number of sections
    if (ini->section_count >= INI_MAX_SECTIONS) {
        debugf("[INI] section limit reached\n");
        return NULL;
    }
    
    // Grow sections array if needed
    if (ini->section_count >= ini->section_capacity) {
        int new_cap = ini->section_capacity == 0
            ? INI_INITIAL_SECTION_CAPACITY
            : ini->section_capacity * 2;
        ini_section_t *grown = realloc(ini->sections, new_cap * sizeof(ini_section_t));
        if (!grown) {
            debugf("[INI] failed to allocate sections\n");
            return NULL;
        }
        ini->sections = grown;
        ini->section_capacity = new_cap;
    }

    ini_section_t *new_section = &ini->sections[ini->section_count];
    new_section->name = strdup(section_name);
    if (!new_section->name) {
        debugf("[INI] failed to allocate section name\n");
        return NULL;
    }
    new_section->pairs = NULL;
    new_section->pair_count = 0;
    new_section->pair_capacity = 0;
    ini->section_count++;
    
    return new_section;
}


/**
 * @brief Find a key-value pair in a section
 */
static ini_pair_t* find_pair(ini_section_t *section, const char *key) {
    if (!section || !key) return NULL;

    size_t key_len = strnlen(key, INI_MAX_NAME_LENGTH);
    if (key_len == INI_MAX_NAME_LENGTH) {
        debugf("[INI] unterminated key\n");
        return NULL;
    }
    
    for (int i = 0; i < section->pair_count; i++) {
        const char *existing_key = section->pairs[i].key;
        if (existing_key && (strncmp(existing_key, key, key_len + 1) == 0)) {
            return &section->pairs[i];
        }
    }
    
    return NULL;
}


/**
 * @brief Find or create a key-value pair in a section
 */
static ini_pair_t* find_or_create_pair(ini_section_t *section, const char *key) {
    if (!section || !key) return NULL;
    
    // Try to find existing pair
    ini_pair_t *pair = find_pair(section, key);
    if (pair) return pair;
    
    // Enforce logical cap on number of pairs per section
    if (section->pair_count >= INI_MAX_PAIRS) {
        debugf("[INI] pair limit reached in section\n");
        return NULL;
    }
    
    // Grow pairs array if needed
    if (section->pair_count >= section->pair_capacity) {
        int new_cap = section->pair_capacity == 0
            ? INI_INITIAL_PAIR_CAPACITY
            : section->pair_capacity * 2;
        ini_pair_t *grown = realloc(section->pairs, new_cap * sizeof(ini_pair_t));
        if (!grown) {
            debugf("[INI] failed to allocate pairs\n");
            return NULL;
        }
        section->pairs = grown;
        section->pair_capacity = new_cap;
    }

    pair = &section->pairs[section->pair_count];
    pair->key = strdup(key);
    if (!pair->key) {
        debugf("[INI] failed to allocate pair key\n");
        return NULL;
    }
    pair->value = NULL;
    section->pair_count++;
    
    return pair;
}


/** Public API Implementation */

ini_t* ini_create(void) {
    ini_t *ini = malloc(sizeof(ini_t));
    if (ini) {
        ini->sections = NULL;
        ini->section_count = 0;
        ini->section_capacity = 0;
    }
    return ini;
}


void ini_free(ini_t *ini) {
    if (!ini) return;

    for (int i = 0; i < ini->section_count; i++) {
        ini_section_t *section = &ini->sections[i];
        for (int j = 0; j < section->pair_count; j++) {
            free(section->pairs[j].key);
            free(section->pairs[j].value);
        }
        free(section->pairs);
        free(section->name);
    }
    free(ini->sections);
    free(ini);
}


ini_t* ini_parse_buffer(const char *buffer, size_t size) {
    if (!buffer || size == 0) return ini_create();

    // Prevent wraparound in size + 1 for temporary null-terminated workspace.
    if (size == SIZE_MAX) {
        return NULL;
    }

    // Parse buffer copies are temporary workspaces; scratch reduces heap interleaving.
    char *content = scratch_malloc(size + 1);
    bool used_scratch = true;
    if (!content) {
        used_scratch = false;
        content = malloc(size + 1);
    }
    if (!content) {
        return NULL;
    }

    memcpy(content, buffer, size);
    content[size] = '\0';

    ini_t *ini = ini_parse_mutable_content(content, size);

    if (used_scratch) {
        scratch_free(content);
    } else {
        free(content);
    }

    return ini;
}


ini_t* ini_load(const char *path) {
    if (!path) return ini_create();
    
    // Try to open file
    FILE *file = fopen(path, "rb");
    if (!file) {
        debugf("[INI] ini_load(%s): file not found\n", path);
        return NULL;
    }
    
    // Get file size
    if (fseek(file, 0, SEEK_END) != 0) {
        fclose(file);
        return NULL;
    }
    long file_size = ftell(file);
    if (file_size < 0 || fseek(file, 0, SEEK_SET) != 0) {
        fclose(file);
        return NULL;
    }
    
    if (file_size == 0) {
        fclose(file);
        return ini_create();
    }
    
    // Read file into buffer
    char *buffer = malloc(file_size + 1);
    if (!buffer) {
        fclose(file);
        return NULL;
    }
    
    size_t read_size = fread(buffer, 1, file_size, file);
    int close_rc = fclose(file);
    if (read_size != (size_t)file_size || close_rc != 0) {
        free(buffer);
        return NULL;
    }

    // Parse buffer in-place to avoid a second temporary copy.
    buffer[read_size] = '\0';
    ini_t *ini = ini_parse_mutable_content(buffer, read_size);
    free(buffer);
    
    return ini;
}


ini_t* ini_try_load(const char *path) {
    ini_t *ini = ini_load(path);
    return ini ? ini : ini_create();
}


const char* ini_get_string(ini_t *ini, const char *section, const char *key, const char *default_value) {
    if (!ini || !section || !key) return default_value;
    
    for (int i = 0; i < ini->section_count; i++) {
        if (strcmp(ini->sections[i].name, section) == 0) {
            ini_pair_t *pair = find_pair(&ini->sections[i], key);
            if (pair && pair->value != NULL) {
                return pair->value;
            }
        }
    }
    
    return default_value;
}


int ini_get_int(ini_t *ini, const char *section, const char *key, int default_value) {
    const char *str_value = ini_get_string(ini, section, key, NULL);
    if (!str_value) return default_value;
    
    char *end = NULL;
    errno = 0;
    long parsed = strtol(str_value, &end, 10);
    if (errno == ERANGE || end == str_value || *end != '\0' ||
        parsed < INT_MIN || parsed > INT_MAX) {
        return default_value;
    }

    return (int)parsed;
}


bool ini_get_bool(ini_t *ini, const char *section, const char *key, bool default_value) {
    const char *str_value = ini_get_string(ini, section, key, NULL);
    if (!str_value) return default_value;
    
    // Check for common boolean representations
    if (strcasecmp(str_value, "true") == 0 || strcasecmp(str_value, "yes") == 0 || 
        strcasecmp(str_value, "on") == 0 || strcmp(str_value, "1") == 0) {
        return true;
    }
    
    if (strcasecmp(str_value, "false") == 0 || strcasecmp(str_value, "no") == 0 || 
        strcasecmp(str_value, "off") == 0 || strcmp(str_value, "0") == 0) {
        return false;
    }
    
    return default_value;
}


void ini_set_string(ini_t *ini, const char *section, const char *key, const char *value) {
    if (!ini || !section || !key || !value) return;
    // Clamp to parse-time limits for consistent round-trip behaviour
    char sec_buf[INI_MAX_NAME_LENGTH];
    snprintf(sec_buf, sizeof(sec_buf), "%s", section);

    char key_buf[INI_MAX_NAME_LENGTH];
    snprintf(key_buf, sizeof(key_buf), "%s", key);

    char val_buf[INI_MAX_VALUE_LENGTH];
    snprintf(val_buf, sizeof(val_buf), "%s", value);

    ini_section_t *sec = find_or_create_section(ini, sec_buf);
    if (!sec) return;
    
    ini_pair_t *pair = find_or_create_pair(sec, key_buf);
    if (pair) {
        char *new_value = strdup(val_buf);
        if (new_value) {
            free(pair->value);
            pair->value = new_value;
        } else {
            debugf("[INI] failed to allocate pair value\n");
        }
    }
}


void ini_set_int(ini_t *ini, const char *section, const char *key, int value) {
    if (!ini || !section || !key) return;
    
    char str_value[32];
    snprintf(str_value, sizeof(str_value), "%d", value);
    ini_set_string(ini, section, key, str_value);
}


void ini_set_bool(ini_t *ini, const char *section, const char *key, bool value) {
    if (!ini || !section || !key) return;
    
    ini_set_string(ini, section, key, value ? "true" : "false");
}


void ini_delete_key(ini_t *ini, const char *section, const char *key) {
    if (!ini || !section || !key) return;
    
    for (int i = 0; i < ini->section_count; i++) {
        if (strcmp(ini->sections[i].name, section) == 0) {
            ini_section_t *sec = &ini->sections[i];
            ini_pair_t *pair = find_pair(sec, key);
            if (pair) {
                // Free and NULL the value to mark as deleted
                free(pair->value);
                pair->value = NULL;
            }
            return;
        }
    }
}


bool ini_is_empty(ini_t *ini) {
    if (!ini) return true;
    
    for (int i = 0; i < ini->section_count; i++) {
        for (int j = 0; j < ini->sections[i].pair_count; j++) {
            const char *v = ini->sections[i].pairs[j].value;
            if (v != NULL) {
                return false;
            }
        }
    }
    
    return true;
}


/**
 * `@brief` Returns true when a stored value must be quoted on write.
 *
 * Quoting is required when the value contains a comment marker (';' or '#'),
 * a quote character, or leading/trailing whitespace that would otherwise be
 * stripped on the next read.
 */
static bool value_needs_quoting(const char *value) {
    if (!value || *value == '\0') return false;
    if (*value == ' ' || *value == '\t') return true;
    size_t len = strlen(value);
    if (value[len - 1] == ' ' || value[len - 1] == '\t') return true;
    for (size_t i = 0; i < len; i++) {
        char c = value[i];
        if (c == ';' || c == '#' || c == '"' || c == '\'') return true;
    }
    return false;
}


bool ini_save(ini_t *ini, const char *path) {
    if (!ini || !path) return false;
    
    FILE *file = fopen(path, "wb");
    if (!file) {
        debugf("[INI] ini_save(%s): cannot open file for writing\n", path);
        return false;
    }
    
    bool ok = true;
    bool wrote_any_block = false;

    // 1) Write global (pre-section) key/value pairs first, without a header
    for (int i = 0; i < ini->section_count && ok; i++) {
        ini_section_t *section = &ini->sections[i];
        if (section->name[0] != '\0') continue; // only the "" sentinel
        for (int j = 0; j < section->pair_count && ok; j++) {
            ini_pair_t *pair = &section->pairs[j];
            if (!pair->value) continue; // deleted
            if (value_needs_quoting(pair->value)) {
                if (fprintf(file, "%s = \"", pair->key) < 0) { ok = false; break; }
                for (const char *vp = pair->value; *vp; vp++) {
                    if (*vp == '"' || *vp == '\\') {
                        if (fputc('\\', file) == EOF) { ok = false; break; }
                    }
                    if (fputc(*vp, file) == EOF) { ok = false; break; }
                }
                if (ok && fprintf(file, "\"\n") < 0) ok = false;
            } else {
                if (fprintf(file, "%s = %s\n", pair->key, pair->value) < 0) { ok = false; }
            }
            wrote_any_block = true;
        }
        break; // only one global section possible
    }

    // 2) Then write named sections; add a blank line before each block after the first
    for (int i = 0; i < ini->section_count && ok; i++) {
        ini_section_t *section = &ini->sections[i];
        if (section->name[0] == '\0') continue; // skip global sentinel here
        bool has_live_pairs = false;
        for (int j = 0; j < section->pair_count; j++) {
            if (section->pairs[j].value) { has_live_pairs = true; break; }
        }
        if (!has_live_pairs) continue;

        if (wrote_any_block) {
            if (fprintf(file, "\n") < 0) { ok = false; break; }
        }
        if (fprintf(file, "[%s]\n", section->name) < 0) { ok = false; break; }

        for (int j = 0; j < section->pair_count && ok; j++) {
            ini_pair_t *pair = &section->pairs[j];
            if (!pair->value) continue;
            if (value_needs_quoting(pair->value)) {
                if (fprintf(file, "%s = \"", pair->key) < 0) { ok = false; break; }
                for (const char *vp = pair->value; *vp; vp++) {
                    if (*vp == '"' || *vp == '\\') {
                        if (fputc('\\', file) == EOF) { ok = false; break; }
                    }
                    if (fputc(*vp, file) == EOF) { ok = false; break; }
                }
                if (ok && fprintf(file, "\"\n") < 0) ok = false;
            } else {
                if (fprintf(file, "%s = %s\n", pair->key, pair->value) < 0) { ok = false; }
            }
        }
        wrote_any_block = true;
    }
    debugf("[INI] ini_save(%s): saving complete\n", path);
    return fclose(file) == 0 && ok;
}
