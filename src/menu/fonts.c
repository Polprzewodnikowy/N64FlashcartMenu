#include <libdragon.h>

#include "fonts.h"


#define FONT_IMPORT(f) \
    extern void *_binary_assets_##f##_start __attribute__((section(".data"))); \
    extern int _binary_assets_##f##_size __attribute__((section(".data")));
#define FONT_LOAD(f) rdpq_font_load_buf((void *) (&_binary_assets_##f##_start), (size_t) (&_binary_assets_##f##_size))


FONT_IMPORT(FiraMono_Bold_font64);


static void load_default_font (void) {
    rdpq_font_t *default_font = FONT_LOAD(FiraMono_Bold_font64);

    rdpq_font_style(default_font, STL_DEFAULT, &((rdpq_fontstyle_t) { .color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF) }));
    rdpq_font_style(default_font, STL_DIRECTORY, &((rdpq_fontstyle_t) { .color = RGBA32(0xFF, 0xFF, 0x70, 0xFF) }));
    rdpq_font_style(default_font, STL_SAVE, &((rdpq_fontstyle_t) { .color = RGBA32(0x70, 0xFF, 0x70, 0xFF) }));
    rdpq_font_style(default_font, STL_MEDIA, &((rdpq_fontstyle_t) { .color = RGBA32(0x70, 0xBC, 0xFF, 0xFF) }));
    rdpq_font_style(default_font, STL_UNKNOWN, &((rdpq_fontstyle_t) { .color = RGBA32(0xA0, 0xA0, 0xA0, 0xFF) }));

    rdpq_text_register_font(FNT_DEFAULT, default_font);
}


void fonts_init (void) {
    load_default_font();
}
