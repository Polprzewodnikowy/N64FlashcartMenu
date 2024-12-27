#include "../ui_components.h"
#include "constants.h"


static const char* tabs[3] =
{
    "Files",
    "History",
    "Favorites"
};

void ui_compontents_tabs_common_draw(int selected)
{
    float width = (VISIBLE_AREA_X1 - VISIBLE_AREA_X0 - 8.0f) / 3.0f;
    // 189
    ui_components_tabs_draw(tabs, 3, selected, width);
}