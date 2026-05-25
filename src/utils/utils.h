#ifndef UTILS_H__
#define UTILS_H__

/**
 * @file utils.h
 * @brief Utility macros for alignment, min/max, and size conversions.
 * @ingroup utils
 */

/**
 * @def ALIGN(x, a)
 * @brief Align a value to the specified alignment boundary.
 *
 * This macro aligns the given value @p x to the specified alignment @p a.
 *
 * @param x The value to align.
 * @param a The alignment boundary (must be a power of two).
 * @return The aligned value.
 */
#define ALIGN(x, a)     (((x) + ((typeof(x))(a) - 1)) & ~((typeof(x))(a) - 1))

/**
 * @def MAX(a, b)
 * @brief Return the maximum of two values.
 *
 * This macro returns the maximum of the two provided values @p a and @p b.
 *
 * @param a The first value.
 * @param b The second value.
 * @return The maximum of @p a and @p b.
 */
#define MAX(a,b)        ({ typeof(a) _a = a; typeof(b) _b = b; _a > _b ? _a : _b; })

/**
 * @def MIN(a, b)
 * @brief Return the minimum of two values.
 *
 * This macro returns the minimum of the two provided values @p a and @p b.
 *
 * @param a The first value.
 * @param b The second value.
 * @return The minimum of @p a and @p b.
 */
#define MIN(a,b)        ({ typeof(a) _a = a; typeof(b) _b = b; _a < _b ? _a : _b; })

/**
 * @def KiB(x)
 * @brief Convert a value to kibibytes.
 *
 * This macro converts the given value @p x to kibibytes (multiplying by 1024).
 *
 * @param x The value to convert.
 * @return The value in kibibytes.
 */
#define KiB(x)          ((x) * 1024)

/**
 * @def MiB(x)
 * @brief Convert a value to mebibytes.
 *
 * This macro converts the given value @p x to mebibytes (multiplying by 1024*1024).
 *
 * @param x The value to convert.
 * @return The value in mebibytes.
 */
#define MiB(x)          ((x) * 1024 * 1024)

#endif // UTILS_H__
