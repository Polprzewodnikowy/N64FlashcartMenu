
/**
 * @file cpakfs_utils.c
 * @brief Implementation of Controller Pak file system utilities.
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <libdragon.h>
#include <fatfs/ff.h>
#include <errno.h>
#include <dir.h>
#include "cpakfs_utils.h"

/**
 * @brief Array of Controller Pak mount point strings.
 */
const char * const CPAK_MOUNT_ARRAY[4] = {
    "cpak1:/", "cpak2:/", "cpak3:/", "cpak4:/"
};

/**
 * @brief Check if a Controller Pak is present in the given controller.
 *
 * @param controller The controller index (0-3).
 * @return true if a Controller Pak is present, false otherwise.
 */
bool has_cpak(int controller) {
    joypad_accessory_type_t val = joypad_get_accessory_type(controller);
    return val == JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK;
}

/**
 * @brief Get the block size from a Controller Pak file system path.
 *
 * @param filename_cpak The Controller Pak file path.
 * @return The block size, or -1 on error.
 */
int get_block_size_from_fs_path(const char *filename_cpak) {
    return get_file_size_from_fs_path(filename_cpak) / (MEMPAK_BLOCK_SIZE);
}

/**
 * @brief Get the file size from a Controller Pak file system path.
 *
 * @param filename_cpak The Controller Pak file path.
 * @return The file size in bytes, or -1 on error.
 */
int get_file_size_from_fs_path(const char *filename_cpak) {
    FILE *f = fopen(filename_cpak, "rb");
    if (!f) {
        return -1;
    }
    fseek(f, 0L, SEEK_END);
    int sz = ftell(f);
    fclose(f);
    return sz;
}

/**
 * @brief Unmount all Controller Pak file systems.
 */
void unmount_all_cpakfs() {
    for (int i = 0; i < 4; i++) {
        int val = cpakfs_unmount(i);
        if (val < 0) {
            debugf("Failed to unmount cpakfs on port %d: %d\n", i+1, val);
        } else {
            debugf("Unmounted cpakfs on port %d\n", i+1);
        }
    }
}

/**
 * @brief Increment a note index, wrapping around MAX_NUM_NOTES.
 *
 * @param current_index The current note index.
 * @return The incremented note index.
 */
int inc_index_note(int current_index) {
    return ((current_index + 1) + MAX_NUM_NOTES) % MAX_NUM_NOTES;
}

/**
 * @brief Decrement a note index, wrapping around MAX_NUM_NOTES.
 *
 * @param current_index The current note index.
 * @return The decremented note index.
 */
int dec_index_note(int current_index) {
    return ((current_index - 1) + MAX_NUM_NOTES) % MAX_NUM_NOTES;
}

/**
 * @brief Mount the Controller Pak file system for a given controller.
 *
 * @param controller The controller index (0-3).
 * @return 0 on success, -1 on failure.
 */
int mount_cpakfs(int controller) {
    if (cpakfs_mount(controller, CPAK_MOUNT_ARRAY[controller]) < 0) {
        return -1;
    } else {
        return 0;
    }
}

/**
 * @brief Extract the title from an absolute file path.
 *
 * The title is the filename up to the first underscore or dot, or the whole filename if neither is present.
 *
 * @param path The absolute file path.
 * @param outbuf Output buffer for the title.
 * @param outbuf_size Size of the output buffer.
 * @return 0 on success, negative value on error.
 */
int extract_title_from_absolute_path(const char *path, char *outbuf, size_t outbuf_size) {
    // 1. Find start of filename
    const char *fname = strrchr(path, '/');
    if (!fname) fname = path;
    else fname++;

    // 2. Find first underscore after filename
    const char *uscore = strchr(fname, '_');
    const char *dot = strrchr(fname, '.');  // last dot -> extension

    const char *end;

    if (uscore) {
        // Case with timestamp: cut at underscore
        end = uscore;
    } else if (dot) {
        // Case without timestamp: cut before extension
        end = dot;
    } else {
        // No underscore, no dot -> whole filename
        end = fname + strlen(fname);
    }

    // 3. Copy substring
    size_t len = (size_t)(end - fname);
    if (len >= outbuf_size) len = outbuf_size - 1;
    memcpy(outbuf, fname, len);
    outbuf[len] = '\0';
    return 0;
}

