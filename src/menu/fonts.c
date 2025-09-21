#include <libdragon.h>

#include "fonts.h"
#include "utils/fs.h"


static void load_default_font (char *custom_font_path) {
    char *font_path = "rom:/Firple-Bold.font64";

    if (custom_font_path && file_exists(custom_font_path)) {
        font_path = custom_font_path;
    }

    rdpq_font_t *default_font = rdpq_font_load(font_path);

    rdpq_font_style(default_font, STL_DEFAULT, &((rdpq_fontstyle_t) { .color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF) }));
    rdpq_font_style(default_font, STL_GREEN, &((rdpq_fontstyle_t) { .color = RGBA32(0x70, 0xFF, 0x70, 0xFF) }));
    rdpq_font_style(default_font, STL_BLUE, &((rdpq_fontstyle_t) { .color = RGBA32(0x70, 0xBC, 0xFF, 0xFF) }));
    rdpq_font_style(default_font, STL_YELLOW, &((rdpq_fontstyle_t) { .color = RGBA32(0xFF, 0xFF, 0x70, 0xFF) }));
    rdpq_font_style(default_font, STL_ORANGE, &((rdpq_fontstyle_t) { .color = RGBA32(0xFF, 0x99, 0x00, 0xFF) }));
    rdpq_font_style(default_font, STL_RED, &((rdpq_fontstyle_t) { .color = RGBA32(0xFF, 0x40, 0x40, 0xFF) }));
    rdpq_font_style(default_font, STL_GRAY, &((rdpq_fontstyle_t) { .color = RGBA32(0xA0, 0xA0, 0xA0, 0xFF) }));

    rdpq_text_register_font(FNT_DEFAULT, default_font);
}


void fonts_init (char *custom_font_path) {
    load_default_font(custom_font_path);
}
