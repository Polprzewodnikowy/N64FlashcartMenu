#ifndef UTILS_FS_H__
#define UTILS_FS_H__


#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


#define FS_SECTOR_SIZE      (512)


char *strip_sd_prefix (char *path);

bool file_exists (char *path);
size_t file_get_size (char *path);
bool file_delete (char *path);
bool file_allocate (char *path, size_t size);
bool file_fill (char *path, uint8_t value);
bool file_get_sectors (char *path, void (*callback) (uint32_t sector_count, uint32_t file_sector, uint32_t cluster_sector, uint32_t cluster_size));
bool file_has_extensions (char *path, const char *extensions[]);

bool directory_exists (char *path);
bool directory_delete (char *path);
bool directory_create (char *path);


#endif
