#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "path.h"


#define PATH_CAPACITY_INITIAL   255
#define PATH_CAPACITY_ALIGNMENT 32


static void path_resize (path_t *path, size_t min_length) {
    path->capacity = min_length > PATH_CAPACITY_INITIAL ? min_length : PATH_CAPACITY_INITIAL;
    size_t alignment = path->capacity % PATH_CAPACITY_ALIGNMENT;
    if (alignment != (PATH_CAPACITY_ALIGNMENT - 1)) {
        path->capacity += PATH_CAPACITY_ALIGNMENT - alignment;
    }
    path->buffer = realloc(path->buffer, (path->capacity + 1) * sizeof(char));
    assert(path->buffer != NULL);
}

static path_t *path_create (char *string) {
    if (string == NULL) {
        string = "";
    }
    path_t *path = calloc(1, sizeof(path_t));
    assert(path != NULL);
    path_resize(path, strlen(string));
    memset(path->buffer, 0, path->capacity + 1);
    strcpy(path->buffer, string);
    path->root = path->buffer;
    return path;
}

static void path_append (path_t *path, char *string) {
    size_t buffer_length = strlen(path->buffer);
    size_t string_length = strlen(string);
    size_t new_path_length = buffer_length + string_length;
    if (new_path_length > path->capacity) {
        path_resize(path, new_path_length);
    }
    strcat(path->buffer, string);
}


path_t *path_init (char *prefix, char *string) {
    path_t *path = path_create(prefix);
    size_t prefix_length = strlen(prefix);
    if ((prefix_length > 0) && (prefix[prefix_length - 1] == '/')) {
        path->root = path->buffer + prefix_length - 1;
    } else {
        path->root = path->buffer + prefix_length;
        path_append(path, "/");
    }
    path_push(path, string);
    return path;
}

void path_free (path_t *path) {
    if (path != NULL) {
        free(path->buffer);
        free(path);
    }
}

path_t *path_clone (path_t *path) {
    path_t *cloned = path_create(path->buffer);
    cloned->root = cloned->buffer + (path->root - path->buffer);
    return cloned;
}

path_t *path_clone_push (path_t *path, char *string) {
    path_t *cloned = path_clone(path);
    path_push(cloned, string);
    return cloned;
}

char *path_get (path_t *path) {
    return path->buffer;
}

char *path_last_get (path_t *path) {
    char *last_slash = strrchr(path->root, '/');
    return (last_slash == NULL) ? path->root : (last_slash + 1);
}

bool path_is_root (path_t *path) {
    return (strcmp(path->root, "/") == 0);
}

void path_pop (path_t *path) {
    if (path_is_root(path)) {
        return;
    }
    char *last_slash = strrchr(path->root, '/');
    if (last_slash == path->root) {
        *(last_slash + 1) = '\0';
    } else if (last_slash != NULL) {
        *last_slash = '\0';
    }
}

void path_push (path_t *path, char *string) {
    if (path->buffer[strlen(path->buffer) - 1] != '/') {
        path_append(path, "/");
    }
    if (string[0] == '/') {
        string += 1;
    }
    path_append(path, string);
}

void path_push_subdir (path_t *path, char *string) {
    char *file = path_last_get(path);
    char *tmp = alloca(strlen(file) + 1);
    strcpy(tmp, file);
    path_pop(path);
    path_push(path, string);
    path_push(path, tmp);
}

char *path_ext_get (path_t *path) {
    char *buffer = path_last_get(path);
    char *last_dot = strrchr(buffer, '.');
    if (last_dot != NULL) {
        return last_dot + 1;
    }
    return NULL;
}

void path_ext_remove (path_t *path) {
    char *buffer = path_last_get(path);
    char *last_dot = strrchr(buffer, '.');
    if (last_dot != NULL) {
        *last_dot = '\0';
    }
}

void path_ext_replace (path_t *path, char *ext) {
    path_ext_remove(path);
    path_append(path, ".");
    path_append(path, ext);
}
