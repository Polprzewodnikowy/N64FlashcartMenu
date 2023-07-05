#ifndef FRAGMENTS_H__
#define FRAGMENTS_H__


#include <surface.h>


void widget_scrollbar (int x, int y, int width, int height, int position, int items, int visible_items);
void widget_horizontal_line (int x1, int x2, int y, int thickness);
void widget_border (int x1, int y1, int x2, int y2, int thickness);


typedef struct {
    int offset_x;
    int offset_y;

    int offset_text_x;
    int offset_text_y;

    int line_height;
    int scroll_width;

    int border_thickness;

    int main_lines;

    int actions_x;
    int actions_y;
    int actions_lines;
} layout_t;


layout_t *get_layout(void);
void fragment_borders (surface_t *d);
void fragment_scrollbar (surface_t *d, int progress, int items);


#endif
