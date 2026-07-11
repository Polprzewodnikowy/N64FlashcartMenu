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

/**
 * @brief Compare two strings for exact equality using a bounded scan.
 *
 * @param lhs First string.
 * @param rhs Second string.
 * @param max_len Maximum number of characters to scan for each input.
 * @return true if both strings are non-NULL, null-terminated within
 *         @p max_len, and exactly equal; false otherwise.
 */
bool bounded_streq(const char *lhs, const char *rhs, size_t max_len);

/**
 * @brief Compare two strings for case-insensitive equality using a bounded scan.
 *
 * @param lhs First string.
 * @param rhs Second string.
 * @param max_len Maximum number of characters to scan for each input.
 * @return true if both strings are non-NULL, null-terminated within
 *         @p max_len, and equal ignoring case; false otherwise.
 */
bool bounded_strcaseeq(const char *lhs, const char *rhs, size_t max_len);

#endif
