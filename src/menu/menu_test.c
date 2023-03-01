#include <libdragon.h>
#include "libs/menu_utils/include/menu.h"

#include "menu_test.h"


void menu_test(void) {
	// display_init(RESOLUTION_512x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE);
	// display_context_t disp = display_lock();
	// -- initializing global variables used by every menu --

	// main init function
	// (required to set default colors, can omit if setting colors using `menu_global_set_default_colors` or setting colors for items)
	// menu_global_init();

	// // initializes the sprites and offsets used. only required if using them (when creating the menu)
	// // !!!menu_global_set_sprites(menu_sprite, hand_sprite, hand_sprite_offset);

	// // optionally overriding the default colors
	// //uint32_t selected = graphics_make_color(0, 0, 0, 255);
	// //... ommiting the other colors, but you can use the function above to set each color
	// // !!!menu_global_set_default_colors(selected, enabled, disabled, background, out_of_bounds, menu_background);

	// // -- now to create the menu... --

	// // // callback to be used. it's optional, but improves code legibility when using sub-menus.
	// // void menu_callback(int option, MenuItem* item);

	// Menu* menu;
	// // create a new menu (memory_pool can be NULL if not using it)
	// uint8_t total_items = 10; // max items that the menu can have
	// uint8_t max_items = 5; // max items that will be displayed at any time (if more than this, there will be scrolling)
	// int top = 100, left = 100; // positions on-screen of the menu
	// uint8_t item_height = 16; // height in pixels that each item will have (used to calculate background bottom and scrolling)
	// menu = menu_init(NULL, total_items, max_items, top, left, item_height, NULL); // not using callbacks
	// //menu = menu_init(NULL, total_items, max_items, top, left, item_height, &menu_callback); // using callbacks

	// // add items
	// //bool enabled = true; // if the item can be selected or not. color can differ baed on colors set.
	// //void *object = &my_custom_object; // this can be held to be retrieved later for custom actions baed on selected item.

	// // adding simple text items
	// //menu_add_item(menu, "Item 1", enabled, object); // with object
	// menu_add_item(menu, "Item 2", false, NULL); // no object here, forcing disabled

	// // adding colored item (can use different colors for each item)
	// //uint32_t color_selected = graphics_make_color(0, 0, 0, 255);
	// // omitting other colors...
	// //menu_add_item_colored(menu, "Item 3", enabled, color_selected, color_enabled, color_disabled, object);

	// // // adding item as an image instead of text
	// // sprite_t *custom_image_sprite; // custom sprite to be used by the item
	// // int sprite_index = 2; // index for the custom sprite
	// // menu_add_item_image(menu, custom_image_sprite, sprite_index, enabled, object);

	// // ticks the menu to calculate changes to its state. should only be called when the menu is active.
	// //struct controller_data keys_down = get_keys_down(); // you should do this only once per frame in your normal code
	// //int selected_option = menu_tick(menu, &keys_down);
	// // if 'selected_option >= 0' then the corresponding item was selected and you can act accordingly.
	// // if using callbacks, 'selected_option' can also be '>= 0', but the function will also be called.

	// // render the menu
	// menu_render(menu, disp);

	// // -- optional set-up steps: --

	// // // add hand cursor
	// // int hand_position_x = 105; // X position of the hand. this will never change.
	// // int hand_position_y_offset = 5; // offset in pixels to be applied to the sprite when rendering the hand.
	// // menu_set_hand(menu, hand_position_x, hand_position_y_offset);

	// // add background
	// int menu_width = 200; // width in pixels of the menu. this will be the same regardless of the item width.
	// menu_set_background(menu, menu_width);
}