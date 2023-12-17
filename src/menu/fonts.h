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
    STL_DIRECTORY,
    STL_SAVE,
    STL_MEDIA,
    STL_UNKNOWN,
} menu_font_style_t;


void fonts_init (void);


#endif
