/**
 * @file bounded_string.c
 * @brief Bounded string comparison helper implementations.
 */

#include "bounded_string.h"

#include <string.h>
#include <strings.h>

typedef int (*strcmp_fn_t)(const char *, const char *, size_t);

/**
 * @brief Shared null, termination, and length guard for equality checks.
 *
 * Returns false if either input is NULL, exceeds @p max_len without a null
 * terminator, or the two lengths differ. Otherwise delegates the byte
 * comparison to @p cmp_fn.
 */
static bool bounded_eq(const char *lhs, const char *rhs, size_t max_len, strcmp_fn_t cmp_fn) {
    if (!lhs || !rhs) return false;

    size_t lhs_len = strnlen(lhs, max_len);
    size_t rhs_len = strnlen(rhs, max_len);
    if ((lhs_len == max_len) || (rhs_len == max_len) || (lhs_len != rhs_len)) {
        return false;
    }

    return cmp_fn(lhs, rhs, lhs_len) == 0;
}

bool bounded_streq(const char *lhs, const char *rhs, size_t max_len) {
    return bounded_eq(lhs, rhs, max_len, strncmp);
}

bool bounded_strcaseeq(const char *lhs, const char *rhs, size_t max_len) {
    return bounded_eq(lhs, rhs, max_len, strncasecmp);
}

int bounded_strcasecmp_cmp(const char *lhs, const char *rhs, size_t max_len) {
    if (!lhs && !rhs) return 0;
    if (!lhs) return -1;
    if (!rhs) return 1;

    size_t lhs_len = strnlen(lhs, max_len);
    size_t rhs_len = strnlen(rhs, max_len);
    size_t cmp_len = lhs_len < rhs_len ? lhs_len : rhs_len;

    int cmp = strncasecmp(lhs, rhs, cmp_len);
    if (cmp != 0) {
        return cmp;
    }

    if (lhs_len == rhs_len) {
        return 0;
    }

    return lhs_len < rhs_len ? -1 : 1;
}
