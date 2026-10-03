/**
 * @file ed64_vseries_ll.h
 * @brief ED64 V series flashcart low level access
 * @ingroup flashcart
 */

#ifndef FLASHCART_ED64_VSERIES_LL_H__
#define FLASHCART_ED64_VSERIES_LL_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @addtogroup ed64_vseries_ll
 * @{
 */

/**
 * @brief Load and apply an RLE-compressed FPGA/CPLD image, switching the cart from its
 *        minimal cold-boot personality to the full one (RTC/USB/save emulation registers,
 *        shared with X series - see ed64_bios_ll.h). This is volatile (lost on power-off)
 *        and must be redone on every boot, mirroring what the official EverDrive-64 OS
 *        does during its own startup.
 * 
 * @param image_path Path (e.g. "rom:/...") to the RLE-compressed image to load.
 * @return true if the cart responded correctly after reconfiguration, false otherwise.
 */
bool ed64_vseries_ll_configure_fpga (const char *image_path);

/** @} */ /* ed64_vseries_ll */


#endif
