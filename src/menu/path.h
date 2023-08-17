/**
 * @file path.h
 * @brief Menu Path
 * @ingroup menu 
 */

#ifndef PATH_H__
#define PATH_H__


#include <stdbool.h>
#include <stdlib.h>


/** @brief Path Structure */
typedef struct {
    char *buffer;
    char *root;
    size_t capacity;
} path_t;


path_t *path_init (char *prefix, char *string);
void path_free (path_t *path);
path_t *path_clone (path_t *string);
path_t *path_clone_push (path_t *path, char *string);
char *path_get (path_t *path);
char *path_last_get (path_t *path);
bool path_is_root (path_t *path);
void path_pop (path_t *path);
void path_push (path_t *path, char *string);
void path_push_subdir (path_t *path, char *string);
char *path_ext_get (path_t *path);
void path_ext_remove (path_t *path);
void path_ext_replace (path_t *path, char *ext);


#endif
