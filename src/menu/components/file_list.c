#include <stdlib.h>

#include "../components.h"
#include "../fonts.h"
#include "constants.h"


static const char *dir_prefix = "/";


static int format_file_size (char *buffer, int size) {
    if (size < 8 * 1024) {
        return sprintf(buffer, "%d B", size);
    } else if (size < 8 * 1024 * 1024) {
        return sprintf(buffer, "%d kB", size / 1024);
    } else if (size < 1 * 1024 * 1024 * 1024) {
        return sprintf(buffer, "%d MB", size / 1024 / 1024);
    } else {
        return sprintf(buffer, "%d GB", size / 1024 / 1024 / 1024);
    }
}


void component_file_list_draw (entry_t *list, int entries, int selected) {
    int starting_position = 0;

    if (entries > FILE_LIST_ENTRIES && selected >= (FILE_LIST_ENTRIES / 2)) {
        starting_position = selected - (FILE_LIST_ENTRIES / 2);
        if (starting_position >= entries - FILE_LIST_ENTRIES) {
            starting_position = entries - FILE_LIST_ENTRIES;
        }
    }

    component_file_list_scrollbar_draw(selected, entries, FILE_LIST_ENTRIES);

    if (entries == 0) {
        component_main_text_draw(
            ALIGN_LEFT, VALIGN_TOP,
            "^%02X** empty directory **",
            STL_UNKNOWN
        );
    } else {
        rdpq_paragraph_t *file_list_layout;
        rdpq_paragraph_t *layout;

        size_t name_lengths[FILE_LIST_ENTRIES];
        size_t total_length = 1;

        for (int i = 0; i < FILE_LIST_ENTRIES; i++) {
            int entry_index = starting_position + i;

            if (entry_index >= entries) {
                name_lengths[i] = 0;
            } else {
                size_t length = strlen(list[entry_index].name);
                name_lengths[i] = length;
                total_length += length + (list[entry_index].type == ENTRY_TYPE_DIR ? strlen(dir_prefix) : 0);
            }
        }

        file_list_layout = malloc(sizeof(rdpq_paragraph_t) + (sizeof(rdpq_paragraph_char_t) * total_length));
        memset(file_list_layout, 0, sizeof(rdpq_paragraph_t));
        file_list_layout->capacity = total_length;

        rdpq_paragraph_builder_begin(
            &(rdpq_textparms_t) {
                .width = FILE_LIST_MAX_WIDTH - (TEXT_MARGIN_HORIZONTAL * 2),
                .height = LAYOUT_ACTIONS_SEPARATOR_Y - VISIBLE_AREA_Y0  - (TEXT_MARGIN_VERTICAL * 2),
                .wrap = WRAP_ELLIPSES,
            },
            FNT_DEFAULT,
            file_list_layout
        );

        for (int i = 0; i < FILE_LIST_ENTRIES; i++) {
            int entry_index = starting_position + i;

            entry_t *entry = &list[entry_index];

            menu_font_style_t style;

            switch (entry->type) {
                case ENTRY_TYPE_DIR: style = STL_DIRECTORY; break;
                case ENTRY_TYPE_SAVE: style = STL_SAVE; break;
                case ENTRY_TYPE_OTHER: style = STL_UNKNOWN; break;
                case ENTRY_TYPE_IMAGE: style = STL_MEDIA; break;
                case ENTRY_TYPE_MUSIC: style = STL_MEDIA; break;
                default: style = STL_DEFAULT; break;
            }

            rdpq_paragraph_builder_style(style);

            if (entry->type == ENTRY_TYPE_DIR) {
                rdpq_paragraph_builder_span(dir_prefix, strlen(dir_prefix));
            }

            rdpq_paragraph_builder_span(entry->name, name_lengths[i]);

            if ((entry_index + 1) >= entries) {
                break;
            }

            rdpq_paragraph_builder_newline();
        }

        layout = rdpq_paragraph_builder_end();

        int highlight_height = (layout->bbox.y1 - layout->bbox.y0) / layout->nlines;
        int highlight_y = VISIBLE_AREA_Y0 + TEXT_MARGIN_VERTICAL + ((selected - starting_position) * highlight_height);

        component_box_draw(
            FILE_LIST_HIGHLIGHT_X,
            highlight_y,
            FILE_LIST_HIGHLIGHT_X + FILE_LIST_HIGHLIGHT_WIDTH,
            highlight_y + highlight_height,
            FILE_LIST_HIGHLIGHT_COLOR
        );

        rdpq_paragraph_render(
            layout,
            VISIBLE_AREA_X0 + TEXT_MARGIN_HORIZONTAL,
            VISIBLE_AREA_Y0 + TEXT_MARGIN_VERTICAL
        );

        rdpq_paragraph_free(layout);

        rdpq_paragraph_builder_begin(
            &(rdpq_textparms_t) {
                .width = VISIBLE_AREA_WIDTH - FILE_LIST_SCROLLBAR_WIDTH - (TEXT_MARGIN_HORIZONTAL * 2),
                .height = LAYOUT_ACTIONS_SEPARATOR_Y - VISIBLE_AREA_Y0  - (TEXT_MARGIN_VERTICAL * 2),
                .align = ALIGN_RIGHT,
                .wrap = WRAP_ELLIPSES,
            },
            FNT_DEFAULT,
            NULL
        );

        char file_size[8];

        for (int i = starting_position; i < entries; i++) {
            entry_t *entry = &list[i];

            if (entry->type != ENTRY_TYPE_DIR) {
                rdpq_paragraph_builder_span(file_size, format_file_size(file_size, entry->size));
            }

            if ((i + 1) == (starting_position + FILE_LIST_ENTRIES)) {
                break;
            }

            rdpq_paragraph_builder_newline();
        }

        layout = rdpq_paragraph_builder_end();

        rdpq_paragraph_render(
            layout,
            VISIBLE_AREA_X0 + TEXT_MARGIN_HORIZONTAL,
            VISIBLE_AREA_Y0 + TEXT_MARGIN_VERTICAL
        );

        rdpq_paragraph_free(layout);
    }
}
