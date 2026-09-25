## Menu Settings

N64FlashcartMenu automatically creates a `config.ini` file in `sd:/menu/`, which
contains the options shown in the Menu pane of the rightmost Settings tab.
If required, you can manually adjust the file (required for some advanced settings) on the SD card using your computer.

Use `L` and `R` to switch main tabs. In Settings, use Up and Down to choose Menu,
Controller, Time, Information, Flashcart, or N64, then press `A` to enter the pane.
Press `B` to return to categories. Tab switching is blocked while a pane has a
modal confirmation or operation open.

In the Menu pane, use Up and Down to select a row and `A` or Left/Right to toggle
a boolean setting. Actions such as Reset to Defaults and clearing Autoload ROM
require `A`. Default Directory is a read-only status row.

### Autoload ROM

To configure autoload, open a ROM's details from the file browser, press `R`, and
choose "Set ROM to autoload". The menu will load that ROM on subsequent boots.
Hold `Start` while the menu boots to disable autoload and clear the saved target.
The Autoload ROM row in the Menu pane shows the active ROM's path, truncated with
an ellipsis to fit, or "None" when disabled. Select it and press `A` to disable
autoload and clear the saved target immediately. Left and Right do not change it;
use the ROM options to choose a new target.

### Show Hidden Files

Shows any N64FlashcartMenu system-related and hidden files/folders. This setting is OFF by default.

Besides system-related files, the File Browser respects FAT hidden attributes, allowing you to hide files and folders from the file list. This is useful for organizing your SD card and keeping system-related, homebrew-related, or temporary files out of sight.

#### Setting Hidden Attribute on Windows

1. Right-click the file or folder you want to hide.
2. Select **Properties**.
3. In the **General** tab, check the **Hidden** checkbox.
4. Click **Apply** and then **OK**.

#### Setting Hidden Attribute on Linux

Use the `fatattr` command-line tool to manage FAT attributes:

```bash
# Set a file or folder as hidden
fatattr +h /path/to/file

# Remove the hidden attribute
fatattr -h /path/to/file

# View current attributes
fatattr /path/to/file
```

#### Setting Hidden Attribute on macOS

Use the `chflags` command-line tool to manage FAT attributes:

```bash
# Set a file or folder as hidden
chflags hidden /path/to/file

# Remove the hidden attribute
chflags nohidden /path/to/file

# View current attributes
ls -lO /path/to/file
```

### Use Save Folders

Controls whether N64FlashcartMenu should use `/saves` folders to store ROM save data. This setting is ON by default.
ON: ROM saves are saved in separate subfolders (called `\saves`, will create one `\saves` subfolder per folder).
OFF: ROM saves are saved alongside the ROM file.

### Sound Effects

The menu has default sound effects to improve the user experience. See the [sound documentation](./40_sound.md) for details. This setting is OFF by default.
