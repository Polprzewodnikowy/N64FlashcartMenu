#ifndef ASSETS_H__
#define ASSETS_H__


#include <rdpq_font.h>


typedef struct {
    rdpq_font_t *font;
    int font_height;
} assets_t;


void assets_init (void);
assets_t *assets_get (void);


#endif
