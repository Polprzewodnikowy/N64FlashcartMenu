#include "fragments.h"


// TODO: Prepare layout for PAL display
static layout_t layout = {
    .offset_x = 32,
    .offset_y = 24,

    .offset_text_x = 10,
    .offset_text_y = 7,

    .line_height = 18,
    .scrollbar_width = 10,
    .progressbar_height = 16,

    .border_thickness = 2,

    .main_lines = 20,

    .actions_x = 20,
    .actions_y = 404,
    .actions_lines = 2,
};


layout_t *get_layout(void) {
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
        layout.actions_y - layout.border_thickness - layout.progressbar_height,
        d->width - (layout.offset_x * 2),
        layout.progressbar_height,
        progress
    );
}
