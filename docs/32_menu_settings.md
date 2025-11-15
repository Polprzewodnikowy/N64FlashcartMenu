[Return to the index](./00_index.md)
## Menu Settings
N64FlashcartMenu automatically creates a `config.ini` file in `sd:/menu/`, which contains various settings that can be set within the menu's Settings editor.
If required, you can manually adjust the file (required for some advanced settings) on the SD card using your computer.

### Show Hidden Files
Shows any N64FlashcartMenu system-related files. This setting is OFF by default.

### Use Save Folders
Controls whether N64FlashcartMenu should use `/saves` folders to store ROM save data. This setting is ON by default.
ON: ROM saves are saved in separate subfolders (called `\saves`, will create one `\saves` subfolder per folder).
OFF: ROM saves are saved alongside the ROM file.

### Sound Effects
The menu has default sound effects to improve the user experience. See the [sound documentation](./40_sound.md) for details. This setting is OFF by default.

### Fast ROM reboots
Certain flashcarts support the ability to use the N64 `RESET` button for re-loading the last game, rather than returning to the menu. When enabled (and if supported by your flashcart), the power switch must be toggled to return to the menu.  

> [!TIP] if a USB cable is connected to the flashcart, the last game will continue to be re-loaded. If debugging the menu, make sure this option is off! 

Fast Rebooting a 64DD disk once will result in a blank screen. Twice will return to menu. This is expected until disk swapping is implemented.
This setting is OFF by default.
