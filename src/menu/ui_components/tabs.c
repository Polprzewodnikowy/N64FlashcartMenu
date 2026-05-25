/**
 * @file tabs.c
 * @brief Implementation of the tabs UI component.
 * @ingroup ui_components
 */

#include "../ui_components.h"
#include "constants.h"

/**
 * @brief Common tab labels used for the main menu.
 */
static const char *tabs[] = {
    "Files",
    "History",
    "Favorites",
    NULL
};

/**
 * @brief Draw the common tabs used for the main menu.
 *
 * @param selected Index of the currently selected tab.
 */
void ui_components_tabs_common_draw(int selected)
{
    uint8_t tabs_count = 3;
    float width = (VISIBLE_AREA_X1 - VISIBLE_AREA_X0 - 8.0f) / (tabs_count + 1 * 0.5f);
    ui_components_tabs_draw(tabs, tabs_count, selected, width);
}