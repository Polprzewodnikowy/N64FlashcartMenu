#ifndef HDMI_H__
#define HDMI_H__


#include "boot/boot.h"


void hdmi_reset_game_id (void);
void hdmi_broadcast_game_id (boot_params_t *boot_params);


#endif
