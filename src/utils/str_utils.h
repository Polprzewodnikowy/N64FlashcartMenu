#ifndef STR_UTILS_H__
#define STR_UTILS_H__


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

// e.g. if (str_endswith(cur_rom, ".z64", false) || str_endswith(cur_rom, ".n64", false))
static bool str_endswith(const char *str, const char *suffix, bool case_sensitive) {
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    if (str_len < suffix_len)
        return false;

    if (!case_sensitive) {
        for (size_t i = 0; i < suffix_len; i++) {
            if (tolower(str[str_len - suffix_len + i]) != tolower(suffix[i]))
                return false;
        }
    } else {
        for (size_t i = 0; i < suffix_len; i++) {
            if (str[str_len - suffix_len + i] != suffix[i])
                return false;
        }
    }

    return true;
}

#endif