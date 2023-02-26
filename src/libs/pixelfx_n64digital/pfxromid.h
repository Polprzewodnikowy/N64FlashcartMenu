/*
** Copyright 2021 JonesAlmighty & PixelFx
*/

#ifndef __PIXELFX_ROMID_H
#define __PIXELFX_ROMID_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

void pfx_send_game_id(uint8_t* crc_hi, uint8_t* crc_lo, uint8_t media_format, uint8_t country_id);

#ifdef __cplusplus
}
#endif

#endif
