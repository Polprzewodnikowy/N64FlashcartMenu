/**
 * @file cic.h
 * @brief Header file for CIC (Copy Protection Chip) related functions and definitions.
 * @ingroup boot
 */

#ifndef CIC_H__
#define CIC_H__

#include <stdint.h>

#define IPL3_LENGTH     (4032)

/**
 * @enum cic_type_t
 * @brief Enumeration of different CIC types.
 */
typedef enum {
    CIC_5101,   /**< CIC type 5101 */
    CIC_5167,   /**< CIC type 5167 */
    CIC_6101,   /**< CIC type 6101 */
    CIC_7102,   /**< CIC type 7102 */
    CIC_x102,   /**< CIC type x102 */
    CIC_x103,   /**< CIC type x103 */
    CIC_x105,   /**< CIC type x105 */
    CIC_x106,   /**< CIC type x106 */
    CIC_8301,   /**< CIC type 8301 */
    CIC_8302,   /**< CIC type 8302 */
    CIC_8303,   /**< CIC type 8303 */
    CIC_8401,   /**< CIC type 8401 */
    CIC_8501,   /**< CIC type 8501 */
    CIC_UNKNOWN /**< Unknown CIC type */
} cic_type_t;

/**
 * @brief Detects the CIC type based on the provided IPL3 data.
 *
 * This function analyzes the provided IPL3 data to determine the CIC type.
 *
 * @param ipl3 A pointer to the IPL3 data.
 * @return The detected CIC type.
 */
cic_type_t cic_detect(uint8_t *ipl3);

/**
 * @brief Gets the seed value for the specified CIC type.
 *
 * This function returns the seed value associated with the given CIC type.
 *
 * @param cic_type The type of CIC.
 * @return The seed value for the specified CIC type.
 */
uint8_t cic_get_seed(cic_type_t cic_type);

#endif // CIC_H__
