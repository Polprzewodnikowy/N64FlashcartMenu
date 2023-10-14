/**
 * @file usb_comm.h
 * @brief USB communication subsystem
 * @ingroup menu
 */

#ifndef USB_COMM_H__
#define USB_COMM_H__


#include "menu_state.h"


#ifndef NDEBUG
void usb_comm_poll (menu_t *menu);
#else
#define usb_comm_poll(menu)
#endif


#endif
