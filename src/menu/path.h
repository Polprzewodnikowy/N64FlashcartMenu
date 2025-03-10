/**
 * @file path.h
 * @brief Menu Path
 * @ingroup menu 
 */

#ifndef PATH_H__
#define PATH_H__

#include <stdbool.h>
#include <stdlib.h>

/** 
 * @brief Path Structure 
 */
typedef struct {
    char *buffer;    /**< Buffer for the path */
    char *root;      /**< Root directory */
    size_t capacity; /**< Capacity of the buffer */
} path_t;

/**
 * @brief Create a new path object
 * 
 * @param string Initial path string
 * @return path_t* Pointer to the created path object
 */
path_t *path_create(const char *string);

/**
 * @brief Initialize a path object with a prefix and string
 * 
 * @param prefix Prefix for the path
 * @param string Initial path string
 * @return path_t* Pointer to the initialized path object
 */
path_t *path_init(const char *prefix, char *string);

/**
 * @brief Free a path object
 * 
 * @param path Pointer to the path object to be freed
 */
void path_free(path_t *path);

/**
 * @brief Clone a path object
 * 
 * @param string Path object to be cloned
 * @return path_t* Pointer to the cloned path object
 */
path_t *path_clone(path_t *string);

/**
 * @brief Clone a path object and push a string onto it
 * 
 * @param path Path object to be cloned
 * @param string String to be pushed onto the cloned path
 * @return path_t* Pointer to the cloned and modified path object
 */
path_t *path_clone_push(path_t *path, char *string);

/**
 * @brief Get the string representation of a path
 * 
 * @param path Path object
 * @return char* String representation of the path
 */
char *path_get(path_t *path);

/**
 * @brief Get the last component of a path
 * 
 * @param path Path object
 * @return char* Last component of the path
 */
char *path_last_get(path_t *path);

/**
 * @brief Check if the path is the root directory
 * 
 * @param path Path object
 * @return true If the path is the root directory
 * @return false Otherwise
 */
bool path_is_root(path_t *path);

/**
 * @brief Pop the last component from the path
 * 
 * @param path Path object
 */
void path_pop(path_t *path);

/**
 * @brief Push a string onto the path
 * 
 * @param path Path object
 * @param string String to be pushed onto the path
 */
void path_push(path_t *path, char *string);

/**
 * @brief Push a subdirectory onto the path
 * 
 * @param path Path object
 * @param string Subdirectory to be pushed onto the path
 */
void path_push_subdir(path_t *path, char *string);

/**
 * @brief Get the file extension from the path
 * 
 * @param path Path object
 * @return char* File extension
 */
char *path_ext_get(path_t *path);

/**
 * @brief Remove the file extension from the path
 * 
 * @param path Path object
 */
void path_ext_remove(path_t *path);

/**
 * @brief Replace the file extension of the path
 * 
 * @param path Path object
 * @param ext New file extension
 */
void path_ext_replace(path_t *path, char *ext);

/**
 * @brief Check if the path has a value
 * 
 * @param path Path object
 * @return true If the path has a value
 * @return false Otherwise
 */
bool path_has_value(path_t *path);

/**
 * @brief Check if two paths match
 * 
 * @param left First path object
 * @param right Second path object
 * @return true If the paths match
 * @return false Otherwise
 */
bool path_are_match(path_t *left, path_t *right);

#endif // PATH_H__
