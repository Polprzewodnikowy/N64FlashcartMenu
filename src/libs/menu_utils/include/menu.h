#pragma once

#include <libdragon.h>

#include "mem_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enum with the order of the background sprites in a texture. Any texture has to have this
 * order.
 */
typedef enum MenuBackgroundSprite {
	SPRITE_menu_bottom,
	SPRITE_menu_bottom_left,
	SPRITE_menu_bottom_right,
	SPRITE_menu_center,
	SPRITE_menu_left,
	SPRITE_menu_right,
	SPRITE_menu_top,
	SPRITE_menu_top_left,
	SPRITE_menu_top_right,
	SPRITE_menu_SPRITES_MAX
} MenuBackgroundSprite;

/**
 * @brief Enum that contains the state of a given item in relation with the current selected item.
 */
typedef enum MenuItemState {
	/// The item is inside the menu.
	MSIS_Inside,
	/// The item is on the edge of the menu.
	MSIS_OnBorder,
	/// The item is outside of the range of displayed items and will not be rendered.
	MSIS_Outside,
} MenuItemState;

/**
 * @brief Struct that represents an item inside the menu. Used internally when using
 * 'menu_add_item*' functions.
 */
typedef struct MenuItem {
	/// Text that will be rendered. Not used when is an image.
	char *text;
	/// Sprite that will be rendered. Used only when an image.
	sprite_t *sprite;
	/// Index of the sprite that will be rendered. Used only when an image.
	int sprite_index;

	/// X position of this item. Calculated whenever the state of the menu changes.
	int x;
	/// Y position of this item. Calculated whenever the state of the menu changes.
	int y;
	/// If the item is enabled. An enabled item can be used.
	bool enabled;
	/// If the item has custom colors. If false, will use the default colors. Default colors can be
	/// changed using 'menu_global_set_default_colors'.
	bool has_custom_colors;
	/// Color for when this item is enabled. Used when 'has_custom_colors' is true.
	uint32_t color_enabled;
	/// Color for when this item is disabled ('enabled' == false). Used when 'has_custom_colors' is
	/// true.
	uint32_t color_disabled;
	/// Color for when this item is selected. Used when 'has_custom_colors' is true.
	uint32_t color_selected;

	/// State of the item in relation to the menu. @see MenuItemState
	MenuItemState state;

	/// Custom object that is set by the user when creating the item.
	void *object;
} MenuItem;

/**
 * @brief Optional callback for when an item is used.
 *
 * @param[in] option
 *            The index of the used item.
 * @param[in] menu_item
 *            A reference to the used menu item. @see MenuItem
 */
typedef void (*fnMenuCallback)(int option, MenuItem *menu_item);

/**
 * @brief Struct used to hold a Menu. Created by the 'menu_init' function.
 */
typedef struct Menu {
	/// Array of items of this menu.
	MenuItem *items;
	/// Maximum amount of items this menu can have. Also the size of 'items'.
	uint8_t total_items;
	/// Current index when inserting new MenuItems. Has the current amount of used items.
	uint8_t current_add_index;
	/// Current selected menu item index.
	int current_menu_option;

	/// Current active submenu. If '-1', no submenu is active.
	int active_submenu;
	/// Array with references to all submenus.
	struct Menu **submenus;

	/// If this menu should be rendered even when a submenu is active.
	bool display_when_on_submenu;

	/// If this menu should display a background. Configured using 'menu_global_set_sprites' and
	/// 'menu_set_background'.
	bool display_background;
	/// Width of the background. Configured using 'menu_set_background'.
	int menu_width;

	/// If the hand should be displayed for this menu
	bool display_hand;
	/// X position of the hand cursor. Configured using 'menu_global_set_sprites' and
	/// 'menu_set_hand'.
	int hand_position_x;
	/// Y offset for the hand cursor. Configured using 'menu_global_set_sprites' and
	/// 'menu_set_hand'.
	int hand_position_y_offset;

	/// Amount of items that can be displayed at once.
	uint8_t item_vertical_limit;
	/// Height of each item inside the menu. Used to calculate Y position of items.
	uint8_t item_height;
	/// Y position of the menu.
	int top;
	/// X position of the menu.
	int left;
	/// Index of the item currently at the top of rendered menu.
	uint8_t cur_top_item;
	/// Index of the item currently at the bottom of rendered menu.
	uint8_t cur_bottom_item;

	/// Callback function for when an item is used. @see fnMenuCallback
	fnMenuCallback callback;
} Menu;

/**
 * @brief Allocates and returns a new Menu object.
 *
 * @param memory_pool
 *        The memory pool to be used. If NULL will use malloc.
 * @param total_items
 *        Total items that can be created inside the Menu.
 * @param max_items
 *        Count of items that will be displayed at once.
 * @param top
 *        Y position of the menu.
 * @param left
 *        X position of the menu.
 * @param item_height
 *        Height of each item inside the menu.
 * @param callback
 *        Callback function for when an item is used. @see fnMenuCallback
 *
 * @return A new Menu object.
 */
Menu *menu_init(MemZone *memory_pool, uint8_t total_items, uint8_t max_items, int top, int left,
				uint8_t item_height, fnMenuCallback callback);
