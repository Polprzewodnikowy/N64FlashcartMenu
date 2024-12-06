#include "../ui_components.h"



static const char* tabs[3] =
{
    "Browser",
    "History",
    "Favorites"
};

void ui_compontents_tabs_common_draw(int selected)
{
    ui_components_tabs_draw(tabs, 3, selected);
}