/**
 * @brief Parse a Controller Pak full filename into its components.
 *
 * @param fullname The full filename string.
 * @param out Output struct for parsed components.
 * @return 0 on success, negative value on error.
 */
int parse_cpakfs_fullname(const char *fullname, cpakfs_path_strings_t *out) {
    const char *dot1 = strchr(fullname, '.');
    if (!dot1) return -1; // no first dot
    const char *dash = strchr(dot1 + 1, '-');
    if (!dash) return -2; // no dash
    const char *dot2 = strrchr(dash + 1, '.'); // last dot (if any)

    // Game code (4 chars)
    if (dot1 - fullname != 4) return -3;
    strncpy(out->gamecode, fullname, 4);
    out->gamecode[4] = '\0';

    // Publisher code (2 or 4 chars)
    size_t publen = dash - (dot1 + 1);
    if (publen != 2 && publen != 4) return -4;
    if (publen > sizeof(out->pubcode) - 1) publen = sizeof(out->pubcode) - 1;
    strncpy(out->pubcode, dot1 + 1, publen);
    out->pubcode[publen] = '\0';

    // Filename
    if (dot2) {
        size_t fnlen = dot2 - (dash + 1);
        if (fnlen > 16) fnlen = 16;
        strncpy(out->filename, dash + 1, fnlen);
        out->filename[fnlen] = '\0';

        // Extension
        strncpy(out->ext, dot2 + 1, 4);
        out->ext[4] = '\0';
    } else {
        // No extension
        strncpy(out->filename, dash + 1, 16);
        out->filename[16] = '\0';
        out->ext[0] = '\0';
    }

    return 0;
}

/**
 * @brief Parse a Controller Pak full filename into its components (strict version).
 *
 * @param s The full filename string.
 * @param out Output struct for parsed components.
 * @return 0 on success, negative value on error.
 */
static int parse_fullname(const char *s, cpakfs_path_strings_t *out) {
    const char *dot1 = strchr(s, '.');          if (!dot1 || dot1 - s != 4) return -1;
    const char *dash = strchr(dot1 + 1, '-');   if (!dash || dash - (dot1+1) != 2) return -2;
    const char *dot2 = strrchr(dash + 1, '.');  // may be NULL

    memcpy(out->gamecode, s, 4); out->gamecode[4] = '\0';
    memcpy(out->pubcode, dot1+1, 2); out->pubcode[2] = '\0';

    if (dot2 && dot2 > dash) {
        size_t fnlen = (size_t)(dot2 - (dash + 1)); if (fnlen > 16) fnlen = 16;
        memcpy(out->filename, dash+1, fnlen); out->filename[fnlen] = '\0';
        size_t exlen = strnlen(dot2 + 1, 4);
        memcpy(out->ext, dot2+1, exlen); out->ext[exlen] = '\0';
    } else {
        size_t fnlen = strnlen(dash + 1, 16);
        memcpy(out->filename, dash+1, fnlen); out->filename[fnlen] = '\0';
        out->ext[0] = '\0';
    }
    return 0;
}

/**
 * @brief Format a Controller Pak path struct into a full filename string.
 *
 * @param in Input struct with path components.
 * @param out Output buffer for the formatted filename.
 * @param outsz Size of the output buffer.
 * @return 0 on success, negative value on error.
 */
static int format_fullname(const cpakfs_path_strings_t *in, char *out, size_t outsz) {
    if (strlen(in->gamecode) != 4 || strlen(in->pubcode) != 2) return -1;
    size_t fn = strnlen(in->filename, 16);
    size_t ex = strnlen(in->ext, 4);
    size_t need = 4 + 1 + 2 + 1 + fn + (ex ? (1 + ex) : 0) + 1;
    if (need > outsz) return -2;

    char *p = out;
    memcpy(p, in->gamecode, 4); p += 4; *p++ = '.';
    memcpy(p, in->pubcode, 2);  p += 2; *p++ = '-';
    if (fn) { memcpy(p, in->filename, fn); p += fn; }
    if (ex) { *p++ = '.'; memcpy(p, in->ext, ex); p += ex; }
    *p = '\0';
    return 0;
}

