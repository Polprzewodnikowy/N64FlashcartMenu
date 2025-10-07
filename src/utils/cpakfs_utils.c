#include <stdbool.h>
#include <stdio.h>
#include <libdragon.h>
#include <fatfs/ff.h>
#include <errno.h>
#include <dir.h>
#include "cpakfs_utils.h"

const char * const CPAK_MOUNT_ARRAY[4] = {
    "cpak1:/", "cpak2:/", "cpak3:/", "cpak4:/"
};


/**
 * Check whether the specified controller has a controller pak accessory.
 *
 * @param controller Index of the controller (typically 0–3).
 * @returns `true` if the controller's accessory type is a controller pak, `false` otherwise.
 */
bool has_cpak(int controller) {
    
    joypad_accessory_type_t val =  joypad_get_accessory_type(controller);

    return val == JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK;
}

/**
 * Compute how many MEMPAK blocks a file occupies.
 *
 * @param filename_cpak Path to the file on the filesystem.
 * @returns Number of MEMPAK blocks occupied by the file (file size divided by MEMPAK_BLOCK_SIZE).
 */
int get_block_size_from_fs_path(const char *filename_cpak) {
    return get_file_size_from_fs_path(filename_cpak) / (MEMPAK_BLOCK_SIZE);
}

/**
 * Get the size in bytes of the file at the given filesystem path.
 *
 * @param filename_cpak Path to the file to measure.
 * @returns The file size in bytes as an `int`, or `-1` if the file cannot be opened or its size cannot be determined.
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
 * Unmounts CPAK filesystems on all four controller ports.
 *
 * Attempts to unmount the CPAK filesystem for controller ports 0 through 3.
 * Individual unmount failures are not propagated to the caller.
 */
void unmount_all_cpakfs() {
    for (int i = 0; i < 4; i++) {
        int val = cpakfs_unmount(i);
        if (val < 0) {
            //debugf("Failed to unmount cpakfs on port %d: %d\n", i+1, val);
        } else {
            //debugf("Unmounted cpakfs on port %d\n", i+1);
        }
    }
}

/**
 * Advance the note index by one, wrapping to 0 after the maximum.
 * @param current_index Current index in the range [0, MAX_NUM_NOTES - 1].
 * @returns The next index in the range [0, MAX_NUM_NOTES - 1].
 */
int inc_index_note(int current_index) {
    return ((current_index + 1) + MAX_NUM_NOTES) % MAX_NUM_NOTES; 
}

/**
 * Return the previous note index, wrapping to MAX_NUM_NOTES-1 when current_index is 0.
 *
 * @param current_index Current note index (expected in range 0..MAX_NUM_NOTES-1).
 * @returns Previous index in range 0..MAX_NUM_NOTES-1 (`(current_index - 1) mod MAX_NUM_NOTES`).
 */
int dec_index_note(int current_index) {
    return ((current_index - 1) + MAX_NUM_NOTES) % MAX_NUM_NOTES; 
}

// -1 = error
/**
 * Mounts a CPAK filesystem on the specified controller port.
 *
 * @param controller Index of the controller port (0-3) to mount.
 * @returns 0 on success, -1 if mounting failed.
 */
int mount_cpakfs(int controller) {
    if (cpakfs_mount(controller, CPAK_MOUNT_ARRAY[controller]) < 0) {
        //debugf("Failed to mount cpakfs on port %d\n", controller + 1);
       return -1;

    } else {
        //debugf("Mounted cpakfs on port %d\n", controller + 1);
        return 0;
    }
}

/**
 * Extracts a title from an absolute file path and writes it into the provided buffer.
 *
 * The title is derived from the filename portion (text after the last '/'):
 * - If the filename contains an underscore ('_'), the title is the substring before the first underscore.
 * - Otherwise, if the filename contains a dot ('.'), the title is the substring before the last dot.
 * - If neither is present, the entire filename is used.
 *
 * The result is copied into `outbuf` and always null-terminated; if the title is longer than
 * `outbuf_size - 1`, it is truncated to fit.
 *
 * @param path Absolute or relative path containing the filename.
 * @param outbuf Destination buffer to receive the extracted title (null-terminated).
 * @param outbuf_size Size of `outbuf` in bytes.
 * @returns Always returns 0.
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
 * Parse a CPAK-style full filename into its components: game code, publisher code, filename, and extension.
 *
 * The input must follow the form "GGGG.PP-NAME[.EXT]" where:
 * - GGGG is a 4-character game code.
 * - PP is a 2- or 4-character publisher code.
 * - NAME is the filename (up to 16 characters stored).
 * - EXT is an optional extension (up to 4 characters stored).
 *
 * Parsed fields are written into the provided `out` structure. If a component exceeds the destination
 * field size the value is truncated and always null-terminated.
 *
 * @param fullname Null-terminated string containing the full name to parse.
 * @param out Pointer to a cpakfs_path_strings_t structure that will receive the parsed fields.
 * @returns 0 on success.
 * @returns -1 if the required first dot separator is missing.
 * @returns -2 if the required dash separator is missing after the first dot.
 * @returns -3 if the game code is not exactly 4 characters long.
 * @returns -4 if the publisher code is not 2 or 4 characters long.
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
 * Parse a CPAK-style fullname into its components: gamecode, pubcode, filename, and extension.
 *
 * Expects the input format "GGGG.PP-FILENAME[.EXT]" where:
 * - `GGGG` is a 4-character game code before the first dot,
 * - `PP` is a 2-character publisher code immediately after that dot and before the dash,
 * - `FILENAME` is the segment after the dash (truncated to 16 characters if longer),
 * - `.EXT` is an optional extension after a second dot (up to 4 characters).
 *
 * @param s Input fullname string to parse.
 * @param out Pointer to a cpakfs_path_strings_t that will be populated with null-terminated
 *            `gamecode`, `pubcode`, `filename`, and `ext` fields on success.
 * @returns 0 on successful parse; -1 if the gamecode portion is missing or not 4 characters
 *          before the first dot; -2 if the publisher code is missing or not 2 characters
 *          between the first dot and the dash.
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
 * Format structured CPAK filename components into a single full name string.
 *
 * @param in Pointer to a cpakfs_path_strings_t containing `gamecode` (4 chars), `pubcode` (2 chars), optional `filename` (up to 16 chars) and optional `ext` (up to 4 chars).
 * @param out Destination buffer to receive the formatted name.
 * @param outsz Size of the destination buffer in bytes.
 * @returns 0 on success.
 * @returns -1 if `in->gamecode` is not 4 characters or `in->pubcode` is not 2 characters.
 * @returns -2 if `outsz` is too small to hold the formatted name including the terminating NUL.
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

/**
 * Validate that a string contains only base-36 characters and is not empty.
 *
 * @param s Input C string to validate.
 * @returns `1` if `s` is non-empty and every character is in the set '0'–'9' or 'A'–'Z', `0` otherwise.
 */