/**
 * @brief Set the background for the Menu. Should only be called after calling
 * 'menu_global_set_sprites' at least once in the game.
 *
 * @param menu
 *        Reference to the menu object.
 * @param menu_width
 *        Width of the background.
 */
void menu_set_background(Menu *menu, int menu_width);
/**
 * @brief Resets the items inside a menu. After calling this method you can add items to refill it.
 *
 * @param menu
 *        Reference to the menu object.
 */
void menu_reset_items(Menu *menu);
/**
 * @brief Set the hand cursor for the Menu. Should only be called after calling
 * 'menu_global_set_sprites' at least once in the game.
 *
 * @param menu
 *        Reference to the menu object.
 * @param hand_position_x
 *        X position for the hand cursor.
 * @param hand_position_y_offset
 *        Y position offset for the hand cursor.
 */
void menu_set_hand(Menu *menu, int hand_position_x, int hand_position_y_offset);

/**
 * @brief Ticks the menu. Should be called every frame.
 *
 * @param menu
 *        Reference to the menu object.
 * @param keys_down
 *        Reference to the controller data for the current frame. You can get this data using the
 * 'get_keys_down()' function.
 *
 * @return Index of the item used, or '-1' is none.
 */
int menu_tick(Menu *menu, struct controller_data *keys_down);

/**
 * @brief Add a new text item to the menu without any custom colors.
 *
 * @param menu
 *        Reference to the menu object.
 * @param text
 *        Text that will be rendered.
 * @param enabled
 *        If the item will start enabled.
 * @param object
 *        Reference to a custom object that can be used later by the user.
 */
void menu_add_item(Menu *menu, char *text, bool enabled, void *object);
/**
 * @brief Add a new image item to the menu.
 *
 * @param menu
 *        Reference to the menu object.
 * @param sprite
 *        Sprite that will be used by this item.
 * @param sprite_index
 *        Index inside the sprite.
 * @param enabled
 *        If the item will start enabled.
 * @param object
 *        Reference to a custom object that can be used later by the user.
 */
void menu_add_item_image(Menu *menu, sprite_t *sprite, int sprite_index, bool enabled,
						 void *object);
/**
 * @brief Add a new text item to the menu with custom colors.
 *
 * @param menu
 *        Reference to the menu object.
 * @param text
 *        Text that will be rendered.
 * @param enabled
 *        If the item will start enabled.
 * @param color_selected
 *        Color for this object when selected.
 * @param color_enabled
 *        Color for this object when enabled and not selected.
 * @param color_disabled
 *        Color for this object when disabled and not selected.
 * @param object
 *        Reference to a custom object that can be used later by the user.
 */
void menu_add_item_colored(Menu *menu, char *text, bool enabled, uint32_t color_selected,
						   uint32_t color_enabled, uint32_t color_disabled, void *object);

/**
 * @brief Render the menu.
 *
 * @param menu
 *        Reference to the menu object.
 * @param disp
 *        The display context used in the game.
 */
void menu_render(Menu *menu, display_context_t disp);

/**
 * @brief Allocates and initializes submenus for the menu.
 *
 * @param menu
 *        Reference to the menu object.
 * @param memory_pool
 *        The memory pool to be used. If NULL will use malloc.
 * @param total_submenus
 *        Total count of submenus that will be created.
 * @param display_when_on_submenu
 *        If the menu will be displayed even when a submenu is active.
 */
void menu_init_submenus(Menu *menu, MemZone *memory_pool, uint8_t total_submenus,
						bool display_when_on_submenu);

/**
 * @brief Method that will render the borders of the menu.
 */
void menu_draw_background_borders(display_context_t disp, int top, int left, int bottom, int right);
/**
 * @brief Method that will render the center of the menu.
 */
void menu_draw_background_center(display_context_t disp, int top, int left, int bottom, int right);

/**
 * @brief Initializes the menu default colors
 */
void menu_global_init();

/**
 * @brief Set the sprite used by the background menu
 *
 * @param menu_sprite
 *        Sprite that will be used. See `sample_assets/menu.png` for example.
 *        Order should be according to enum `MenuBackgroundSprite`.
 * @param hand_sprite
 *        Sprite that will be used as the 'hand'.
 * @param hand_sprite_offset
 *        Offset of the sprite (used by `graphics_draw_sprite_trans_stride`).
 */
void menu_global_set_sprites(sprite_t *menu_sprite, sprite_t *hand_sprite,
							 uint8_t hand_sprite_offset);

/**
 * @brief Set the default colors for all menus when the item doesn't have any set.
 *
 * @param selected
 *        Color used when the item is the current selected one. Default is {0,0,255,255}
 * @param enabled
 *        Color used when the item is enabled but not selected. Default is {255,255,255,255}
 * @param disabled
 *        Color used when the item is disabled. Default is {100,100,255,255}
 * @param background
 *        Color used for the background of the text. Default is {0,0,0,0}
 * @param out_of_bounds
 *        Color used when the item is out of the bounds of the menu (when scrolling). Default is
 * {0,0,0,255}
 * @param menu_background
 *        Color used for the background. Due to 32 bits issue, we do not use the background from the
 * sprite.
 */
void menu_global_set_default_colors(uint32_t selected, uint32_t enabled, uint32_t disabled,
									uint32_t background, uint32_t out_of_bounds,
									uint32_t menu_background);

#ifdef __cplusplus
}
#endif
