/**
 * @file ed64_xseries_ll.h
 * @brief ed64 x series flashcart low level access
 * @ingroup flashcart
 */

#ifndef FLASHCART_ED64_XSERIES_LL_H__
#define FLASHCART_ED64_XSERIES_LL_H__

#include <stdbool.h>

/**
 * @addtogroup ed64_xseries_ll
 * @{
 */

/**
 * @brief Load and apply the RLE-compressed ICE40 and main FPGA images, switching the cart
 *        from its minimal cold-boot personality to the full one (RTC/USB/save emulation
 *        registers, shared with V series - see ed64_bios_ll.h). This is volatile (lost on
 *        power-off) and must be redone on every boot, mirroring the official OS's
 *        edConfigureFpga(); unlike that OS this is unconditionally redone on every boot
 *        rather than only on a true cold power-on.
 * 
 *        The ICE40 is loaded first and the main FPGA only if that succeeds, mirroring the
 *        official OS exactly: the ICE40 hosts the cart's foundational register block
 *        (EDID/cart-id, boot-config, SD card SPI) that the main FPGA's own config port
 *        depends on, so a failed ICE40 load means the main FPGA can't be loaded either -
 *        there's no main-FPGA-only fallback.
 * 
 * @param ice_image_path Path (e.g. "rom:/...") to the RLE-compressed ICE40 image.
 * @param main_image_path Path (e.g. "rom:/...") to the RLE-compressed main FPGA image.
 * @return true if the cart responded correctly after reconfiguration, false otherwise.
 */
bool ed64_xseries_ll_configure_fpga (const char *ice_image_path, const char *main_image_path);

/** @} */ /* ed64_xseries_ll */


#endif
