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
 * Check whether the specified controller has a Controller Pak accessory attached.
 *
 * @param controller Index of the controller/joypad (platform-specific, typically 0-based).
 * @return true if the controller's accessory type is JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK; false otherwise.
 */
bool has_cpak(int controller) {
    
    joypad_accessory_type_t val =  joypad_get_accessory_type(controller);

    return val == JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK;
}

/**
 * Return the size of the file at the given path measured in MEMPAK blocks.
 *
 * The result is computed as get_file_size_from_fs_path(filename_cpak) divided
 * by MEMPAK_BLOCK_SIZE.
 *
 * @param filename_cpak Filesystem path to the .cpak file.
 * @return Number of MEMPAK blocks (file size / MEMPAK_BLOCK_SIZE). */
int get_block_size_from_fs_path(const char *filename_cpak) {
    return get_file_size_from_fs_path(filename_cpak) / (MEMPAK_BLOCK_SIZE);
}

/**
 * Get the size of a file on the filesystem.
 *
 * Attempts to open the file at `filename_cpak` in binary mode and returns its
 * size in bytes. If the file cannot be opened, returns -1.
 *
 * @param filename_cpak Path to the file whose size is requested.
 * @return File size in bytes on success, or -1 if the file cannot be opened.
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
 * Unmount CPAK filesystems on all four controller ports.
 *
 * Iterates ports 0..3 and calls cpakfs_unmount for each. Failures are ignored
 * (no errors are propagated); successful and failed attempts are currently
 * only observable via logging (which is commented out).
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
 * Advance a note index by one within the circular range of 0..MAX_NUM_NOTES-1.
 *
 * Computes and returns (current_index + 1) modulo MAX_NUM_NOTES. Accepts values
 * outside the 0..MAX_NUM_NOTES-1 range (including negative) and normalizes them via modulo.
 * @param current_index Current note index.
 * @return Next note index in the range [0, MAX_NUM_NOTES-1].
 */
int inc_index_note(int current_index) {
    return ((current_index + 1) + MAX_NUM_NOTES) % MAX_NUM_NOTES; 
}

/**
 * Decrement an index in a circular range [0, MAX_NUM_NOTES-1].
 *
 * Wraps underflow to the maximum index so callers can move one step backward
 * in a cyclic note list.
 *
 * @param current_index Current index (expected 0..MAX_NUM_NOTES-1).
 * @return The previous index in the same range, wrapped modulo MAX_NUM_NOTES.
 */
int dec_index_note(int current_index) {
    return ((current_index - 1) + MAX_NUM_NOTES) % MAX_NUM_NOTES; 
}

// -1 = error
/**
 * Mount a CPAK filesystem on the specified controller port.
 *
 * Attempts to mount the CPAK filesystem using the controller index and
 * the corresponding mount prefix from CPAK_MOUNT_ARRAY.
 *
 * @param controller Controller index (0..3) specifying which CPAK port to mount.
 * @return 0 on success, -1 on failure.
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
 * Extract title from an absolute or relative filesystem path.
 *
 * Locates the filename (text after the last '/') and returns the title portion:
 * - if the filename contains an underscore ('_'), the title is the substring before the first underscore;
 * - otherwise, if the filename contains a dot ('.'), the title is the substring before the last dot;
 * - otherwise the whole filename is used.
 *
 * The result is copied into outbuf, truncated if necessary to fit, and always null-terminated.
 *
 * @param path Input path string (must be non-NULL).
 * @param outbuf Destination buffer to receive the extracted title (must be non-NULL).
 * @param outbuf_size Size of outbuf; must be at least 1.
 * @return 0 on success.
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
 * Parse a CPak-style fullname into its components.
 *
 * Parses strings of the form "GAME.PUB-NAME[.EXT]" into the provided
 * cpakfs_path_strings_t structure. On success the structure fields
 * (gamecode, pubcode, filename, ext) are populated and always NUL-terminated.
 *
 * Rules and limits:
 * - `GAME` must be exactly 4 characters.
 * - `PUB` must be either 2 or 4 characters.
 * - `NAME` is copied up to 16 characters; if longer it is truncated.
 * - `EXT` (optional) is copied up to 4 characters; if longer it is truncated.
 *
 * @param fullname Input full name to parse (expected ASCII, no validation of characters).
 * @param out Destination structure to receive parsed components (must be non-NULL).
 * @return 0 on success.
 * @return -1 if no first '.' is found after the game code.
 * @return -2 if no '-' (dash) separator is found after publisher code.
 * @return -3 if the game code before the first '.' is not exactly 4 characters.
 * @return -4 if the publisher code length is not 2 or 4 characters.
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
 * Parse a CPak fullname string into its components (gamecode, pubcode, filename, ext).
 *
 * Expects strings of the form "GGGG.PP-NAME[.EXT]" where:
 * - GGGG is a 4-character game code,
 * - PP is a 2-character publisher code,
 * - NAME is up to 16 characters,
 * - EXT is an optional extension up to 4 characters.
 *
 * On success fills the provided cpakfs_path_strings_t fields (null-terminated)
 * and returns 0.
 *
 * @param s Input fullname to parse.
 * @param out Destination structure to receive parsed components; its string
 *            buffers are written and null-terminated by this function.
 * @returns 0 on success,
 *          -1 if the initial gamecode/dot format is invalid (gamecode not 4 chars before the first '.'),
 *          -2 if the publisher code/dash format is invalid (publisher not 2 chars between '.' and '-').
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
 * Format a CPak full filename string from its parsed components.
 *
 * Builds "GAME.PB-NAME" with an optional ".EXT" appended, copying the result
 * into the provided output buffer and NUL-terminating it.
 *
 * Requirements and behavior:
 * - `in->gamecode` must be exactly 4 characters and `in->pubcode` exactly 2
 *   characters; otherwise the function returns -1.
 * - `in->filename` is truncated to at most 16 characters and `in->ext` to at
 *   most 4 characters when computing required output size.
 * - If the output buffer is too small to hold the formatted string plus a
 *   terminating NUL, the function returns -2 and writes nothing.
 *
 * @param in     Parsed components (gamecode, pubcode, filename, ext).
 * @param out    Destination buffer to receive the formatted fullname.
 * @param outsz  Size of the destination buffer in bytes.
 * @return 0 on success; -1 if input components have invalid lengths;
 *         -2 if outsz is insufficient.
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
 * Check whether a string is a non-empty base-36 identifier (digits 0-9 and uppercase A-Z only).
 *
 * Returns 1 if the provided NUL-terminated string is non-empty and every character is
 * in the ranges '0'–'9' or 'A'–'Z'. Returns 0 for empty strings or if any character
 * falls outside those ranges (lowercase letters are considered invalid).
 */
