#include <libdragon.h>


void widget_horizontal_line (int x1, int x2, int y, int thickness) {
    color_t line_color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF);

    rdpq_set_mode_fill(line_color);

    rdpq_fill_rectangle(x1, y, x2, y + thickness);
}

void widget_border (int x1, int y1, int x2, int y2, int thickness) {
    color_t border_color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF);

    rdpq_set_mode_fill(border_color);

    rdpq_fill_rectangle(x1 - thickness, y1 - thickness, x2 + thickness, y1);
    rdpq_fill_rectangle(x1 - thickness, y2, x2 + thickness, y2 + thickness);

    rdpq_fill_rectangle(x1 - thickness, y1, x1, y2);
    rdpq_fill_rectangle(x2, y1, x2 + thickness, y2);
}

void widget_scrollbar (int x, int y, int width, int height, int position, int items, int visible_items) {
    color_t bg_color = RGBA32(0x3F, 0x3F, 0x3F, 0xFF);
    color_t inactive_color = RGBA32(0x5F, 0x5F, 0x5F, 0xFF);
    color_t active_color = RGBA32(0x7F, 0x7F, 0x7F, 0xFF);

    if (items < 2 || items <= visible_items) {
        rdpq_set_mode_fill(inactive_color);
        rdpq_fill_rectangle(x, y, x + width, y + height);
    } else {
        int scroll_height = (int) ((visible_items / (float) (items)) * height);
        float scroll_position = ((position / (float) (items - 1)) * (height - scroll_height));

        rdpq_set_mode_fill(bg_color);
        rdpq_fill_rectangle(x, y, x + width, y + height);

        rdpq_set_fill_color(active_color);
        rdpq_fill_rectangle(x, y + scroll_position, x + width, y + scroll_position + scroll_height);
    }
}

void widget_progressbar (int x, int y, int width, int height, float progress) {
    color_t bg_color = RGBA32(0x3F, 0x3F, 0x3F, 0xFF);
    color_t fg_color = RGBA32(0x7F, 0x7F, 0x7F, 0xFF);

    float progress_width = progress * width;

    rdpq_set_fill_color(fg_color);
    rdpq_fill_rectangle(x, y, x + progress_width, y + height);

    rdpq_set_mode_fill(bg_color);
    rdpq_fill_rectangle(x + progress_width, y, x + width, y + height);
}
