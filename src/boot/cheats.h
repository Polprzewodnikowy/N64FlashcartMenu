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
 * @brief Installs cheats based on the CIC type.
 *
 * This function installs the cheats provided in the cheat list based on the
 * specified CIC type.
 *
 * @param cic_type The type of CIC (Copy Protection Chip) used.
 * @param cheat_list A pointer to an array of cheats to be installed.
 * @return true if the cheats were successfully installed, false otherwise.
 */
bool cheats_install(cic_type_t cic_type, uint32_t *cheat_list);

#endif // CHEATS_H__
