#ifndef UTILS_FS_H__
#define UTILS_FS_H__


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#define FS_SECTOR_SIZE      (512)


char *strip_fs_prefix (char *path);

bool file_exists (char *path);
int64_t file_get_size (char *path);
bool file_allocate (char *path, size_t size);
bool file_fill (char *path, uint8_t value);
bool file_has_extensions (char *path, const char *extensions[]);

bool directory_exists (char *path);
bool directory_create (char *path);


#endif
