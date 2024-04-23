/**
 * @file fonts.h
 * @brief Menu fonts
 * @ingroup menu 
 */

#ifndef FONTS_H__
#define FONTS_H__

/** @brief Font type enumeration. */
typedef enum {
    FNT_DEFAULT = 1,
} menu_font_type_t;

/** @brief Font style enumeration. */
typedef enum {
    STL_DEFAULT = 0,
    STL_GREEN,
    STL_BLUE,
    STL_YELLOW,
    STL_ORANGE,
    STL_GRAY,
} menu_font_style_t;


void fonts_init (char *custom_font_path);


#endif
