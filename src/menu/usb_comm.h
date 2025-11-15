/**
 * @file usb_comm.h
 * @brief USB communication subsystem
 * @ingroup menu
 *
 * This file contains the declarations for the USB communication subsystem
 * used in the menu system.
 */

#ifndef USB_COMM_H__
#define USB_COMM_H__

#include "menu_state.h"

#ifndef NDEBUG
/**
 * @brief Poll the USB communication subsystem.
 *
 * This function polls the USB communication subsystem to handle any
 * incoming or outgoing data. It is only available in debug builds.
 *
 * @param menu Pointer to the menu structure.
 */
void usb_comm_poll(menu_t *menu);
#else
/**
 * @brief Poll the USB communication subsystem (no-op in release builds).
 *
 * This macro is a no-op in release builds, where USB communication polling
 * is disabled.
 *
 * @param menu Pointer to the menu structure.
 */
void usb_comm_poll(menu_t *menu);
#endif

#endif /* USB_COMM_H__ */
