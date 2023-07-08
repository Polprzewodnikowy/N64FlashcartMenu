#ifndef PAHT_H__
#define PATH_H__


#include <stdbool.h>


typedef struct {
    char *buffer;
    size_t capacity;
} path_t;


path_t *path_init (char *string);
void path_free (path_t *path);
path_t *path_clone (path_t *string);
char *path_get (path_t *path);
char *path_last_get (path_t *path);
bool path_is_root (path_t *path);
void path_append (path_t *path, char *string);
void path_concat (path_t *dst, path_t *str);
void path_push (path_t *path, char *string);
void path_pop (path_t *path);
char *path_ext_get (path_t *path);
void path_ext_remove (path_t *path);
void path_ext_replace (path_t *path, char *ext);


#endif
