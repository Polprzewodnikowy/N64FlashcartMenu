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

/**
 * @brief Poll the USB communication subsystem.
 *
 * This function polls the USB communication subsystem to handle any
 * incoming or outgoing data. It is only available in debug builds.
 *
 * @param menu Pointer to the menu structure.
 */
void usb_comm_poll(menu_t *menu);

#endif /* USB_COMM_H__ */
