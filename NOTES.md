# N64 Flashcart Menu - CPAK & TPAK Features to Test

## Features Available in the `Settings` Section

While in the N64 Flashcart Menu, press the `START` button to access the settings. The options for `Controller Pak settings` and `Transfer Pak settings` should appear.

## Folder Creation on the SD Card

Upon opening either the `Controller Pak settings` or `Transfer Pak settings` page for the first time, the corresponding folders should be automatically created on the SD card:

* `cpak_saves` (at the root of the SD card)

  * A subfolder named `notes` will also be created within `cpak_saves`
* `gb_saves` (at the root of the SD card)

**Note:** If you open these settings pages while viewing the SD card root, the newly created folders will not immediately be visible upon exiting the menu, as the list doesn't refresh automatically. To view these folders, navigate to a sub-folder and then return to the root.

# CPAK (Controller Pak)

## Displaying CPAK Contents

* If the selected controller port has **no controller or CPAK**, the message `NO CPAK detected` should be displayed in orange.
* If the selected controller port has **a corrupted CPAK**, the message `CPAK detected (is NOT valid. Corrupted)` should be displayed in orange.

## Hot-Swapping CPAKs

* Hot-swapping CPAKs is supported. The ROM should automatically detect CPAKs being unplugged or plugged in.

## Japanese Character Support

* Verify that CPAKs with Japanese game notes correctly display the Japanese characters in note titles.

## RTC Check

CPAK dump functionality relies on RTC (Real-Time Clock) to name the dump files (e.g., `CPAK_2024.07.11_19h33m13s.mpk`).

* Attempt to dump or format a CPAK without RTC enabled or working. The operation should not proceed, and only the text `No RTC` should appear at the upper-left corner.

## Complete CPAK Dump

* With a CPAK displayed, press the `A` button to initiate a complete CPAK dump. A confirmation modal will appear. After confirming, a file should be created in `/cpak_saves` at the SD card root.
* If the CPAK is empty, you will still be able to proceed, but nothing will be dumped. *(TODO: Inform users explicitly that nothing has been saved.)*
* If the CPAK has a capacity greater than 123 blocks, the ROM currently treats it as empty. *(TODO: Test with unofficial CPAKs larger than 123 blocks.)*

### Always perform a complete CPAK dump before testing to prevent potential data loss, especially when testing individual notes features.

## Restore Complete CPAK

* Restoration can only be performed with `.mpk` files. The restoration process occurs by selecting an `.mpk` file directly from the SD card root (not in `Controller Pak settings`). After selecting a file with the `A` button, choose the target CPAK in one of the controller ports to proceed.
* If no CPAK is detected on the selected port, an informative message will appear.

**Note:** There is no validation of file content during restoration, only file extension verification. Restoring invalid files will corrupt your CPAK. *(TODO: Implement validation.)*

## CPAK single notes dump & restore is temporarily disabled due to an issue that still needs to be investigated. 

# TPAK (Currently Only Works on Controller Port 1)

## Displaying TPAK Contents

* If the selected controller port has **no controller or TPAK**, the message `NO TPAK detected` should be displayed in orange.
* If the selected port has **a controller with a TPAK**, the message `TPAK detected (is init)` should be displayed in green.
* If all is functioning correctly, the inserted game's name and details (game code, region, etc.) should appear.

## Hot-Swapping TPAKs

* Hot-swapping TPAKs is supported. The ROM should automatically detect TPAKs being unplugged or plugged in.

**Note:** Game information is retrieved from an embedded database based on ROM checksum. If a game's information is unavailable, the display will show three lines containing `CHECKSUM`. This incomplete mapping may affect the savefile dump (due to an incorrectly detected game code). Please report affected games.

## Dump GB/GBC ROM

* With a TPAK displayed, press the `L` button to dump the GB/GBC game's ROM. After confirmation, a ROM file will be created in `/gb_saves` at the SD card root, named by its game code with `.rom` extension (e.g., Gameboy Camera: `GBD-PCAX.rom`). *(TODO: Use `.gb` or `.gbc` extensions appropriately.)*

## Dump GB/GBC Savefile

* With a TPAK displayed, press the `A` button to dump the game's savefile. After confirmation, a `.sav` file will be created in `/gb_saves` at the SD card root, named by the game code (e.g., Gameboy Camera: `GBD-PCAX.sav`).

**Note:** Not all savefiles are supported at the moment. Cartridges that are at **100% not supported** are:
- MBC6
- MBC7
- Bandai TAMA5
- HUC3

### Always dump savefiles before testing restore features to ensure data safety.

## Restore GB/GBC Savefile

* Restore functionality requires a matching `.sav` file in `/gb_saves` corresponding to the game's code (e.g., `GBD-PCAX.sav`). Restore is initiated by pressing the `START` button in the `Transfer Pak settings` page.

**Note:** There's a minor visual bug causing the progress bar to appear twice. It doesn't affect functionality.

### WARNING

**Do NOT modify savefile names to match a different game code.** Doing so will corrupt or delete the cartridge's savefile and freeze the flashcart.
