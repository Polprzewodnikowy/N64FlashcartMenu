[Return to the index](./00_index.md)

:Warning: this feature is superseded on the `SummerCart64` and `64 Drive` with [Fast Reboot](./32_menu_settings.md#fast-rom-reboots).
Similar functionality can be obtained by keeping the USB port of the flashcart powered after sending a ROM via the terminal application. Further more, this feature is still possible as a build flag using `FEATURE_AUTOLOAD_ROM_ENABLED`.

## Autoloading N64 ROMs
You can set up N64FlashcartMenu to load a specific ROM directly instead of booting up the menu's graphical user interface.
If you only want to continously reload a ROM for a single gaming session, you should consider the setting for [Fast ROM reboots](./32_menu_settings.md) instead.

### How to enable autoloading
To use the autoload function, open the `N64 ROM information` screen on any ROM, then press the `R` Button on your Controller and select the `Set ROM to autoload` option. When you restart the console, N64FlashcartMenu will now only load the selected ROM, rather than the menu itself.  
**NOTE:** If you want to return to the menu, press and hold the `START` Button on your Controller while turning the console's POWER button to the ON position.
