#include "../include/menu.h"

#include "../include/memory_alloc.h"

void menu_scroll_up(Menu *menu);
void menu_scroll_down(Menu *menu);

uint32_t MENU_C_SELECTED;
uint32_t MENU_C_ENABLED;
uint32_t MENU_C_DISABLED;
uint32_t MENU_C_BACKGROUND;
uint32_t MENU_C_MENU_BACKGROUND;
uint32_t MENU_C_OUT_OF_BOUNDS;

sprite_t *menu_background_sprite;
sprite_t *menu_hand_sprite;
uint8_t menu_hand_sprite_offset;

void menu_global_init() {
	MENU_C_SELECTED = graphics_make_color(0, 0, 255, 255);
	MENU_C_ENABLED = graphics_make_color(255, 255, 255, 255);
	MENU_C_DISABLED = graphics_make_color(100, 100, 255, 255);
	MENU_C_BACKGROUND = graphics_make_color(0, 0, 0, 0);
	MENU_C_OUT_OF_BOUNDS = graphics_make_color(0, 0, 0, 255);
	MENU_C_MENU_BACKGROUND = graphics_make_color(255, 255, 255, 255);
}

void menu_global_set_default_colors(uint32_t selected, uint32_t enabled, uint32_t disabled,
									uint32_t background, uint32_t out_of_bounds,
									uint32_t menu_background) {
	MENU_C_SELECTED = selected;
	MENU_C_ENABLED = enabled;
	MENU_C_DISABLED = disabled;
	MENU_C_BACKGROUND = background;
	MENU_C_OUT_OF_BOUNDS = out_of_bounds;
	MENU_C_MENU_BACKGROUND = menu_background;
}

void menu_global_set_sprites(sprite_t *menu_sprite, sprite_t *hand_sprite,
							 uint8_t hand_sprite_offset) {
	menu_background_sprite = menu_sprite;
	menu_hand_sprite = hand_sprite;
	menu_hand_sprite_offset = hand_sprite_offset;
}

Menu *menu_init(MemZone *memory_pool, uint8_t total_items, uint8_t max_items, int top, int left,
				uint8_t item_height, fnMenuCallback callback) {
	Menu *menu = MEM_ALLOC(sizeof(Menu), memory_pool);
	menu->total_items = total_items;
	menu->items = MEM_ALLOC(sizeof(MenuItem) * total_items, memory_pool);
	menu->current_menu_option = 0;
	menu_reset_items(menu);

	menu->active_submenu = -1;
	menu->submenus = NULL;

	menu->display_when_on_submenu = false;

	menu->display_background = false;
	menu->menu_width = 0;

	menu->item_vertical_limit = max_items;
	menu->item_height = item_height;
	menu->top = top;
	menu->left = left;
	menu->cur_top_item = 0;
	menu->cur_bottom_item = 0;

	menu->callback = callback;

	menu->display_hand = false;
	menu->hand_position_x = 0;
	menu->hand_position_y_offset = 0;

	return menu;
}

void menu_set_background(Menu *menu, int menu_width) {
	menu->display_background = true;
	menu->menu_width = menu_width;
}

void menu_reset_items(Menu *menu) {
	menu->current_add_index = 0;
	for (uint8_t i = 0; i < menu->total_items; ++i) {
		menu->items[i].enabled = false;
		menu->items[i].text = NULL;
		menu->items[i].x = 0;
		menu->items[i].y = 0;
	}
}

void menu_set_hand(Menu *menu, int hand_position_x, int hand_position_y_offset) {
	menu->display_hand = true;
	menu->hand_position_x = hand_position_x;
	menu->hand_position_y_offset = hand_position_y_offset;
}

