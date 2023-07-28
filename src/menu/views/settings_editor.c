#include <libdragon.h>
#include <mini.c/src/mini.h>

#include "../settings.h"

#include "fragments/fragments.h"
#include "views.h"



static void process (menu_t *menu) {
    if (menu->actions.back) {
        menu->next_mode = MENU_MODE_BROWSER;
    }
}

static void draw (menu_t *menu, surface_t *d) {
    layout_t *layout = layout_get();

    const int text_x = layout->offset_x + layout->offset_text_x;
    int text_y = layout->offset_y + layout->offset_text_y;

    const color_t bg_color = RGBA32(0x00, 0x00, 0x00, 0xFF);
    const color_t text_color = RGBA32(0xFF, 0xFF, 0xFF, 0xFF);

    rdpq_attach(d, NULL);
    rdpq_clear(bg_color);

    // Layout
    fragment_borders(d);

    // Text start
    fragment_text_start(text_color);

	text_y += fragment_textf((d->width / 2) - 76, text_y, "MENU SETTINGS");

    // TODO: add editor here!


    // Actions bar
    text_y = layout->actions_y + layout->offset_text_y;
    text_y += fragment_textf(text_x, text_y, "A: Save");
    text_y += fragment_textf(text_x, text_y, "B: Cancel");

    rdpq_detach_show();
}


void view_settings_init (menu_t *menu) {
    // TODO: load the current settings.
}

void view_settings_display (menu_t *menu, surface_t *display) {
    process(menu);
    draw(menu, display);
}
