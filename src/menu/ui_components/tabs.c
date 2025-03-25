/**
 * @file tabs.c
 * @brief Tabs component implementation
 * @ingroup ui_components
 */

#include "../ui_components.h"
#include "constants.h"

/** @brief Common tabs used for the main menu */
static const char *tabs[] = {
    "Files",
    "History",
    "Favorites",
    NULL
};

/**
 * @brief Draws the common tabs used for the main menu.
 * 
 * @param selected The index of the currently selected tab.
 */
void ui_components_tabs_common_draw(int selected)
{
    uint8_t tabs_count = 3;
    float width = (VISIBLE_AREA_X1 - VISIBLE_AREA_X0 - 8.0f) / (tabs_count + 1 * 0.5f);
    ui_components_tabs_draw(tabs, tabs_count, selected, width);
}