void menu_add_item(Menu *menu, char *text, bool enabled, void *object) {
	if (menu->current_add_index >= menu->total_items)
		return;

	MenuItem *item = &menu->items[menu->current_add_index];
	item->text = text;
	item->x = menu->left;
	item->y = menu->top + (menu->item_height * menu->current_add_index);
	item->enabled = enabled;
	item->has_custom_colors = false;
	item->sprite_index = -1;
	item->object = object;

	if (menu->current_add_index == menu->item_vertical_limit)
		item->state = MSIS_OnBorder;
	else if (menu->current_add_index > menu->item_vertical_limit)
		item->state = MSIS_Outside;
	else
		item->state = MSIS_Inside;

	menu->current_add_index++;

	menu->cur_bottom_item = menu->current_add_index >= menu->item_vertical_limit
								? menu->item_vertical_limit - 1
								: menu->current_add_index - 1;
}

void menu_add_item_image(Menu *menu, sprite_t *sprite, int sprite_index, bool enabled,
						 void *object) {
	if (menu->current_add_index >= menu->total_items)
		return;

	MenuItem *item = &menu->items[menu->current_add_index];
	item->sprite = sprite;
	item->sprite_index = sprite_index;
	item->x = menu->left;
	item->y = menu->top + (menu->item_height * menu->current_add_index);
	item->enabled = enabled;
	item->has_custom_colors = false;
	item->object = object;

	if (menu->current_add_index == menu->item_vertical_limit)
		item->state = MSIS_OnBorder;
	else if (menu->current_add_index > menu->item_vertical_limit)
		item->state = MSIS_Outside;
	else
		item->state = MSIS_Inside;

	menu->current_add_index++;

	menu->cur_bottom_item = menu->current_add_index >= menu->item_vertical_limit
								? menu->item_vertical_limit - 1
								: menu->current_add_index - 1;
}

void menu_add_item_colored(Menu *menu, char *text, bool enabled, uint32_t color_selected,
						   uint32_t color_enabled, uint32_t color_disabled, void *object) {
	if (menu->current_add_index >= menu->total_items)
		return;

	MenuItem *item = &menu->items[menu->current_add_index];
	item->text = text;
	item->x = menu->left;
	item->y = menu->top + (menu->item_height * menu->current_add_index);
	item->enabled = enabled;
	item->has_custom_colors = true;
	item->sprite_index = -1;
	item->object = object;

	item->color_selected = color_selected;
	item->color_enabled = color_enabled;
	item->color_disabled = color_disabled;

	if (menu->current_add_index == menu->item_vertical_limit)
		item->state = MSIS_OnBorder;
	else if (menu->current_add_index > menu->item_vertical_limit)
		item->state = MSIS_Outside;
	else
		item->state = MSIS_Inside;

	menu->current_add_index++;

	menu->cur_bottom_item = menu->current_add_index >= menu->item_vertical_limit
								? menu->item_vertical_limit - 1
								: menu->current_add_index - 1;
}

int menu_tick(Menu *menu, struct controller_data *keys_down) {
	if (menu->active_submenu >= 0 && menu->submenus) {
		Menu **menus = menu->submenus;
		Menu *active_submenu = menus[menu->active_submenu];
		int option = menu_tick(active_submenu, keys_down);

		// go back from submenu
		if (option == -2) {
			menu->active_submenu = -1;
			return -1;
		}

		return option;
	}
	if (menu->current_add_index <= 0)
		return -1;

	if (keys_down->c[0].up) {
		if (menu->current_menu_option > 0) {
			menu->current_menu_option -= 1;
			menu_scroll_up(menu);
		}
	} else if (keys_down->c[0].down) {
		if (menu->current_menu_option < menu->current_add_index - 1) {
			menu->current_menu_option += 1;
			menu_scroll_down(menu);
		}
	} else if (keys_down->c[0].A || keys_down->c[0].start) {
		if (menu->items[menu->current_menu_option].enabled) {
			if (menu->callback) {
				menu->callback(menu->current_menu_option, &menu->items[menu->current_menu_option]);
			}
			return menu->current_menu_option;
		}
	} else if (keys_down->c[0].B) {
		return -2;
	}

	return -1;
}

