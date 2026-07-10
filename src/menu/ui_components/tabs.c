/**
 * @file tabs.c
 * @brief Implementation of the tabs UI component.
 * @ingroup ui_components
 */

#include "../ui_components.h"
#include "constants.h"

/**
 * @brief Draw the common tabs used for the main menu.
 *
 * @param tabs Array of tab labels.
 * @param selected Index of the currently selected tab.
 */
void ui_components_tabs_common_draw(const char **tabs, int selected)
{
    uint8_t tabs_count = 0;
    while (tabs[tabs_count] != NULL) {
        tabs_count++;
    }
    float width = (VISIBLE_AREA_X1 - VISIBLE_AREA_X0 - 8.0f) / (tabs_count + 1 * 0.5f);
    ui_components_tabs_draw(tabs, tabs_count, selected, width);
}