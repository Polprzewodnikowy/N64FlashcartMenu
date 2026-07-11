/**
 * @file bounded_string.c
 * @brief Bounded string comparison helper implementations.
 */

#include "bounded_string.h"

#include <string.h>
#include <strings.h>

bool bounded_streq(const char *lhs, const char *rhs, size_t max_len) {
    if (!lhs || !rhs) return false;

    size_t lhs_len = strnlen(lhs, max_len);
    size_t rhs_len = strnlen(rhs, max_len);
    if ((lhs_len == max_len) || (rhs_len == max_len) || (lhs_len != rhs_len)) {
        return false;
    }

    return strncmp(lhs, rhs, lhs_len) == 0;
}

bool bounded_strcaseeq(const char *lhs, const char *rhs, size_t max_len) {
    if (!lhs || !rhs) return false;

    size_t lhs_len = strnlen(lhs, max_len);
    size_t rhs_len = strnlen(rhs, max_len);
    if ((lhs_len == max_len) || (rhs_len == max_len) || (lhs_len != rhs_len)) {
        return false;
    }

    return strncasecmp(lhs, rhs, lhs_len) == 0;
}
