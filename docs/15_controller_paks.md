## Controller Paks
This feature still relies on a controller PAK being "plugged in" to a physical controller (port). The N64 hardware **CANNOT** emulate it from a flashcart.

The N64FlashcartMenu provides comprehensive Controller Pak management, including automatic per-game virtual pak saves.

---

### Virtual Controller Pak System

The Virtual Controller Pak system automatically manages Controller Pak saves on a per-game basis. This means each game can have its own dedicated save data without manually swapping physical paks or worrying about save conflicts between games.

This feature is disabled by default. To enable it, go to **Settings** and turn on **Virtual CPak**.

#### How It Works

1. **Game Launch**: When you launch a game that supports Controller Pak saves, you'll be prompted to select a virtual pak
2. **Pak Selection**: Choose an existing pak or create a new one for that game
3. **Automatic Restore**: The selected pak's data is written to your physical Controller Pak before the game boots
4. **Automatic Backup**: When you return to the menu (via reset), any changes are automatically saved back to the virtual pak file

#### Directory Structure

Virtual pak files are stored on your SD card:
```
SD:/cpak_saves/
├── NHXE/                    # Game code directory (e.g., Hexen)
│   ├── Hexen_001.pak        # First pak for this game
│   └── Hexen_002.pak        # Second pak for this game
├── NDYE/                    # Game code directory (e.g., Diddy Kong Racing)
│   └── DiddyKongRacing_001.pak
└── ...
```

Each game has its own subdirectory based on its 4-character game code. Pak files are named after the game title with a numeric suffix.

#### Pak Selection Screen

When launching a Controller Pak game, you'll see the pak selection screen:

- **[Create New Controller Pak]** - Creates a fresh, empty pak for this game
- **Existing paks** - Listed below, with `*` marking the last-used pak
- **A Button** - Select/confirm
- **B Button** - Cancel (return to ROM info)
- **R Button** - Delete selected pak

#### Power Loss Recovery

The system tracks "dirty state" to handle unexpected power loss:

1. Before booting a game, a state file records which pak is in use
2. If power is lost during gameplay, this state persists
3. On next boot, the menu detects the dirty state and:
   - If a physical pak is present: automatically backs up the pak contents
   - If no physical pak: shows a recovery dialog

This ensures save data is not lost even if you forget to return to the menu before powering off.

#### Requirements

- A physical Controller Pak must be inserted in Port 1 for saves to work
- If no Controller Pak is detected, a warning is shown but you can still launch the game

#### Error Handling

The system will show an error and prevent game launch if:
- The pak file cannot be created (disk full, permission error)
- The pak directory cannot be created
- The pak file cannot be read or is corrupted
- The pak file is too large for the physical device

This prevents launching a game with stale or incorrect save data on the physical pak.

---

### Controller Pak Manager

> [!WARNING]
> **THIS FEATURE IS EXPERIMENTAL**

The N64FlashcartMenu also includes a manual Controller Pak Manager for direct pak manipulation, accessed from the `Start` button within the main file browser.

> [!CAUTION]
> Mileage may vary when hot swapping paks without exiting and re-entering the screen (and may contain incorrect content), and/or re-powering the console.

Features:
- Full pak backup and restore (saved to `SD:/cpak_saves/`).
- Partial pak ('note') backup and restore (saved to `SD:/cpak_saves/notes/`).


#### Using the Controller Pak Manager
Use the "Controller Pak Manager" (accessed using `Start` button) to backup and manage the pak (including full backups or individual notes).
![Backup Controller Pak](./images/cpak-manager.png "Backup Controller Pak confirmation")

#### Restoring saves
To restore full backups or individual notes, browse to the saved file (usually contained within `SD:/cpak_saves/`) and follow the menu instructions.

---

### Troubleshooting

#### "Controller Pak file not found" error
This occurs if the pak file was not successfully created. Check that:
- Your SD card has free space
- The SD card is not write-protected
- The `/cpak_saves/` directory is accessible

#### Saves not persisting between sessions
Ensure you return to the menu via the reset button rather than just powering off. While the dirty state recovery should handle power loss, using reset ensures immediate backup.
