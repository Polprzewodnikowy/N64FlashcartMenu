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


bool has_cpak(int controller) {
    
    joypad_accessory_type_t val =  joypad_get_accessory_type(controller);

    return val == JOYPAD_ACCESSORY_TYPE_CONTROLLER_PAK;
}

int get_block_size_from_fs_path(const char *filename_cpak) {
    return get_file_size_from_fs_path(filename_cpak) / (MEMPAK_BLOCK_SIZE);
}

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

int inc_index_note(int current_index) {
    return ((current_index + 1) + MAX_NUM_NOTES) % MAX_NUM_NOTES; 
}

int dec_index_note(int current_index) {
    return ((current_index - 1) + MAX_NUM_NOTES) % MAX_NUM_NOTES; 
}

// -1 = error
//  0 = success
int mount_cpakfs(int controller) {
    if (cpakfs_mount(controller, CPAK_MOUNT_ARRAY[controller]) < 0) {
        //debugf("Failed to mount cpakfs on port %d\n", controller + 1);
       return -1;

    } else {
        //debugf("Mounted cpakfs on port %d\n", controller + 1);
        return 0;
    }
}

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
static int is_base36_str(const char *s){ if(!*s) return 0; for(const unsigned char*p=(const unsigned char*)s;*p;++p) if(!((*p>='0'&&*p<='9')||(*p>='A'&&*p<='Z'))) return 0; return 1; }
static unsigned base36_to_uint(const char *s){ unsigned v=0; for(const unsigned char*p=(const unsigned char*)s;*p;++p){ v*=36; v+= (*p<='9'?*p-'0':*p-'A'+10); } return v; }
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

// --- safe join: prefix + name -> fullpath ---
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

// exists_fullpath() must accept a FULL mounted path (e.g. "cpak1:/NAME")
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

int file_exists_full(const char *full_mounted_path){
    FILE *f = fopen(full_mounted_path, "rb");
    if (f){ fclose(f); return 1; }
    return 0;
}
