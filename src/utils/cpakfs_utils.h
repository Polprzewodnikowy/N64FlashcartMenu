
#ifndef CPAKFS_UTILS__H__
#define CPAKFS_UTILS__H__

/**
 * @file cpakfs_utils.h
 * @brief Utilities for working with Controller Pak file systems.
 * @ingroup utils
 */

#include <stdbool.h>
#include <libdragon.h>
#include <stddef.h>

/**
 * @def FS_BLOCK_DIVIDER
 * @brief Divider constant for file system block calculations.
 */
#define FS_BLOCK_DIVIDER (8 * 32)

/**
 * @def MAX_NUM_NOTES
 * @brief Maximum number of notes (files) supported on a Controller Pak.
 */
#define MAX_NUM_NOTES 16

/**
 * @struct cpakfs_path_strings_t
 * @brief Structure holding parsed components of a Controller Pak file path.
 *
 * This struct contains the game code, publisher code, filename, and extension
 * parsed from a Controller Pak file path.
 */
typedef struct {
    char gamecode[5];   /**< Game code (4 chars + null terminator) */
    char pubcode[5];    /**< Publisher code (up to 4 chars + null terminator) */
    char filename[17];  /**< Filename (up to 16 chars + null terminator) */
    char ext[5];        /**< Extension (up to 4 chars + null terminator) */
} cpakfs_path_strings_t;

/**
 * @brief Array of Controller Pak mount point strings (e.g., "cpak1:/").
 */
extern const char * const CPAK_MOUNT_ARRAY[4];

/**
 * @brief Get the block size from a Controller Pak file system path.
 *
 * @param filename_cpak The Controller Pak file path.
 * @return The block size, or -1 on error.
 */
int get_block_size_from_fs_path(const char *filename_cpak);

/**
 * @brief Unmount all Controller Pak file systems.
 */
void unmount_all_cpakfs(void);

/**
 * @brief Mount the Controller Pak file system for a given controller.
 *
 * @param controller The controller index (0-3).
 * @return 0 on success, -1 on failure.
 */
int mount_cpakfs(int controller);

/**
 * @brief Check if a Controller Pak is present in the given controller.
 *
 * @param controller The controller index (0-3).
 * @return true if a Controller Pak is present, false otherwise.
 */
bool has_cpak(int controller);

/**
 * @brief Get the file size from a Controller Pak file system path.
 *
 * @param filename_cpak The Controller Pak file path.
 * @return The file size in bytes, or -1 on error.
 */
int get_file_size_from_fs_path(const char *filename_cpak);

/**
 * @brief Extract the title from an absolute file path.
 *
 * @param path The absolute file path.
 * @param outbuf Output buffer for the title.
 * @param outbuf_size Size of the output buffer.
 * @return 0 on success, negative value on error.
 */
int extract_title_from_absolute_path(const char *path, char *outbuf, size_t outbuf_size);

/**
 * @brief Parse a Controller Pak full filename into its components.
 *
 * @param fullname The full filename string.
 * @param out Output struct for parsed components.
 * @return 0 on success, negative value on error.
 */
int parse_cpakfs_fullname(const char *fullname, cpakfs_path_strings_t *out);

/**
 * @brief Check if a file exists at the given full mounted path.
 *
 * @param full_mounted_path The full mounted file path.
 * @return 1 if the file exists, 0 otherwise.
 */
int file_exists_full(const char *full_mounted_path);

/**
 * @brief Increment a note index, wrapping around MAX_NUM_NOTES.
 *
 * @param current_index The current note index.
 * @return The incremented note index.
 */
int inc_index_note(int current_index);

/**
 * @brief Decrement a note index, wrapping around MAX_NUM_NOTES.
 *
 * @param current_index The current note index.
 * @return The decremented note index.
 */
int dec_index_note(int current_index);

/**
 * @brief Pick a unique full filename with mount prefix, avoiding collisions.
 *
 * Attempts to find a unique filename by incrementing the extension in base36 if needed.
 *
 * @param mount_prefix The mount prefix (e.g., "cpak1:/").
 * @param desired_name The desired base filename.
 * @param out_fullpath Output buffer for the unique full path.
 * @param outsz Size of the output buffer.
 * @param exists_fullpath Function pointer to check if a full path exists.
 * @return 0 on success, negative value on error.
 */
int pick_unique_fullname_with_mount(const char *mount_prefix,
                                    const char *desired_name,
                                    char *out_fullpath, size_t outsz,
                                    int (*exists_fullpath)(const char *fullpath));

#endif // CPAKFS_UTILS__H__
