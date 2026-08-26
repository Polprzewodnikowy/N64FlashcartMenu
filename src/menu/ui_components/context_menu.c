/**
 * @file context_menu.c
 * @brief Implementation of the context menu UI component.
 * @ingroup ui_components
 */

#include "../ui_components.h"
#include "../fonts.h"
#include "../sound.h"
#include "constants.h"

/**
 * @brief Get the current submenu from a context menu component.
 *
 * @param cm Pointer to the context menu component.
 * @return Pointer to the current submenu (deepest submenu in the chain).
 */
static component_context_menu_t *get_current_submenu(component_context_menu_t *cm) {
    while (cm->submenu != NULL) {
        cm = cm->submenu;
    }
    return cm;
}

/**
 * @brief Initialize the context menu component.
 *
 * @param cm Pointer to the context menu component.
 */
void ui_components_context_menu_init(component_context_menu_t *cm) {
    cm->row_selected = -1;
    cm->row_count = 0;
    cm->hide_pending = false;
    cm->parent = NULL;
    for (int i = 0; (cm->list[i].text) != NULL; i++) {
        cm->row_count += 1;
    }
}

/**
 * @brief Show the context menu component (reset selection and submenu).
 *
 * @param cm Pointer to the context menu component.
 */
void ui_components_context_menu_show(component_context_menu_t *cm) {
    cm->row_selected = 0;
    cm->submenu = NULL;
}

/**
 * @brief Process the context menu actions (navigation, selection, etc).
 *
 * @param menu Pointer to the menu structure.
 * @param cm Pointer to the context menu component.
 * @return true if the context menu is processed, false otherwise.
 */
bool ui_components_context_menu_process(menu_t *menu, component_context_menu_t *cm) {
    if (!cm || (cm->row_selected < 0)) {
        return false;
    }

    component_context_menu_t *top = cm;

    cm = get_current_submenu(cm);

    if (menu->actions.back) {
        if (cm->parent) {
            cm->parent->submenu = NULL;
        } else {
            cm->hide_pending = true;
        }
        sound_play_effect(SFX_EXIT);
    } else if (menu->actions.enter) {
        if (cm->list[cm->row_selected].submenu) {
            cm->submenu = cm->list[cm->row_selected].submenu;
            ui_components_context_menu_init(cm->submenu);
            // Use custom default selection if available, otherwise default to 0
            if (cm->submenu->get_default_selection) {
                cm->submenu->row_selected = cm->submenu->get_default_selection(menu);
            } else {
                cm->submenu->row_selected = 0;
            }
            cm->submenu->parent = cm;
        } else if (cm->list[cm->row_selected].action) {
            cm->list[cm->row_selected].action(menu, cm->list[cm->row_selected].arg);
            top->hide_pending = true;
        }
        sound_play_effect(SFX_ENTER);
    } else if (menu->actions.go_up) {
        cm->row_selected -= 1;
        if (cm->row_selected < 0) {
            cm->row_selected = 0;
        }
        sound_play_effect(SFX_CURSOR);
    } else if (menu->actions.go_down) {
        cm->row_selected += 1;
        if (cm->row_selected >= cm->row_count) {
            cm->row_selected = (cm->row_count - 1);
        }
        sound_play_effect(SFX_CURSOR);
    }

    return true;
}

/**
 * @brief Draw the context menu UI component.
 *
 * @param cm Pointer to the context menu component.
 */
void ui_components_context_menu_draw(component_context_menu_t *cm) {
    if (!cm || (cm->row_selected < 0)) {
        return;
    }

    component_context_menu_t *top = cm;

    cm = get_current_submenu(cm);

    rdpq_paragraph_builder_begin(
        &(rdpq_textparms_t) {
            .width = VISIBLE_AREA_WIDTH,
            .height = VISIBLE_AREA_HEIGHT,
            .align = ALIGN_CENTER,
            .valign = VALIGN_CENTER,
            .line_spacing = TEXT_LINE_SPACING_ADJUST,
        },
        FNT_DEFAULT,
        NULL
    );

    for (int i = 0; i < cm->row_count; i++) {
        const char *text = cm->list[i].text;
        rdpq_paragraph_builder_span(text, strlen(text));
        if (cm->list[i + 1].text != NULL) {
            rdpq_paragraph_builder_newline();
        }
    }

    rdpq_paragraph_t *layout = rdpq_paragraph_builder_end();

    int width = layout->bbox.x1 - layout->bbox.x0 + MESSAGEBOX_MARGIN;
    int height = layout->bbox.y1 - layout->bbox.y0 + MESSAGEBOX_MARGIN;

    ui_components_dialog_draw(width, height);

    int highlight_x0 = DISPLAY_CENTER_X - (width / 2);
    int highlight_x1 = DISPLAY_CENTER_X + (width / 2);
    int highlight_height = (layout->bbox.y1 - layout->bbox.y0) / layout->nlines;
    int highlight_y = VISIBLE_AREA_Y0 + layout->bbox.y0 + ((cm->row_selected) * highlight_height);

    ui_components_box_draw(
        highlight_x0,
        highlight_y,
        highlight_x1,
        highlight_y + highlight_height,
        CONTEXT_MENU_HIGHLIGHT_COLOR
    );

    rdpq_paragraph_render(layout, VISIBLE_AREA_X0, VISIBLE_AREA_Y0);

    rdpq_paragraph_free(layout);

    if (top->hide_pending) {
        top->hide_pending = false;
        top->row_selected = -1;
    }
}
