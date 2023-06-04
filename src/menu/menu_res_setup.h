#ifndef MENU_RES_SETUP_H__
#define MENU_RES_SETUP_H__

#include <stdint.h>

static const int16_t font_vertical_pixels = 8;
static const int16_t overscan_vertical_pixels = 24;
static const int16_t vertical_start_position = overscan_vertical_pixels / 2;

static const int16_t font_horizontal_pixels = 8;
static const int16_t overscan_horizontal_pixels = 32;
static const int16_t horizontal_start_position = overscan_horizontal_pixels + font_horizontal_pixels;
static const int16_t horizontal_indent = horizontal_start_position + (font_horizontal_pixels * 2);

#endif
