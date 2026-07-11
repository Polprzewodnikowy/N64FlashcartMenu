/**
 * @file bounded_string.h
 * @brief Bounded string comparison helpers.
 *
 * These helpers compare C strings defensively by requiring both inputs to be
 * null-terminated within the provided maximum length. If either input is NULL,
 * not terminated within @p max_len, or has a different length, the comparison
 * returns false.
 */

#ifndef BOUNDED_STRING_H
#define BOUNDED_STRING_H

#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>

/**
 * @brief Compare two strings for exact equality using a bounded scan.
 *
 * @param lhs First string.
 * @param rhs Second string.
 * @param max_len Maximum number of characters to scan for each input.
 * @return true if both strings are non-NULL, null-terminated within
 *         @p max_len, and exactly equal; false otherwise.
 */
static inline bool bounded_streq(const char *lhs, const char *rhs, size_t max_len) {
    if (!lhs || !rhs) return false;

    size_t lhs_len = strnlen(lhs, max_len);
    size_t rhs_len = strnlen(rhs, max_len);
    if ((lhs_len == max_len) || (rhs_len == max_len) || (lhs_len != rhs_len)) {
        return false;
    }

    return strncmp(lhs, rhs, lhs_len) == 0;
}

/**
 * @brief Compare two strings for case-insensitive equality using a bounded scan.
 *
 * @param lhs First string.
 * @param rhs Second string.
 * @param max_len Maximum number of characters to scan for each input.
 * @return true if both strings are non-NULL, null-terminated within
 *         @p max_len, and equal ignoring case; false otherwise.
 */
static inline bool bounded_strcaseeq(const char *lhs, const char *rhs, size_t max_len) {
    if (!lhs || !rhs) return false;

    size_t lhs_len = strnlen(lhs, max_len);
    size_t rhs_len = strnlen(rhs, max_len);
    if ((lhs_len == max_len) || (rhs_len == max_len) || (lhs_len != rhs_len)) {
        return false;
    }

    return strncasecmp(lhs, rhs, lhs_len) == 0;
}

#endif
