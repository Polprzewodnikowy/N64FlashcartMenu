/**
 * @file igr.h
 * @brief In-Game Reset (IGR) payload generator
 * @ingroup boot
 */

#ifndef IGR_H__
#define IGR_H__

#include <stdint.h>

#include "boot_io.h"

/**
 * @brief Append the IGR payload to a cheat engine buffer.
 *
 * Generates MIPS instructions that check for a VI interrupt,
 * polls the controller for the L+R+Z+A+B+Start combo, and
 * writes the SC64 AUX reboot value if matched.
 *
 * @param engine_p Pointer to the engine buffer pointer (updated in-place).
 */
void igr_append_payload (io32_t **engine_p);

#endif /* IGR_H__ */