// --- base36 helpers for extension bumping ---

/**
 * @brief Check if a string is a valid base36 string (0-9, A-Z).
 *
 * @param s The string to check.
 * @return 1 if valid, 0 otherwise.
 */
static int is_base36_str(const char *s) {
    if (!*s) return 0;
    for (const unsigned char *p = (const unsigned char *)s; *p; ++p)
        if (!((*p >= '0' && *p <= '9') || (*p >= 'A' && *p <= 'Z'))) return 0;
    return 1;
}

/**
 * @brief Convert a base36 string to an unsigned integer.
 *
 * @param s The base36 string.
 * @return The converted unsigned integer.
 */
static unsigned base36_to_uint(const char *s) {
    unsigned v = 0;
    for (const unsigned char *p = (const unsigned char *)s; *p; ++p) {
        v *= 36;
        v += (*p <= '9' ? *p - '0' : *p - 'A' + 10);
    }
    return v;
}

/**
 * @brief Convert an unsigned integer to a base36 string.
 *
 * @param n The unsigned integer.
 * @param out Output buffer for the base36 string.
 * @param maxlen Maximum length of the output buffer.
 */
static void uint_to_base36(unsigned n, char *out, size_t maxlen) {
    char buf[8]; // enough for up to 4 digits (36^4)
    int i = 0;
    // produce at least one digit
    do {
        unsigned d = n % 36;
        buf[i++] = (char)(d < 10 ? '0' + d : 'A' + (d - 10));
        n /= 36;
    } while (n && i < (int)sizeof(buf));
    int digits = i;
    if ((size_t)digits > maxlen) digits = (int)maxlen;
    for (int k = 0; k < digits; ++k) {
        out[k] = buf[digits - 1 - k];
    }
    out[digits] = '\0';
}

/**
 * @brief Safely join a mount prefix and name into a full path.
 *
 * @param mount The mount prefix (e.g., "cpak1:/").
 * @param name The file name.
 * @param out Output buffer for the full path.
 * @param outsz Size of the output buffer.
 * @return 0 on success, negative value on error.
 */
static int join_mount_name(const char *mount, const char *name, char *out, size_t outsz) {
    size_t m = strlen(mount), n = strlen(name);
    int need_sep = (m > 0 && mount[m - 1] != '/' && mount[m - 1] != ':');
    size_t need = m + (need_sep ? 1 : 0) + n + 1;
    if (need > outsz) return -1;
    char *p = out;
    memcpy(p, mount, m); p += m;
    if (need_sep) { *p++ = '/'; }
    memcpy(p, name, n); p += n;
    *p = '\0';
    return 0;
}

/**
 * @brief Pick a unique full filename with mount prefix, avoiding collisions.
 *
 * This function attempts to find a unique filename by incrementing the extension in base36 if needed.
 *
 * @param mount_prefix The mount prefix (e.g., "cpak1:/").
 * @param desired_name The desired base filename.
 * @param out_fullpath Output buffer for the unique full path.
 * @param outsz Size of the output buffer.
 * @param exists_fullpath Function pointer to check if a full path exists.
 * @return 0 on success, negative value on error.
 */
