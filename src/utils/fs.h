#ifndef UTILS_FS_H__
#define UTILS_FS_H__


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#define FS_SECTOR_SIZE      (512)


bool file_exists (char *path);
bool directory_exists (char *path);
size_t file_get_size (char *path);
bool file_allocate (char *path, size_t size);
bool file_get_sectors (char *path, uint32_t *sectors, size_t entries);
bool file_has_extensions (char *path, const char *extensions[]);


#endif
