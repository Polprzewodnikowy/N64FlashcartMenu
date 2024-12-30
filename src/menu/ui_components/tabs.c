#include "../ui_components.h"
#include "constants.h"


// Common tabs used for the main menu
static const char* tabs[3] = {
    "Files",
    "History",
    "Favorites"
};

void ui_compontents_tabs_common_draw(int selected) {
    // setup to fit between the box around the main menu
    float width = (VISIBLE_AREA_X1 - VISIBLE_AREA_X0 - 8.0f) / 3.0f;
    ui_components_tabs_draw(tabs, 3, selected, width);
}