int pick_unique_fullname_with_mount(const char *mount_prefix,
                                    const char *desired_name,
                                    char *out_fullpath, size_t outsz,
                                    int (*exists_fullpath)(const char *fullpath))
{
    cpakfs_path_strings_t want;
    if (parse_fullname(desired_name, &want) != 0) return -1;

    /* If desired is free (with mount), use it */
    char test_full[256];
    if (join_mount_name(mount_prefix, desired_name, test_full, sizeof test_full) != 0) return -2;
    if (!exists_fullpath(test_full)) {
        // Copy full path (with prefix) out
        size_t need = strlen(test_full) + 1;
        if (need > outsz) return -3;
        memcpy(out_fullpath, test_full, need);
        return 0;
    }

    // Build base without extension
    cpakfs_path_strings_t base = want; base.ext[0] = '\0';
    char base_name[128];
    if (format_fullname(&base, base_name, sizeof base_name) != 0) return -4;

    // Starting counter
    unsigned start = is_base36_str(want.ext) ? (base36_to_uint(want.ext) + 1) : 0;

    const unsigned MAX = 36 + 36*36 + 36*36*36 + 36*36*36*36;
    for (unsigned k = 0; k < MAX; ++k) {
        unsigned cand = start + k;
        char ext[5];
        if (cand < 36U)                 uint_to_base36(cand, ext, 1);
        else if (cand < 36U*36U)        uint_to_base36(cand, ext, 2);
        else if (cand < 36U*36U*36U)    uint_to_base36(cand, ext, 3);
        else                            uint_to_base36(cand, ext, 4);

        cpakfs_path_strings_t candp = base;
        strncpy(candp.ext, ext, sizeof candp.ext); candp.ext[sizeof candp.ext - 1] = '\0';

        char cand_name[128];
        if (format_fullname(&candp, cand_name, sizeof cand_name) != 0) continue;

        char cand_full[256];
        if (join_mount_name(mount_prefix, cand_name, cand_full, sizeof cand_full) != 0) continue;

        if (!exists_fullpath(cand_full)) {
            size_t need = strlen(cand_full) + 1;
            if (need > outsz) return -5;
            memcpy(out_fullpath, cand_full, need);
            return 0;
        }
    }
    return -6; // exhausted (unlikely)
}

/**
 * @brief Check if a file exists at the given full mounted path.
 *
 * @param full_mounted_path The full mounted file path.
 * @return 1 if the file exists, 0 otherwise.
 */
int file_exists_full(const char *full_mounted_path) {
    FILE *f = fopen(full_mounted_path, "rb");
    if (f) { fclose(f); return 1; }
    return 0;
}

/**
 * @brief Initialize a cpak_io_context_t structure with default values.
 *
 * @param ctx Pointer to context structure to initialize.
 */
static void cpak_io_context_init(cpak_io_context_t *ctx) {
    if (ctx) {
        ctx->failed_bank = -1;
        ctx->total_banks = 0;
        ctx->device_banks = 0;
        ctx->filesize = 0;
        ctx->bytes_expected = 0;
        ctx->bytes_actual = 0;
        ctx->error_code = 0;
    }
}

/**
 * @brief Restore a Controller Pak from a file.
 *
 * Reads a .pak file and writes its contents to the physical Controller Pak.
 * The caller is responsible for unmounting cpakfs before calling if needed.
 *
 * @param controller The controller index (0-3).
 * @param filepath The path to the .pak file to restore from.
 * @param ctx Optional pointer to context struct for detailed error info (can be NULL).
 * @return CPAK_IO_OK on success, or an error code on failure.
 */
