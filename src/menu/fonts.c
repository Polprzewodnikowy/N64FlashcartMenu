#include <libdragon.h>

#include "assets.h"
#include "fonts.h"


static void load_default_font (void) {
    rdpq_font_t *default_font = rdpq_font_load_buf(assets_FiraMonoBold, (int) (assets_FiraMonoBold_size));

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
