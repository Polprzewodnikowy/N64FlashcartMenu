#ifndef UTILS_FS_H__
#define UTILS_FS_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define FS_SECTOR_SIZE      (512)

/**
 * @file fs.h
 * @brief File system utility functions.
 * @ingroup utils
 */

/**
 * @brief Strips the file system prefix from the given path.
 *
 * This function removes the file system prefix from the provided path.
 *
 * @param path The path from which to strip the prefix.
 * @return A pointer to the path without the prefix.
 */
char *strip_fs_prefix(char *path);

/**
 * @brief Gets the basename of the given path.
 *
 * This function returns the basename of the provided path.
 *
 * @param path The path from which to get the basename.
 * @return A pointer to the basename of the path.
 */
char *file_basename(char *path);

/**
 * @brief Checks if a file exists at the given path.
 *
 * This function checks if a file exists at the specified path.
 *
 * @param path The path to the file.
 * @return true if the file exists, false otherwise.
 */
bool file_exists(char *path);

/**
 * @brief Gets the size of the file at the given path.
 *
 * This function returns the size of the file at the specified path.
 *
 * @param path The path to the file.
 * @return The size of the file in bytes, or -1 if the file does not exist.
 */
int64_t file_get_size(char *path);

/**
 * @brief Allocates a file of the specified size at the given path.
 *
 * This function creates a file of the specified size at the provided path.
 *
 * @param path The path to the file.
 * @param size The size of the file to create.
 * @return true if the file was successfully created, false otherwise.
 */
bool file_allocate(char *path, size_t size);

/**
 * @brief Fills a file with the specified value.
 *
 * This function fills the file at the given path with the specified value.
 *
 * @param path The path to the file.
 * @param value The value to fill the file with.
 * @return true if the file was successfully filled, false otherwise.
 */
bool file_fill(char *path, uint8_t value);

/**
 * @brief Checks if a file has one of the specified extensions.
 *
 * This function checks if the file at the given path has one of the specified extensions.
 *
 * @param path The path to the file.
 * @param extensions An array of extensions to check.
 * @return true if the file has one of the specified extensions, false otherwise.
 */
bool file_has_extensions(char *path, const char *extensions[]);

/**
 * @brief Checks if a directory exists at the given path.
 *
 * This function checks if a directory exists at the specified path.
 *
 * @param path The path to the directory.
 * @return true if the directory exists, false otherwise.
 */
bool directory_exists(char *path);

/**
 * @brief Creates a directory at the given path.
 *
 * This function creates a directory at the specified path.
 *
 * @param path The path to the directory.
 * @return true if the directory was successfully created, false otherwise.
 */
bool directory_create(char *path);

#endif // UTILS_FS_H__