cpak_io_err_t cpak_restore_from_file(int controller, const char *filepath, cpak_io_context_t *ctx) {
    cpak_io_context_init(ctx);

    if (!has_cpak(controller)) {
        return CPAK_IO_ERR_NO_PAK;
    }

    uint8_t *data = malloc(CPAK_BANK_SIZE);
    if (!data) {
        return CPAK_IO_ERR_ALLOC;
    }

    FILE *fp = fopen(filepath, "rb");
    if (!fp) {
        free(data);
        return CPAK_IO_ERR_FILE_OPEN;
    }

    /* Get file size */
    if (fseek(fp, 0, SEEK_END) != 0) {
        fclose(fp);
        free(data);
        return CPAK_IO_ERR_FILE_SEEK;
    }
    long filesize = ftell(fp);
    if (filesize < 0) {
        fclose(fp);
        free(data);
        return CPAK_IO_ERR_FILE_FTELL;
    }
    rewind(fp);

    int total_banks = (int)((filesize + CPAK_BANK_SIZE - 1) / CPAK_BANK_SIZE);

    /* Check if file fits on physical pak */
    int banks_on_device = cpak_probe_banks(controller);

    /* Populate context with bank/file info */
    if (ctx) {
        ctx->total_banks = total_banks;
        ctx->device_banks = banks_on_device;
        ctx->filesize = filesize;
    }

    if (banks_on_device < 1) {
        if (ctx) {
            ctx->error_code = banks_on_device;
        }
        fclose(fp);
        free(data);
        return CPAK_IO_ERR_PROBE_BANKS;
    }
    if (total_banks > banks_on_device) {
        fclose(fp);
        free(data);
        return CPAK_IO_ERR_TOO_LARGE;
    }

    /* Write banks to physical pak */
    for (int bank = 0; bank < total_banks; bank++) {
        size_t bytes_read = fread(data, 1, CPAK_BANK_SIZE, fp);
        if (bytes_read == 0 && ferror(fp)) {
            if (ctx) {
                ctx->failed_bank = bank;
            }
            fclose(fp);
            free(data);
            return CPAK_IO_ERR_FILE_READ;
        }
        if (bytes_read == 0 && feof(fp)) {
            break;
        }

        int written = cpak_write((joypad_port_t)controller, (uint8_t)bank, 0, data, bytes_read);
        if (written < 0 || (size_t)written != bytes_read) {
            if (ctx) {
                ctx->failed_bank = bank;
                ctx->bytes_expected = bytes_read;
                ctx->bytes_actual = written;
                ctx->error_code = written;
            }
            fclose(fp);
            free(data);
            return CPAK_IO_ERR_PAK_WRITE;
        }
    }

    fclose(fp);
    free(data);
    return CPAK_IO_OK;
}

/**
 * @brief Backup a Controller Pak to a file.
 *
 * Reads all banks from the physical Controller Pak and writes them to a file.
 *
 * @param controller The controller index (0-3).
 * @param filepath The path to the .pak file to write.
 * @param ctx Optional pointer to context struct for detailed error info (can be NULL).
 * @return CPAK_IO_OK on success, or an error code on failure.
 */
cpak_io_err_t cpak_backup_to_file(int controller, const char *filepath, cpak_io_context_t *ctx) {
    cpak_io_context_init(ctx);

    if (!has_cpak(controller)) {
        return CPAK_IO_ERR_NO_PAK;
    }

    int banks = cpak_probe_banks(controller);
    if (ctx) {
        ctx->device_banks = banks;
    }
    if (banks < 1) {
        banks = 1;  /* Fallback to 1 bank */
    }

    uint8_t *data = malloc(CPAK_BANK_SIZE);
    if (!data) {
        return CPAK_IO_ERR_ALLOC;
    }

    FILE *fp = fopen(filepath, "wb");
    if (!fp) {
        free(data);
        return CPAK_IO_ERR_FILE_OPEN;
    }

    /* Read banks from physical pak and write to file */
    for (int bank = 0; bank < banks; bank++) {
        int bytes_read = cpak_read((joypad_port_t)controller, (uint8_t)bank, 0,
                                    data, CPAK_BANK_SIZE);
        if (bytes_read < 0 || bytes_read != CPAK_BANK_SIZE) {
            if (ctx) {
                ctx->failed_bank = bank;
                ctx->bytes_expected = (size_t)CPAK_BANK_SIZE;
                ctx->bytes_actual = bytes_read;
                ctx->error_code = (bytes_read < 0) ? errno : -1;
            }
            fclose(fp);
            free(data);
            return CPAK_IO_ERR_PAK_READ;
        }

        size_t written = fwrite(data, 1, CPAK_BANK_SIZE, fp);
        if (written != CPAK_BANK_SIZE) {
            if (ctx) {
                ctx->failed_bank = bank;
                ctx->bytes_expected = (size_t)CPAK_BANK_SIZE;
                ctx->bytes_actual = (int)written;
            }
            fclose(fp);
            free(data);
            return CPAK_IO_ERR_FILE_WRITE;
        }
    }

    fclose(fp);
    free(data);
    return CPAK_IO_OK;
}
