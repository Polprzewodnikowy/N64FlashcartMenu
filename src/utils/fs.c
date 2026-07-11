
/**
 * @file fs.c
 * @brief Implementation of file system utility functions.
 */

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>

#include "fs.h"
#include "utils.h"

/**
 * @brief Strip the file system prefix from a path.
 *
 * Removes the file system prefix (such as ":/") from the provided path string.
 *
 * @param path The path from which to strip the prefix.
 * @return A pointer to the path without the prefix.
 */
char *strip_fs_prefix(char *path) {
    const char *prefix = ":/";
    char *found = strstr(path, prefix);
    if (found) {
        return (found + strlen(prefix) - 1);
    }
    return path;
}

/**
 * @brief Get the basename of a path.
 *
 * Returns a pointer to the basename (the final component) of the provided path.
 *
 * @param path The path from which to get the basename.
 * @return A pointer to the basename of the path.
 */
char *file_basename(char *path) {
    char *base = strrchr(path, '/');
    return base ? base + 1 : path;
}

/**
 * @brief Check if a file exists at the given path.
 *
 * Checks if a file exists at the specified path.
 *
 * @param path The path to the file.
 * @return true if the file exists, false otherwise.
 */
bool file_exists(char *path) {
    struct stat st;
    int error = stat(path, &st);
    return ((error == 0) && S_ISREG(st.st_mode));
}

/**
 * @brief Get the size of a file at the given path.
 *
 * Returns the size of the file at the specified path in bytes.
 *
 * @param path The path to the file.
 * @return The size of the file in bytes, or -1 if the file does not exist or an error occurs.
 */
int64_t file_get_size(char *path) {
    struct stat st;
    if (stat(path, &st)) {
        return -1;
    }
    return (int64_t)(st.st_size);
}

/**
 * @brief Allocate a file of the specified size at the given path.
 *
 * Creates a file of the specified size at the provided path. The file is filled with zeros.
 *
 * @param path The path to the file.
 * @param size The size of the file to create in bytes.
 * @return true if the file was successfully created, false otherwise.
 */
bool file_allocate(char *path, size_t size) {
    FILE *f;
    if ((f = fopen(path, "wb")) == NULL) {
        return true;
    }
    if (fseek(f, size, SEEK_SET)) {
        fclose(f);
        return true;
    }
    if (ftell(f) != size) {
        fclose(f);
        return true;
    }
    if (fclose(f)) {
        return true;
    }
    return false;
}

/**
 * @brief Fill a file with the specified value.
 *
 * Fills the file at the given path with the specified byte value.
 *
 * @param path The path to the file.
 * @param value The value to fill the file with (byte).
 * @return true if the file was successfully filled, false otherwise.
 */
bool file_fill(char *path, uint8_t value) {
    FILE *f;
    bool error = false;
    uint8_t buffer[FS_SECTOR_SIZE * 8];
    size_t bytes_to_write;

    for (int i = 0; i < sizeof(buffer); i++) {
        buffer[i] = value;
    }

    if ((f = fopen(path, "rb+")) == NULL) {
        return true;
    }

    setbuf(f, NULL);

    fseek(f, 0, SEEK_END);
    size_t size = ftell(f);
    fseek(f, 0, SEEK_SET);

    for (size_t i = 0; i < size; i += sizeof(buffer)) {
        bytes_to_write = MIN(size - ftell(f), sizeof(buffer));
        if (fwrite(buffer, 1, bytes_to_write, f) != bytes_to_write) {
            error = true;
            break;
        }
    }

    if (fclose(f)) {
        error = true;
    }

    return error;
}

/**
 * @brief Check if a file has one of the specified extensions.
 *
 * Checks if the file at the given path has one of the specified extensions.
 *
 * @param path The path to the file.
 * @param extensions An array of extensions to check (NULL-terminated).
 * @return true if the file has one of the specified extensions, false otherwise.
 */
bool file_has_extensions(char *path, const char *extensions[]) {
    if ((path == NULL) || (extensions == NULL)) {
        return false;
    }

    // Use max+1 so the sentinel result is unambiguous:
    // return value == (max+1) means unterminated/too-long input.
    size_t path_len = strnlen(path, FS_MAX_PATH_SCAN_LENGTH + 1);
    if (path_len == FS_MAX_PATH_SCAN_LENGTH + 1) {
        return false;
    }

    char *ext = NULL;
    for (size_t i = path_len; i > 0; i--) {
        if (path[i - 1] == '.') {
            ext = &path[i - 1];
            break;
        }
    }

    if (ext == NULL) {
        return false;
    }

    size_t ext_len = strnlen(ext + 1, FS_MAX_EXTENSION_LENGTH);
    if ((ext_len == 0) || (ext_len == FS_MAX_EXTENSION_LENGTH)) {
        return false;
    }

    while (*extensions != NULL) {
        size_t candidate_len = strnlen(*extensions, FS_MAX_EXTENSION_LENGTH);
        if ((candidate_len == ext_len) && (candidate_len != FS_MAX_EXTENSION_LENGTH) &&
            (strncasecmp(ext + 1, *extensions, ext_len) == 0)) {
            return true;
        }
        extensions++;
    }

    return false;
}

/**
 * @brief Check if a directory exists at the given path.
 *
 * Checks if a directory exists at the specified path.
 *
 * @param path The path to the directory.
 * @return true if the directory exists, false otherwise.
 */
bool directory_exists(char *path) {
    struct stat st;
    int error = stat(path, &st);
    return ((error == 0) && S_ISDIR(st.st_mode));
}

/**
 * @brief Create a directory at the given path.
 *
 * Creates a directory at the specified path, including any necessary parent directories.
 *
 * @param path The path to the directory.
 * @return false if the directory was successfully created, true if there was an error.
 */
bool directory_create(char *path) {
    bool error = false;

    if (directory_exists(path)) {
        return false;
    }

    char *directory = strdup(path);
    if (directory == NULL) {
        return true;
    }

    char *separator = strip_fs_prefix(directory);

    if ((separator != directory) && (*separator == '/')) {
        separator++;
    }

    for (;;) {
        while ((*separator != '\0') && (*separator != '/')) {
            separator++;
        }

        char terminator = *separator;
        *separator = '\0';

        if (directory[0] != '\0') {
            if (mkdir(directory, 0777) && (errno != EEXIST)) {
                error = true;
                break;
            }
        }

        if (terminator == '\0') {
            break;
        }

        *separator++ = terminator;
    }

    free(directory);

    return error;
}
