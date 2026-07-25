#include <stdio.h>
#include <stdlib.h>

#include "zip_entry_count.h"

#define ZIP_EOCD_MIN_SIZE 22u
#define ZIP_EOCD_MAX_SEARCH (ZIP_EOCD_MIN_SIZE + 0xFFFFu)
#define ZIP_EOCD_SIG 0x06054B50u
#define ZIP64_EOCD_SIG 0x06064B50u
#define ZIP64_LOCATOR_SIG 0x07064B50u
#define ZIP16_SENTINEL 0xFFFFu

enum {
    ZIP_EOCD_ENTRIES_THIS_DISK_OFF = 10,
    ZIP_EOCD_ENTRIES_TOTAL_OFF = 12,
    ZIP_EOCD_COMMENT_LEN_OFF = 20,
    ZIP64_LOCATOR_SIZE = 20,
    ZIP64_LOCATOR_EOCD_OFFSET_OFF = 8,
    ZIP64_EOCD_MIN_READ_SIZE = 56,
    ZIP64_EOCD_ENTRIES_TOTAL_OFF = 32,
};

static uint16_t read_le16(const uint8_t *p) {
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static uint32_t read_le32(const uint8_t *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static uint64_t read_le64(const uint8_t *p) {
    return (uint64_t)read_le32(p) | ((uint64_t)read_le32(p + 4) << 32);
}

static bool zip_eocd_ends_at_eof(const uint8_t *tail, size_t tail_size, size_t eocd_pos) {
    if (eocd_pos + ZIP_EOCD_MIN_SIZE > tail_size) {
        return false;
    }

    uint16_t comment_len = read_le16(&tail[eocd_pos + ZIP_EOCD_COMMENT_LEN_OFF]);
    size_t eocd_end = eocd_pos + ZIP_EOCD_MIN_SIZE + (size_t)comment_len;
    return eocd_end == tail_size;
}

bool zip_try_read_entry_count(const char *zip_path, uint64_t *entry_count) {
    if (!zip_path || !entry_count) {
        return false;
    }

    FILE *f = fopen(zip_path, "rb");
    if (!f) {
        return false;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return false;
    }

    long file_size_signed = ftell(f);
    if (file_size_signed < (long)ZIP_EOCD_MIN_SIZE) {
        fclose(f);
        return false;
    }

    size_t file_size = (size_t)file_size_signed;
    size_t tail_size = file_size < ZIP_EOCD_MAX_SEARCH ? file_size : ZIP_EOCD_MAX_SEARCH;
    size_t tail_offset = file_size - tail_size;

    if (fseek(f, (long)tail_offset, SEEK_SET) != 0) {
        fclose(f);
        return false;
    }

    uint8_t *tail = malloc(tail_size);
    if (!tail) {
        fclose(f);
        return false;
    }

    bool ok = false;
    if (fread(tail, 1, tail_size, f) != tail_size) {
        free(tail);
        fclose(f);
        return false;
    }

    long eocd_pos = -1;
    for (long i = (long)tail_size - (long)ZIP_EOCD_MIN_SIZE; i >= 0; i--) {
        if (read_le32(&tail[i]) == ZIP_EOCD_SIG && zip_eocd_ends_at_eof(tail, tail_size, (size_t)i)) {
            eocd_pos = i;
            break;
        }
    }

    if (eocd_pos >= 0) {
        uint16_t entries16 = read_le16(&tail[eocd_pos + ZIP_EOCD_ENTRIES_THIS_DISK_OFF]);
        uint16_t entries_total16 = read_le16(&tail[eocd_pos + ZIP_EOCD_ENTRIES_TOTAL_OFF]);
        if (entries16 != ZIP16_SENTINEL && entries_total16 != ZIP16_SENTINEL) {
            *entry_count = entries_total16;
            ok = true;
        } else if (eocd_pos >= ZIP64_LOCATOR_SIZE && read_le32(&tail[eocd_pos - ZIP64_LOCATOR_SIZE]) == ZIP64_LOCATOR_SIG) {
            uint64_t zip64_eocd_offset = read_le64(&tail[eocd_pos - ZIP64_LOCATOR_SIZE + ZIP64_LOCATOR_EOCD_OFFSET_OFF]);
            if (fseek(f, (long)zip64_eocd_offset, SEEK_SET) == 0) {
                uint8_t zip64_eocd[ZIP64_EOCD_MIN_READ_SIZE];
                if (fread(zip64_eocd, 1, sizeof(zip64_eocd), f) == sizeof(zip64_eocd) &&
                    read_le32(&zip64_eocd[0]) == ZIP64_EOCD_SIG) {
                    *entry_count = read_le64(&zip64_eocd[ZIP64_EOCD_ENTRIES_TOTAL_OFF]);
                    ok = true;
                }
            }
        }
    }

    free(tail);
    fclose(f);
    return ok;
}
