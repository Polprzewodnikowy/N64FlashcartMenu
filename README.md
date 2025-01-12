![Build](https://github.com/polprzewodnikowy/N64FlashcartMenu/actions/workflows/build.yml/badge.svg)
![GitHub Org's stars](https://img.shields.io/github/stars/Polprzewodnikowy/N64FlashcartMenu)
[![Average time to resolve an issue](http://isitmaintained.com/badge/resolution/Polprzewodnikowy/N64FlashcartMenu.svg)](http://isitmaintained.com/project/Polprzewodnikowy/N64FlashcartMenu "Average time to resolve an issue")
[![Percentage of issues still open](http://isitmaintained.com/badge/open/Polprzewodnikowy/N64FlashcartMenu.svg)](http://isitmaintained.com/project/Polprzewodnikowy/N64FlashcartMenu "Percentage of issues still open")
[![#yourfirstpr](https://img.shields.io/badge/first--timers--only-friendly-blue.svg)](https://github.com/Polprzewodnikowy/N64FlashcartMenu/CONTRIBUTING.md)

# N64 Flashcart Menu
An open source menu for N64 flashcarts.  
![N64FlashcartMenu menu information](./docs/images/menu-information.png "N64FlashcartMenu menu information")

## Supported Flashcarts
This menu aims to support as many N64 flashcarts as possible.

### Fully supported
* SummerCart64
* 64Drive

### Work in Progress
* ED64
* ED64P


## Current (notable) menu features
* Fully Open Source.
* Loads all known N64 games, even if they are byteswapped.
* Fully emulates the 64DD and loads 64DD disks (SummerCart64 only).
* Emulator support (NES, SNES, GB, GBC, SMS, GG, CHF) ROMs.
* N64 ROM box art image support.
* Background image (PNG) support.
* Comprehensive ROM save database (including homebrew headers).
* Comprehensive ROM information display.
* Real Time Clock support.
* Music playback (MP3).
* Menu sound effects.
* N64 ROM autoload.
* ROM information descriptions.
* ROM history and favorites.


## Documentation
* [Getting started guide](./docs/00_getting_started_sd.md)
* [Menu controls](./docs/01_menu_controls.md)
* [Menu customization](./docs/07_menu_customization.md)
* [Developer guide](./docs/99_developer_guide.md)

## Video showcase (as of Oct 12 2023)
[![N64FlashcartMenu Showcase](http://img.youtube.com/vi/6CKImHTifDA/0.jpg)](http://www.youtube.com/watch?v=6CKImHTifDA "N64FlashcartMenu Showcase (Oct 12 2023)")


## Aims
* Support as many N64 FlashCarts as possible.
* Be open source, using permissively licensed third-party libraries.
* Be testable in an emulated environment (Ares).
* Encourage active development from community members and N64 FlashCart owners.
* Support as many common mods and features as possible.


## Experimental features
These features are subject to change:

### ROM Info descriptions
To add a ROM description (shown when loading a ROM), add a file with the content
```
[metadata]
description=This is the ROM description that does X Y Z.
```
The file line endings must use the linux `LF` endings only (CRLF is not supported) and is limited to 300 characters.

### N64 ROM autoload
To use the autoload function, while on the `N64 ROM information` display, press the `R` button on your joypad and select the `Set ROM to autoload` option. When you restart the console, it will now only load the selected ROM rather than the menu.  
**NOTE:** To return to the menu, hold the joypad `Start` button while powering on the console.

### GamePak sprites
To use N64 GamePak sprites, place PNG files within the `sd:/menu/boxart/` folder.

#### Supported sprites
These must be `PNG` files that use the following dimensions:
* Standard N64 GamePak boxart sprites: 158x112
* Japanese N64 GamePak boxart sprites: 112x158
* 64DD boxart sprites: 129x112

They will be loaded by directories using each character (case-sensitive) of the full 4 character Game Code (as identified in the menu ROM information).
i.e. for GoldenEye NTSC USA (NGEE), this would be `sd:/menu/boxart/N/G/E/E/boxart_front.png`.
i.e. for GoldenEye PAL (NGEP), this would be `sd:/menu/boxart/N/G/E/P/boxart_front.png`.

To improve compatibility between regions (as a fallback), you may exclude the region ID (last matched directory) for GamePaks to match with 3 letter IDs instead:  
i.e. for GoldenEye, this would be `sd:/menu/boxart/N/G/E/boxart_front.png`.

**Warning**: Excluding the region ID may show the wrong boxart.
**Note**: For future support, boxart sprites should also include:
* `boxart_back.png`
* `boxart_top.png`
* `boxart_bottom.png`
* `boxart_left.png`
* `boxart_right.png`
* `gamepak_front.png`
* `gamepak_back.png`

As a starting point, here is a link to a boxart pack, that has `boxart_front.png`:
* [boxart](https://drive.google.com/file/d/1IpCmFqmGgGwKKmlRBxYObfFR9XywaC6n/view?usp=drive_link)


### Menu Settings
The Menu creates a `config.ini` file in `sd:/menu/` which contains various settings that are used and can be set within the menu.
If required, you can manually adjust the file on the SD card using your computer.

### Datel cheats
(Action Replay and GameShark codes)
Supported, but no GUI available (yet) to edit them.


## Flashcart specific

### SC64
* Ensure the cart has the latest [firmware](https://github.com/Polprzewodnikowy/SummerCart64/releases/latest) installed.
* Download the latest `sc64menu.n64` file from the [releases](https://github.com/Polprzewodnikowy/N64FlashcartMenu/releases/) page, then put it in the root directory of your SD card.

![SC64 flashcart information](./docs/images/sc64-flashcart-information.png "SC64 flashcart information")


### 64drive
* Ensure the cart has the latest [firmware](https://64drive.retroactive.be/support.php) installed.
* Download the latest `menu.bin` file from the [releases](https://github.com/Polprzewodnikowy/N64FlashcartMenu/releases/) page, then put it in the root directory of your SD card.


### ED64 - WIP - UNTESTED AND UNSUPPORTED - USE AT OWN RISK
Currently not supported, but work is in progress (See [PRs](https://github.com/Polprzewodnikowy/N64FlashcartMenu/pulls)).
**Warning**: The menu may be able to load ROMs but cannot guarantee save functionality. Existing saves may be corrupted.

#### ED64 (Vseries)
The aim is to reach feature parity with [ED64-UnofficialOS](https://github.com/n64-tools/ED64-UnofficialOS-binaries) / [ED64-OfficialOS](https://krikzz.com/pub/support/everdrive-64/v2x-v3x/os-bin/).
Download the `OS64.v64` ROM from the latest [action run - assets] and place it in the `/ED64` folder.

#### ED64 (X series)
The aim is to reach feature parity with [OS](https://krikzz.com/pub/support/everdrive-64/x-series/OS/) for now.
Download the `OS64.v64` ROM from the latest [action run - assets] and place it in the `/ED64` folder.

#### ED64 (P clone)
Download the `OS64P.v64` ROM from the latest [action run - assets] and place it in the `/ED64P` folder.
The aim is to reach feature parity with [Altra64](https://github.com/networkfusion/altra64)


# Open source software and licenses used

* [libdragon](https://github.com/DragonMinded/libdragon) (UNLICENSE License)
* [libspng](https://github.com/randy408/libspng) (BSD 2-Clause License)
* [mini.c](https://github.com/univrsal/mini.c) (BSD 2-Clause License)
* [minimp3](https://github.com/lieff/minimp3) (CC0 1.0 Universal)
* [miniz](https://github.com/richgel999/miniz) (MIT License)

## Sounds
See [License](https://pixabay.com/en/service/license-summary/) for the following sounds:
* [Cursor sound](https://pixabay.com/en/sound-effects/click-buttons-ui-menu-sounds-effects-button-7-203601/) by Skyscraper_seven (Free to use)
* [Actions (Enter, Back) sound](https://pixabay.com/en/sound-effects/menu-button-user-interface-pack-190041/) by Liecio (Free to use)
* [Error sound](https://pixabay.com/en/sound-effects/error-call-to-attention-129258/) by Universfield (Free to use)