static int is_base36_str(const char *s){ if(!*s) return 0; for(const unsigned char*p=(const unsigned char*)s;*p;++p) if(!((*p>='0'&&*p<='9')||(*p>='A'&&*p<='Z'))) return 0; return 1; }
/**
 * Convert an ASCII base-36 string to an unsigned integer.
 *
 * Interprets the NUL-terminated string `s` as an unsigned base-36 number composed
 * of digits '0'–'9' and uppercase letters 'A'–'Z' (A=10 ... Z=35). Processing
 * stops at the first NUL byte; an empty string yields 0.
 *
 * The function does not validate input characters or check for overflow: supplying
 * characters outside the expected ranges or a value that exceeds unsigned capacity
 * results in implementation-defined behavior.
 *
 * @param s NUL-terminated base-36 string (expected ASCII digits '0'–'9' and uppercase 'A'–'Z').
 * @return The numeric value of the parsed base-36 string.
 */
static unsigned base36_to_uint(const char *s){ unsigned v=0; for(const unsigned char*p=(const unsigned char*)s;*p;++p){ v*=36; v+= (*p<='9'?*p-'0':*p-'A'+10); } return v; }
/**
 * Convert an unsigned integer to an uppercase base-36 string.
 *
 * The result is written as a null-terminated string into `out`. Digits use
 * '0'..'9' then 'A'..'Z'. At least one digit is produced (zero -> "0").
 * If the full base-36 representation exceeds `maxlen`, the function truncates
 * by dropping the least-significant digits (keeps the most-significant
 * `maxlen` digits). `maxlen` specifies the maximum number of characters to
 * write (excluding the terminating NUL). If `maxlen` is 0, only the NUL is
 * written.
 *
 * @param n       Value to convert.
 * @param out     Destination buffer for the NUL-terminated base-36 string.
 * @param maxlen  Maximum number of characters to write into `out` (not
 *                counting the terminating NUL).
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
 * Join a mount prefix and filename into a null-terminated mounted path.
 *
 * Concatenates `mount` and `name` into `out`, inserting a '/' separator if
 * `mount` does not already end with '/' or ':'.
 *
 * @param mount Mount prefix (may be empty).
 * @param name  Filename or relative path to append.
 * @param out   Destination buffer for the resulting string.
 * @param outsz Size of the destination buffer in bytes.
 * @return 0 on success; -1 if `outsz` is too small to hold the result including
 *         the terminating NUL.
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
 * Pick a non-conflicting CPak fullname under a mount prefix.
 *
 * Attempts to produce a mounted full path for `desired_name` under `mount_prefix`.
 * If the exact desired name (joined with the mount) does not exist according to
 * `exists_fullpath`, that full path is returned. Otherwise the function appends
 * a base-36 extension counter (up to 4 digits) to the base name and probes
 * candidates until a free name is found.
 *
 * @param mount_prefix Mount prefix to prepend (e.g. "cpak1:/").
 * @param desired_name Desired CPak fullname in internal format (parsed by parse_fullname).
 * @param out_fullpath Destination buffer to receive the chosen full mounted path.
 * @param outsz Size of `out_fullpath` in bytes.
 * @param exists_fullpath Callback that returns non-zero if a given full mounted path exists.
 *
 * @return 0 on success (out_fullpath filled with a null-terminated mounted path).
 * Negative error codes on failure:
 *   -1 : `desired_name` failed to parse.
 *   -2 : internal buffer too small when joining mount and name.
 *   -3 : provided `out_fullpath` buffer too small to hold the immediate free path.
 *   -4 : failed to format base name (unexpected).
 *   -5 : provided `out_fullpath` buffer too small to hold a generated candidate path.
 *   -6 : exhausted all base-36 candidate extensions without finding a free name.
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
 * Attempts to open the path for reading ("rb"); a successful open means the
 * file exists and is considered accessible. Does not create the file. This is
 * a lightweight existence probe and relies on the C stdio open semantics.
 *
 * @param full_mounted_path Full path on the mounted filesystem to check.
 * @return 1 if the file exists and was opened successfully, 0 otherwise.
 */
int my_exists_full(const char *full_mounted_path){
    FILE *f = fopen(full_mounted_path, "rb");
    if (f){ fclose(f); return 1; }
    return 0;
}
