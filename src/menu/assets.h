/**
 * @file assets.h
 * @brief Menu Assets
 * @ingroup menu 
 */

#ifndef ASSETS_H__
#define ASSETS_H__


#include <rdpq_font.h>


/** @brief Assets Structure */
typedef struct {
    /** @brief RDPQ Font */
    rdpq_font_t *font;
    /** @brief Font Height */
    int font_height;
} assets_t;


void assets_init (void);
assets_t *assets_get (void);


#endif