void menu_render(Menu *menu, display_context_t disp) {
	if (menu->active_submenu >= 0 && menu->submenus) {
		Menu *active_submenu = menu->submenus[menu->active_submenu];

		menu_render(active_submenu, disp);

		if (!menu->display_when_on_submenu)
			return;
	}

	// render window background
	if (menu->display_background) {
		const int item_count = menu->current_add_index < menu->item_vertical_limit
								   ? menu->current_add_index
								   : menu->item_vertical_limit;

		const int menu_left = menu->left - 8;
		const int menu_top = menu->top - menu->item_height;
		const int menu_bottom = menu->top + (menu->item_height * item_count);
		const int menu_right = menu->left + menu->menu_width;

		menu_draw_background_center(disp, menu_top, menu_left, menu_bottom, menu_right);
	}

	for (uint8_t i = 0; i < menu->current_add_index; ++i) {
		if (menu->items[i].state == MSIS_Outside)
			continue;

		if (menu->items[i].sprite_index == -1) {
			if (menu->items[i].state == MSIS_Inside) {
				if (i == menu->current_menu_option) {
					if (menu->items[i].has_custom_colors) {
						graphics_set_color(menu->items[i].color_selected, MENU_C_BACKGROUND);
					} else {
						graphics_set_color(MENU_C_SELECTED, MENU_C_BACKGROUND);
					}
				} else if (menu->items[i].enabled) {
					if (menu->items[i].has_custom_colors) {
						graphics_set_color(menu->items[i].color_enabled, MENU_C_BACKGROUND);
					} else {
						graphics_set_color(MENU_C_ENABLED, MENU_C_BACKGROUND);
					}
				} else {
					if (menu->items[i].has_custom_colors) {
						graphics_set_color(menu->items[i].color_disabled, MENU_C_BACKGROUND);
					} else {
						graphics_set_color(MENU_C_DISABLED, MENU_C_BACKGROUND);
					}
				}
			} else {
				graphics_set_color(MENU_C_OUT_OF_BOUNDS, MENU_C_BACKGROUND);
			}

			graphics_draw_text(disp, menu->items[i].x, menu->items[i].y, menu->items[i].text);
		} else {
			graphics_draw_sprite_trans_stride(disp, menu->items[i].x, menu->items[i].y,
											  menu->items[i].sprite, menu->items[i].sprite_index);
		}
	}

	if (menu->display_background) {
		const int item_count = menu->current_add_index < menu->item_vertical_limit
								   ? menu->current_add_index
								   : menu->item_vertical_limit;

		const int menu_left = menu->left - 8;
		const int menu_top = menu->top - menu->item_height;
		const int menu_bottom = menu->top + (menu->item_height * item_count);
		const int menu_right = menu->left + menu->menu_width;

		menu_draw_background_borders(disp, menu_top, menu_left, menu_bottom, menu_right);
	}

	// render hand icon
	if (menu->display_hand && menu->current_add_index > 0) {
		int option = menu->current_menu_option;
		if (menu->current_add_index > 0)
			graphics_draw_sprite_trans_stride(disp, menu->hand_position_x,
											  menu->items[option].y + menu->hand_position_y_offset,
											  menu_hand_sprite, menu_hand_sprite_offset);
	}
}

void menu_init_submenus(Menu *menu, MemZone *memory_pool, uint8_t total_submenus,
						bool display_when_on_submenu) {
	// do not recreate submenus
	if (menu->submenus)
		return;

	menu->submenus = MEM_ALLOC(sizeof(Menu *) * total_submenus, memory_pool);
	menu->display_when_on_submenu = display_when_on_submenu;
}

