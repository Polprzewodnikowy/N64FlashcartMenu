#include "fragments.h"


static layout_t layout = {
    .offset_x = 20,
    .offset_y = 32,

    .offset_text_x = 10,
    .offset_text_y = 7,

    .line_height = 18,
    .scroll_width = 10,

    .border_thickness = 2,

    .main_lines = 19,

    .actions_x = 20,
    .actions_y = 394,
    .actions_lines = 2,
};


layout_t *get_layout(void) {
    return &layout;
}

void fragment_scrollbar (surface_t *d, int progress, int items) {
    widget_scrollbar(
        d->width - layout.offset_x - layout.scroll_width,
        layout.offset_y,
        layout.scroll_width,
        (layout.main_lines + 1) * layout.line_height,
        progress,
        items,
        layout.main_lines
    );
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
