/**
 * @file virtual_cpak.h
 * @brief Virtual Controller Pak management for per-game pak saves.
 * @ingroup menu
 */

#ifndef VIRTUAL_CPAK_H__
#define VIRTUAL_CPAK_H__

#include <stdbool.h>
#include <stdint.h>

/** @brief Base directory for virtual cpak saves on SD card */
#define VCPAK_SAVES_BASE_DIR    "cpak_saves"

/** @brief Filename for dirty state tracking file */
#define VCPAK_STATE_FILENAME    "vcpak_state.dat"

/** @brief Magic number for state file validation ("VCPS") */
#define VCPAK_STATE_MAGIC       0x56435053

/** @brief Virtual CPak error codes */
typedef enum {
    VCPAK_OK,                   /**< Success */
    VCPAK_ERR_NO_CPAK,          /**< No physical Controller Pak detected */
    VCPAK_ERR_IO,               /**< I/O error during read/write */
    VCPAK_ERR_CORRUPTED,        /**< Pak data is corrupted */
    VCPAK_ERR_FILE_NOT_FOUND,   /**< Pak file not found */
    VCPAK_ERR_ALLOC,            /**< Memory allocation failed */
    VCPAK_ERR_DIR_CREATE,       /**< Failed to create directory */
    VCPAK_ERR_TOO_LARGE,        /**< Pak file too large for physical device */
} vcpak_err_t;

/**
 * @brief Dirty state information persisted to SD card.
 *
 * This structure is written before booting a game and cleared after
 * successfully backing up the pak on return. If the menu starts and
 * finds this file with is_dirty=1, an unclean exit occurred.
 *
 * Fields are ordered for natural alignment (uint32_t first, then arrays).
 */
typedef struct {
    uint32_t magic;             /**< Magic number for validation (VCPAK_STATE_MAGIC) */
    uint32_t timestamp;         /**< Unix timestamp when game was launched */
    char game_code[5];          /**< 4-char game code + null terminator */
    char game_title[21];        /**< 20-char game title + null terminator */
    char rom_path[256];         /**< Full path to the ROM file */
    char pak_path[256];         /**< Full path to the .pak file being used */
    uint8_t is_dirty;           /**< 1 if session not cleanly ended, 0 otherwise */
    uint8_t reserved[30];       /**< Reserved for future use */
} vcpak_state_t;

/**
 * @brief Entry representing a single pak file in a game's pak list.
 */
typedef struct {
    char filename[64];          /**< Filename only (no path) */
    char full_path[256];        /**< Full SD card path to the file */
    bool is_last_used;          /**< True if this was the last used pak for this game */
} vcpak_entry_t;

/**
 * @brief List of pak files available for a game.
 */
typedef struct {
    vcpak_entry_t *entries;     /**< Array of pak entries (dynamically allocated) */
    int count;                  /**< Number of entries in the list */
    int selected;               /**< Index of currently selected entry (-1 if none) */
    char game_code[5];          /**< Game code this list is for */
} vcpak_list_t;


/*
 * Directory Management
 */

/**
 * @brief Ensure the game-specific cpak directory exists.
 *
 * Creates SD:/cpak_saves/{game_code}/ if it doesn't exist.
 *
 * @param storage_prefix The storage prefix (e.g., "sd:/")
 * @param game_code The 4-character game code
 * @return vcpak_err_t Error code
 */
vcpak_err_t vcpak_ensure_game_directory(const char *storage_prefix, const char *game_code);

/**
 * @brief Get the full path to a game's cpak directory.
 *
 * @param storage_prefix The storage prefix (e.g., "sd:/")
 * @param game_code The 4-character game code
 * @param out_path Buffer to store the path (must be at least 256 bytes)
 * @param out_size Size of the output buffer
 */
void vcpak_get_game_directory(const char *storage_prefix, const char *game_code,
                               char *out_path, size_t out_size);


/*
 * Pak File Enumeration
 */

/**
 * @brief List all pak files available for a game.
 *
 * Populates the vcpak_list_t structure with entries from the game's
 * cpak directory. The caller must call vcpak_list_free() when done.
 *
 * @param storage_prefix The storage prefix (e.g., "sd:/")
 * @param game_code The 4-character game code
 * @param last_used_filename The filename of the last-used pak (for marking is_last_used)
 * @param out_list Output list structure to populate
 * @return vcpak_err_t Error code
 */
vcpak_err_t vcpak_list_paks(const char *storage_prefix, const char *game_code,
                            const char *last_used_filename, vcpak_list_t *out_list);

/**
 * @brief Free resources allocated by vcpak_list_paks().
 *
 * @param list The list to free
 */
void vcpak_list_free(vcpak_list_t *list);


/*
 * Pak Operations
 */

/**
 * @brief Restore a pak file from SD card to the physical Controller Pak.
 *
 * @param pak_path Full path to the .pak file on SD card
 * @param controller Controller port (0-3)
 * @return vcpak_err_t Error code
 */
vcpak_err_t vcpak_restore_to_physical(const char *pak_path, int controller);

/**
 * @brief Backup the physical Controller Pak contents to a file on SD card.
 *
 * @param pak_path Full path to save the .pak file
 * @param controller Controller port (0-3)
 * @return vcpak_err_t Error code
 */
vcpak_err_t vcpak_backup_from_physical(const char *pak_path, int controller);

/**
 * @brief Create a new empty (formatted) pak file.
 *
 * @param pak_path Full path where the new .pak file should be created
 * @return vcpak_err_t Error code
 */
vcpak_err_t vcpak_create_empty(const char *pak_path);

/**
 * @brief Generate a default filename for a new pak.
 *
 * Creates a filename like "GameName_001.pak" based on existing files.
 *
 * @param storage_prefix The storage prefix
 * @param game_code The game code
 * @param game_title The game title (used for filename prefix)
 * @param out_filename Buffer for the generated filename
 * @param out_size Size of the output buffer
 * @return vcpak_err_t Error code
 */
vcpak_err_t vcpak_generate_filename(const char *storage_prefix, const char *game_code,
                                    const char *game_title, char *out_filename, size_t out_size);


/*
 * Dirty State Management
 */

/**
 * @brief Save the dirty state to SD card.
 *
 * Called just before booting a game to track that a session is in progress.
 *
 * @param storage_prefix The storage prefix
 * @param state The state to save
 * @return vcpak_err_t Error code
 */
vcpak_err_t vcpak_state_save(const char *storage_prefix, const vcpak_state_t *state);

/**
 * @brief Load the dirty state from SD card.
 *
 * @param storage_prefix The storage prefix
 * @param state Output state structure
 * @return vcpak_err_t Error code (VCPAK_ERR_FILE_NOT_FOUND if no state file)
 */
vcpak_err_t vcpak_state_load(const char *storage_prefix, vcpak_state_t *state);

/**
 * @brief Clear (delete) the dirty state file.
 *
 * Called after successfully backing up the pak on return to menu.
 *
 * @param storage_prefix The storage prefix
 * @return vcpak_err_t Error code
 */
vcpak_err_t vcpak_state_clear(const char *storage_prefix);

/**
 * @brief Check if a dirty state exists.
 *
 * @param storage_prefix The storage prefix
 * @return true if dirty state file exists and is_dirty is set
 */
bool vcpak_state_is_dirty(const char *storage_prefix);

#endif /* VIRTUAL_CPAK_H__ */
