#ifndef UTILS_H__
#define UTILS_H__

/**
 * @file utils.h
 * @brief Utility macros and functions.
 * @ingroup utils
 */

/**
 * @brief Aligns a value to the specified alignment.
 *
 * This macro aligns the given value `x` to the specified alignment `a`.
 *
 * @param x The value to align.
 * @param a The alignment boundary.
 * @return The aligned value.
 */
#define ALIGN(x, a)     (((x) + ((typeof(x))(a) - 1)) & ~((typeof(x))(a) - 1))

/**
 * @brief Returns the maximum of two values.
 *
 * This macro returns the maximum of the two provided values `a` and `b`.
 *
 * @param a The first value.
 * @param b The second value.
 * @return The maximum of `a` and `b`.
 */
#define MAX(a,b)        ({ typeof(a) _a = a; typeof(b) _b = b; _a > _b ? _a : _b; })

/**
 * @brief Returns the minimum of two values.
 *
 * This macro returns the minimum of the two provided values `a` and `b`.
 *
 * @param a The first value.
 * @param b The second value.
 * @return The minimum of `a` and `b`.
 */
#define MIN(a,b)        ({ typeof(a) _a = a; typeof(b) _b = b; _a < _b ? _a : _b; })

/**
 * @brief Converts a value to kilobytes.
 *
 * This macro converts the given value `x` to kilobytes.
 *
 * @param x The value to convert.
 * @return The value in kilobytes.
 */
#define KiB(x)          ((x) * 1024)

/**
 * @brief Converts a value to megabytes.
 *
 * This macro converts the given value `x` to megabytes.
 *
 * @param x The value to convert.
 * @return The value in megabytes.
 */
#define MiB(x)          ((x) * 1024 * 1024)

#endif // UTILS_H__
