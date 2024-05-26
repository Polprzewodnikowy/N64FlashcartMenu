/**
 * @file menu.h
 * @brief Menu Subsystem
 * @ingroup menu 
 */

#ifndef MENU_H__
#define MENU_H__

#include <libdragon.h>

#include "boot/boot.h"

extern wav64_t sfx_cursor, sfx_error, sfx_enter, sfx_exit, sfx_settings;

enum sfx_channels {
	SFX_CHANNEL
};

void menu_run (boot_params_t *boot_params);


#endif
