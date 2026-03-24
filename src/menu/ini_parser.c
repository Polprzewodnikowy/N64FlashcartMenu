/**
 * @file ini_parser.c
 * @brief Lightweight INI file parser implementation
 * @ingroup menu
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <libdragon.h>

#include "ini_parser.h"


/** @brief Maximum number of key-value pairs */
#define INI_MAX_PAIRS 256

/** @brief Maximum number of sections */
#define INI_MAX_SECTIONS 16

/** @brief Maximum length of a key or section name */
#define INI_MAX_NAME_LENGTH 512

/** @brief Maximum length of a value */
#define INI_MAX_VALUE_LENGTH 512


/** @brief Key-value pair structure */
typedef struct {
    char key[INI_MAX_NAME_LENGTH];
    char value[INI_MAX_VALUE_LENGTH];
} ini_pair_t;


/** @brief Section structure */
typedef struct {
    char name[INI_MAX_NAME_LENGTH];
    ini_pair_t pairs[INI_MAX_PAIRS];
    int pair_count;
} ini_section_t;


/** @brief INI structure */
struct ini_s {
    ini_section_t sections[INI_MAX_SECTIONS];
    int section_count;
};


/**
 * @brief Find or create a section
 */
static ini_section_t* find_or_create_section(ini_t *ini, const char *section_name) {
    if (!ini || !section_name) return NULL;
    
    // Try to find existing section
    for (int i = 0; i < ini->section_count; i++) {
        if (strcmp(ini->sections[i].name, section_name) == 0) {
            return &ini->sections[i];
        }
    }
    
    // Create new section if space available
    if (ini->section_count >= INI_MAX_SECTIONS) {
        debugf("[INI] section limit reached\n");
        return NULL;
    }
    
    ini_section_t *new_section = &ini->sections[ini->section_count];
    strncpy(new_section->name, section_name, INI_MAX_NAME_LENGTH - 1);
    new_section->name[INI_MAX_NAME_LENGTH - 1] = '\0';
    new_section->pair_count = 0;
    ini->section_count++;
    
    return new_section;
}


/**
 * @brief Find a key-value pair in a section
 */
static ini_pair_t* find_pair(ini_section_t *section, const char *key) {
    if (!section || !key) return NULL;
    
    for (int i = 0; i < section->pair_count; i++) {
        if (strcmp(section->pairs[i].key, key) == 0) {
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
    
    // Create new pair if space available
    if (section->pair_count >= INI_MAX_PAIRS) {
        debugf("[INI] pair limit reached in section\n");
        return NULL;
    }
    
    pair = &section->pairs[section->pair_count];
    strncpy(pair->key, key, INI_MAX_NAME_LENGTH - 1);
    pair->key[INI_MAX_NAME_LENGTH - 1] = '\0';
    pair->value[0] = '\0';
    section->pair_count++;
    
    return pair;
}


/** Public API Implementation */

ini_t* ini_create(void) {
    ini_t *ini = malloc(sizeof(ini_t));
    if (ini) {
        ini->section_count = 0;
    }
    return ini;
}


void ini_free(ini_t *ini) {
    free(ini);
}


ini_t* ini_parse_buffer(const char *buffer, size_t size) {
    if (!buffer || size == 0) return ini_create();
    
    ini_t *ini = ini_create();
    if (!ini) return NULL;
    
    // Make a null-terminated copy
    char *content = malloc(size + 1);
    if (!content) {
        ini_free(ini);
        return NULL;
    }
    
    memcpy(content, buffer, size);
    content[size] = '\0';
    
    // Parse sections and key=value pairs
    const char *pos = content;
    ini_section_t *section = NULL;
    
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
        
        // Check for section header
        if (*pos == '[') {
            const char *section_start = pos + 1;
            const char *section_end = strchr(section_start, ']');
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
        }
        
        // Parse key=value pair
        const char *key_start = pos;
        const char *eq_pos = strchr(key_start, '=');
        if (eq_pos && eq_pos > key_start && section) {
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
            
            // Get value
            const char *value_start = eq_pos + 1;
            while (*value_start && (*value_start == ' ' || *value_start == '\t')) {
                value_start++;
            }
            
            const char *value_end = value_start;
            while (*value_end && *value_end != '\n' && *value_end != '\r' && *value_end != ';' && *value_end != '#') {
                value_end++;
            }
            
            // Trim trailing whitespace from value
            while (value_end > value_start && (*(value_end - 1) == ' ' || *(value_end - 1) == '\t')) {
                value_end--;
            }
            
            size_t value_len = value_end - value_start;
            
            ini_pair_t *pair = find_or_create_pair(section, key);
            if (pair && value_len < INI_MAX_VALUE_LENGTH) {
                strncpy(pair->value, value_start, value_len);
                pair->value[value_len] = '\0';
            }
            
            pos = value_end;
            continue;
        }
        
        // Skip to next line
        while (*pos && *pos != '\n') pos++;
    }
    
    free(content);
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
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    if (file_size <= 0) {
        fclose(file);
        return ini_create();
    }
    
    // Read file into buffer
    char *buffer = malloc(file_size + 1);
    if (!buffer) {
        fclose(file);
        return ini_create();
    }
    
    size_t read_size = fread(buffer, 1, file_size, file);
    fclose(file);
    
    // Parse buffer
    ini_t *ini = ini_parse_buffer(buffer, read_size);
    free(buffer);
    
    return ini ? ini : ini_create();
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
            if (pair && pair->value[0] != '\0') {
                return pair->value;
            }
        }
    }
    
    return default_value;
}


int ini_get_int(ini_t *ini, const char *section, const char *key, int default_value) {
    const char *str_value = ini_get_string(ini, section, key, NULL);
    if (!str_value) return default_value;
    
    return atoi(str_value);
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
    
    ini_section_t *sec = find_or_create_section(ini, section);
    if (!sec) return;
    
    ini_pair_t *pair = find_or_create_pair(sec, key);
    if (pair) {
        strncpy(pair->value, value, INI_MAX_VALUE_LENGTH - 1);
        pair->value[INI_MAX_VALUE_LENGTH - 1] = '\0';
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
                // Mark as deleted by setting empty value
                pair->value[0] = '\0';
            }
            return;
        }
    }
}


bool ini_is_empty(ini_t *ini) {
    if (!ini) return true;
    
    for (int i = 0; i < ini->section_count; i++) {
        for (int j = 0; j < ini->sections[i].pair_count; j++) {
            if (ini->sections[i].pairs[j].value[0] != '\0') {
                return false;
            }
        }
    }
    
    return true;
}


bool ini_save(ini_t *ini, const char *path) {
    if (!ini || !path) return false;
    
    FILE *file = fopen(path, "wb");
    if (!file) {
        debugf("[INI] ini_save(%s): cannot open file for writing\n", path);
        return false;
    }
    
    for (int i = 0; i < ini->section_count; i++) {
        ini_section_t *section = &ini->sections[i];
        
        // Skip empty sections
        if (section->pair_count == 0) continue;
        
        // Write section header
        fprintf(file, "[%s]\n", section->name);
        
        // Write key-value pairs
        for (int j = 0; j < section->pair_count; j++) {
            ini_pair_t *pair = &section->pairs[j];
            
            // Skip empty pairs
            if (pair->value[0] == '\0') continue;
            
            fprintf(file, "%s = %s\n", pair->key, pair->value);
        }
        
        // Add blank line between sections
        if (i < ini->section_count - 1) {
            fprintf(file, "\n");
        }
    }
    
    fclose(file);
    return true;
}
