#ifndef FRAGMENTS_H__
#define FRAGMENTS_H__


#include <surface.h>


void widget_horizontal_line (int x1, int x2, int y, int thickness);
void widget_border (int x1, int y1, int x2, int y2, int thickness);
void widget_scrollbar (int x, int y, int width, int height, int position, int items, int visible_items);
void widget_progressbar (int x, int y, int width, int height, float progress);


typedef struct {
    int offset_x;
    int offset_y;

    int offset_text_x;
    int offset_text_y;

    int line_height;
    int scrollbar_width;
    int progressbar_height;

    int border_thickness;

    int main_lines;

    int actions_x;
    int actions_y;
    int actions_lines;
} layout_t;


layout_t *layout_get(void);
void fragment_borders (surface_t *d);
void fragment_scrollbar (surface_t *d, int position, int items);
void fragment_progressbar (surface_t *d, float progress);
void fragment_text_start (color_t color);
void fragment_text_set_color (color_t color);
int fragment_textf (int x, int y, char *fmt, ...);


#endif