static int is_base36_str(const char *s){ if(!*s) return 0; for(const unsigned char*p=(const unsigned char*)s;*p;++p) if(!((*p>='0'&&*p<='9')||(*p>='A'&&*p<='Z'))) return 0; return 1; }
/**
 * Convert a base-36 string (digits '0'–'9' and uppercase 'A'–'Z') to an unsigned integer.
 * @param s NUL-terminated base-36 string to convert. The function expects characters '0'–'9' and 'A'–'Z'.
 *          Behavior for other characters is determined by the same numeric mapping but is not validated.
 * @returns The numeric value represented by `s` interpreted in base 36.
 */
static unsigned base36_to_uint(const char *s){ unsigned v=0; for(const unsigned char*p=(const unsigned char*)s;*p;++p){ v*=36; v+= (*p<='9'?*p-'0':*p-'A'+10); } return v; }
/**
 * Convert an unsigned integer to a base-36 string using digits `0-9` and `A-Z`.
 *
 * The converted representation is written into `out` as an ASCII string with
 * the most-significant digit first and always contains at least one digit
 * (representing zero when `n` is 0). Up to `maxlen` characters are written;
 * if the full representation would exceed `maxlen` characters it is truncated
 * to the most-significant `maxlen` digits. The result is null-terminated.
 *
 * @param n The unsigned integer to convert.
 * @param out Buffer to receive the resulting null-terminated string.
 * @param maxlen Maximum number of characters to write into `out` (not counting the terminating NUL).
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
 * Build a file path by concatenating a mount prefix and a name, inserting a '/'
 * between them if the mount does not already end with '/' or ':'.
 * @param mount Mount prefix (may be empty or end with '/' or ':').
 * @param name Path or filename to append to the mount prefix.
 * @param out Buffer to receive the resulting null-terminated path.
 * @param outsz Size of the output buffer in bytes.
 * @returns 0 on success, -1 if the output buffer is too small.
 */
static int join_mount_name(const char *mount, const char *name, char *out, size_t outsz){
    size_t m = strlen(mount), n = strlen(name);
    int need_sep = (m>0 && mount[m-1] != '/' && mount[m-1] != ':');
    size_t need = m + (need_sep?1:0) + n + 1;
    if (need > outsz) return -1;
    char *p = out;
    memcpy(p, mount, m); p += m;
    if (need_sep){ *p++ = '/'; }
    memcpy(p, name, n); p += n;
    *p = '\0';
    return 0;
}

/**
 * Find or generate a unique mounted CPAK filename for a desired name under a mount prefix.
 *
 * Attempts to use desired_name as-is (joined with mount_prefix) if the resulting full path does not exist.
 * If it exists, iteratively generates candidate names by appending or incrementing a base-36 extension until an unused full path is found, then writes that full mounted path into out_fullpath.
 *
 * @param mount_prefix Mount prefix (e.g. "cpak1:/") to prepend to generated names.
 * @param desired_name Desired CPAK filename in the expected formatted form (parsed by parse_fullname).
 * @param out_fullpath Buffer that will receive the full mounted path (mount_prefix + filename) on success.
 * @param outsz Size of out_fullpath buffer in bytes.
 * @param exists_fullpath Callback that returns non-zero if the given full mounted path exists; it will be called with a FULL mounted path (e.g. "cpak1:/NAME").
 *
 * @returns 0 on success with out_fullpath populated.
 *          -1 if desired_name failed parsing.
 *          -2 if joining mount_prefix and desired_name overflowed the internal buffer.
 *          -3 if out_fullpath is too small to hold the initially available full path.
 *          -4 if formatting the base filename failed.
 *          -5 if out_fullpath is too small to hold a chosen candidate full path.
 *          -6 if no unique name could be found after exhausting the candidate space.
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
 * Check whether a file exists at the given mounted full path.
 *
 * @param full_mounted_path Path to the file including mount prefix.
 * @returns `1` if a file at `full_mounted_path` can be opened for reading (exists), `0` otherwise.
 */
int file_exists_full(const char *full_mounted_path){
    FILE *f = fopen(full_mounted_path, "rb");
    if (f){ fclose(f); return 1; }
    return 0;
}