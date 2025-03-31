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
 * @brief Converts a value to kibibytes.
 *
 * This macro converts the given value `x` to kibibytes.
 *
 * @param x The value to convert.
 * @return The value in kibibytes.
 */
#define KiB(x)          ((x) * 1024)

/**
 * @brief Converts a value to mebibytes.
 *
 * This macro converts the given value `x` to mebibytes.
 *
 * @param x The value to convert.
 * @return The value in mebibytes.
 */
#define MiB(x)          ((x) * 1024 * 1024)

#endif // UTILS_H__