void menu_draw_background_borders(display_context_t disp, int top, int left, int bottom,
								  int right) {
	graphics_draw_sprite_trans_stride(disp, left, top, menu_background_sprite,
									  SPRITE_menu_top_left);

	graphics_draw_sprite_trans_stride(disp, left, bottom, menu_background_sprite,
									  SPRITE_menu_bottom_left);

	graphics_draw_sprite_trans_stride(disp, right, top, menu_background_sprite,
									  SPRITE_menu_top_right);

	graphics_draw_sprite_trans_stride(disp, right, bottom, menu_background_sprite,
									  SPRITE_menu_bottom_right);

	const int repeat_x = (right - left - 8) / 8;
	for (size_t i = 0; i < repeat_x; ++i) {
		graphics_draw_sprite_trans_stride(disp, left + 8 + (i * 8), top, menu_background_sprite,
										  SPRITE_menu_top);
		graphics_draw_sprite_trans_stride(disp, left + 8 + (i * 8), bottom, menu_background_sprite,
										  SPRITE_menu_bottom);
	}
	const int repeat_y = ((bottom - top) / 8) - 1;
	for (size_t i = 0; i < repeat_y; ++i) {
		graphics_draw_sprite_trans_stride(disp, left, top + 8 + (i * 8), menu_background_sprite,
										  SPRITE_menu_left);
		graphics_draw_sprite_trans_stride(disp, right, top + 8 + (i * 8), menu_background_sprite,
										  SPRITE_menu_right);
	}
}

void menu_draw_background_center(display_context_t disp, int top, int left, int bottom, int right) {
	// TODO: remove this "hack" and properly render the texture. Cause: libdragon issue with 32 bits
	left += 8;
	top += 8;

	rdp_sync(SYNC_PIPE);

	rdp_attach_display(disp);
	rdp_enable_blend_fill();
	rdp_set_default_clipping();

	rdp_sync(SYNC_PIPE);
	rdp_set_blend_color(MENU_C_MENU_BACKGROUND);
	rdp_draw_filled_triangle(left, top, right, top, right, bottom);
	rdp_draw_filled_triangle(left, top, left, bottom, right, bottom);

	rdp_detach_display();
}

void menu_scroll_fix_y(Menu *menu) {
	if (menu->cur_top_item > 0) {
		menu->items[menu->cur_top_item - 1].y = menu->top - menu->item_height;
	}
	for (size_t ix = menu->cur_top_item, i = 0; i <= menu->cur_bottom_item; ++i, ++ix) {
		menu->items[ix].y = menu->top + (i * menu->item_height);
	}
	if (menu->cur_bottom_item + 1 < menu->current_add_index) {
		menu->items[menu->cur_bottom_item + 1].y = menu->top +
												   (menu->item_vertical_limit * menu->item_height);
	}
}

void menu_scroll_up(Menu *menu) {
	if (menu->current_menu_option < menu->cur_top_item) {
		if (menu->cur_bottom_item + 1 < menu->current_add_index)
			menu->items[menu->cur_bottom_item + 1].state = MSIS_Outside;
		menu->items[menu->cur_bottom_item].state = MSIS_OnBorder;

		if (menu->cur_top_item - 2 >= 0)
			menu->items[menu->cur_top_item - 2].state = MSIS_OnBorder;
		menu->items[menu->current_menu_option].state = MSIS_Inside;

		menu->cur_top_item--;
		menu->cur_bottom_item--;

		menu_scroll_fix_y(menu);
	}
}

void menu_scroll_down(Menu *menu) {
	if (menu->current_menu_option > menu->cur_bottom_item) {
		if (menu->cur_top_item > 0)
			menu->items[menu->cur_top_item - 1].state = MSIS_Outside;
		menu->items[menu->cur_top_item].state = MSIS_OnBorder;

		menu->items[menu->current_menu_option].state = MSIS_Inside;

		if (menu->current_menu_option + 1 < menu->current_add_index)
			menu->items[menu->current_menu_option + 1].state = MSIS_OnBorder;

		menu->cur_top_item++;
		menu->cur_bottom_item++;

		menu_scroll_fix_y(menu);
	}
}
