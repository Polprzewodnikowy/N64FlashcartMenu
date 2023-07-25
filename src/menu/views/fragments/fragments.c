#include <stdarg.h>

#include <libdragon.h>

#include "fragments.h"
#include "../../assets.h"


// TODO: Prepare layout for PAL display
static layout_t layout = {
    .offset_x = 32,
    .offset_y = 24,

    .offset_text_x = 10,
    .offset_text_y = 7,

    .line_height = 18,
    .scrollbar_width = 10,
    .progressbar_height = 16,

    .border_thickness = 5,

    .main_lines = 20,

    .actions_x = 20,
    .actions_y = 405,
    .actions_lines = 2,
};


layout_t *layout_get(void) {
    return &layout;
}

void fragment_borders (surface_t *d) {
    widget_border(
        layout.offset_x,
        layout.offset_y,
        d->width - layout.offset_x,
        d->height - layout.offset_y,
        layout.border_thickness
    );
    widget_horizontal_line(
        layout.offset_x,
        d->width - layout.offset_x,
        layout.offset_y + ((layout.main_lines + 1) * layout.line_height),
        layout.border_thickness
    );
}

void fragment_scrollbar (surface_t *d, int position, int items) {
    widget_scrollbar(
        d->width - layout.offset_x - layout.scrollbar_width,
        layout.offset_y,
        layout.scrollbar_width,
        (layout.main_lines + 1) * layout.line_height,
        position,
        items,
        layout.main_lines
    );
}

void fragment_progressbar (surface_t *d, float progress) {
    widget_progressbar (
        layout.offset_x,
        layout.offset_y + ((layout.main_lines + 1) * layout.line_height) - layout.progressbar_height,
        d->width - (layout.offset_x * 2),
        layout.progressbar_height,
        progress
    );
}

void fragment_text_start (color_t color) {
    rdpq_font_begin(color);
}

void fragment_text_set_color (color_t color) {
    rdpq_set_prim_color(color);
}

int fragment_textf (int x, int y, char *fmt, ...) {
    char buffer[64];

    assets_t *assets = assets_get();

    rdpq_font_position(x, y + assets->font_height);

    va_list va;
    va_start(va, fmt);
    int n = vsnprintf(buffer, sizeof(buffer), fmt, va);
    va_end(va);

    rdpq_font_printn(assets->font, buffer, n);

    return layout.line_height;
}

void fragment_loader (surface_t *d) {
    const color_t text_color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF);

    const int offset_x = 248;
    const int offset_y = 212;
    const int text_offset_x = -39;

    widget_border(offset_x, offset_y, d->width - offset_x, d->height - offset_y, layout.border_thickness);
    fragment_text_start(text_color);
    fragment_textf((d->width / 2) + text_offset_x, (d->height / 2) - (layout.line_height / 2), "Loading…");
}
