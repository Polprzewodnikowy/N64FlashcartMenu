#ifndef ZIP_ENTRY_COUNT_H__
#define ZIP_ENTRY_COUNT_H__

#include <stdbool.h>
#include <stdint.h>

/**
 * Try to read ZIP entry count using EOCD/Zip64 records without initializing
 * full archive structures. Returns true on success.
 */
bool zip_try_read_entry_count(const char *zip_path, uint64_t *entry_count);

#endif // ZIP_ENTRY_COUNT_H__
