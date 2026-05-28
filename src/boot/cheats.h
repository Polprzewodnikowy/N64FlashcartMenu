/**
 * @file cheats.h
 * @brief Header file for cheat installation functions.
 * @ingroup boot
 */

#ifndef CHEATS_H__
#define CHEATS_H__

#include <stdint.h>
#include "cic.h"

/**
 * @brief Installs cheats and/or IGR payload based on the CIC type.
 *
 * This function installs the cheats provided in the cheat list and/or the
 * In-Game Reset (IGR) payload based on the specified CIC type.
 *
 * @param cic_type The type of CIC (Copy Protection Chip) used.
 * @param cheat_list A pointer to an array of cheats to be installed (may be NULL).
 * @param igr_enabled Whether to install the IGR payload.
 * @return true if the cheats and/or IGR were successfully installed, false otherwise.
 */
bool cheats_install(cic_type_t cic_type, uint32_t *cheat_list, bool igr_enabled);

#endif // CHEATS_H__
