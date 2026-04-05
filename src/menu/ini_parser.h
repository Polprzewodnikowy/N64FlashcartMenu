/**
 * @file ini_parser.h
 * @brief Lightweight INI file parser
 * @ingroup menu
 * 
 * Custom INI parser designed to replace mini.c with:
 * - No filesystem I/O for buffer parsing (works on N64 hardware)
 * - Simple API for reading/writing INI files
 * - Support for sections, comments, and flexible whitespace
 */

#ifndef INI_PARSER_H_
#define INI_PARSER_H_

#include <stddef.h>
#include <stdbool.h>


/** @brief Opaque INI data structure */
typedef struct ini_s ini_t;


/**
 * @brief Load INI file from disk
 * 
 * Reads an INI file from the given path and parses it into memory.
 * Returns NULL if file cannot be opened or parsed.
 * 
 * @param path Path to INI file
 * @return Parsed INI structure, or NULL on error
 */
ini_t* ini_load(const char *path);


/**
 * @brief Load INI file from disk, returning an empty structure on any error
 *
 * Like ini_load, but converts any error (file not found, I/O error, or parse
 * failure) into a valid empty INI structure created via ini_create().
 * Useful for optional config files where absence or corruption should be
 * treated as a fresh/default state.
 *
 * @param path Path to INI file
 * @return Parsed INI structure, or a new empty structure on any error (never NULL)
 */
ini_t* ini_try_load(const char *path);


/**
 * @brief Create a new empty INI structure
 * 
 * @return New INI structure ready for use
 */
ini_t* ini_create(void);


/**
 * @brief Parse INI content from a memory buffer
 * 
 * Parses INI-formatted text from a buffer without filesystem I/O.
 * Useful for extracting metadata from ZIP archives or embedded data.
 * 
 * `@param` buffer INI content (need not be null-terminated)
 * `@param` size Size of buffer in bytes
 * @return Parsed INI structure, or NULL on error
 */
ini_t* ini_parse_buffer(const char *buffer, size_t size);


/**
 * @brief Free INI structure and all associated memory
 * 
 * @param ini INI structure to free
 */
void ini_free(ini_t *ini);


/**
 * @brief Get string value from INI
 * 
 * Retrieves a string value from the specified section and key.
 * Returns default_value if not found.
 * 
 * @param ini INI structure
 * @param section Section name
 * @param key Key name
 * @param default_value Value to return if not found
 * @return String value (do not modify or free)
 */
const char* ini_get_string(ini_t *ini, const char *section, const char *key, const char *default_value);


/**
 * @brief Get integer value from INI
 * 
 * Retrieves an integer value from the specified section and key.
 * Returns default_value if not found or cannot be parsed.
 * 
 * @param ini INI structure
 * @param section Section name
 * @param key Key name
 * @param default_value Value to return if not found
 * @return Integer value
 */
int ini_get_int(ini_t *ini, const char *section, const char *key, int default_value);


/**
 * @brief Get boolean value from INI
 * 
 * Retrieves a boolean value from the specified section and key.
 * Recognizes: true/false, yes/no, 1/0, on/off (case-insensitive).
 * Returns default_value if not found or cannot be parsed.
 * 
 * @param ini INI structure
 * @param section Section name
 * @param key Key name
 * @param default_value Value to return if not found
 * @return Boolean value
 */
bool ini_get_bool(ini_t *ini, const char *section, const char *key, bool default_value);


/**
 * @brief Set string value in INI
 * 
 * Sets a string value in the specified section and key.
 * Creates section if it doesn't exist.
 * 
 * @param ini INI structure
 * @param section Section name
 * @param key Key name
 * @param value Value to set
 */
void ini_set_string(ini_t *ini, const char *section, const char *key, const char *value);


/**
 * @brief Set integer value in INI
 * 
 * Sets an integer value in the specified section and key.
 * Creates section if it doesn't exist.
 * 
 * @param ini INI structure
 * @param section Section name
 * @param key Key name
 * @param value Value to set
 */
void ini_set_int(ini_t *ini, const char *section, const char *key, int value);


/**
 * @brief Set boolean value in INI
 * 
 * Sets a boolean value in the specified section and key as "true"/"false".
 * Creates section if it doesn't exist.
 * 
 * @param ini INI structure
 * @param section Section name
 * @param key Key name
 * @param value Value to set
 */
void ini_set_bool(ini_t *ini, const char *section, const char *key, bool value);


/**
 * @brief Delete a key from a section
 * 
 * Removes the specified key from the section. Does nothing if section or key not found.
 * 
 * @param ini INI structure
 * @param section Section name
 * @param key Key name
 */
void ini_delete_key(ini_t *ini, const char *section, const char *key);


/**
 * @brief Check if INI is empty
 * 
 * Returns true if all sections have no key-value pairs (or only empty values).
 * 
 * @param ini INI structure
 * @return true if empty, false otherwise
 */
bool ini_is_empty(ini_t *ini);


/**
 * @brief Save INI structure to disk
 * 
 * Writes the INI structure to the specified file path.
 * 
 * @param ini INI structure
 * @param path Path to write to
 * @return true on success, false on error
 */
bool ini_save(ini_t *ini, const char *path);


#endif // INI_PARSER_H_
