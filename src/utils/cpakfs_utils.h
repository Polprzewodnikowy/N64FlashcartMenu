
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
 * @def CPAK_BANK_SIZE
 * @brief Size of one Controller Pak bank in bytes (32KB).
 */
#define CPAK_BANK_SIZE 32768

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
 * @brief Error codes for Controller Pak raw I/O operations.
 */
typedef enum {
    CPAK_IO_OK = 0,
    CPAK_IO_ERR_NO_PAK,
    CPAK_IO_ERR_ALLOC,
    CPAK_IO_ERR_FILE_OPEN,
    CPAK_IO_ERR_FILE_READ,
    CPAK_IO_ERR_FILE_WRITE,
    CPAK_IO_ERR_FILE_SEEK,
    CPAK_IO_ERR_FILE_FTELL,
    CPAK_IO_ERR_PROBE_BANKS,
    CPAK_IO_ERR_TOO_LARGE,
    CPAK_IO_ERR_PAK_READ,
    CPAK_IO_ERR_PAK_WRITE,
} cpak_io_err_t;

/**
 * @brief Context structure for detailed error information from Controller Pak I/O operations.
 *
 * This structure is optionally populated by cpak_restore_from_file() and cpak_backup_to_file()
 * to provide additional context for error messages.
 */
typedef struct {
    int failed_bank;        /**< Bank number where operation failed (-1 if not applicable) */
    int total_banks;        /**< Total banks in file (for restore operations) */
    int device_banks;       /**< Banks available on device (from cpak_probe_banks) */
    long filesize;          /**< File size in bytes (for restore operations) */
    size_t bytes_expected;  /**< Expected byte count for read/write operation */
    int bytes_actual;       /**< Actual byte count from read/write operation */
    int error_code;         /**< Raw error code from failed operation */
} cpak_io_context_t;

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

/**
 * @brief Restore a Controller Pak from a file.
 *
 * Reads a .pak file and writes its contents to the physical Controller Pak.
 * The caller is responsible for unmounting cpakfs before calling if needed.
 *
 * @param controller The controller index (0-3).
 * @param filepath The path to the .pak file to restore from.
 * @param ctx Optional pointer to context struct for detailed error info (can be NULL).
 * @return CPAK_IO_OK on success, or an error code on failure.
 */
cpak_io_err_t cpak_restore_from_file(int controller, const char *filepath, cpak_io_context_t *ctx);

/**
 * @brief Backup a Controller Pak to a file.
 *
 * Reads all banks from the physical Controller Pak and writes them to a file.
 *
 * @param controller The controller index (0-3).
 * @param filepath The path to the .pak file to write.
 * @param ctx Optional pointer to context struct for detailed error info (can be NULL).
 * @return CPAK_IO_OK on success, or an error code on failure.
 */
cpak_io_err_t cpak_backup_to_file(int controller, const char *filepath, cpak_io_context_t *ctx);

#endif // CPAKFS_UTILS__H__
