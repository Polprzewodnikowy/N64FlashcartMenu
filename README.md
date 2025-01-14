![Build](https://github.com/polprzewodnikowy/N64FlashcartMenu/actions/workflows/build.yml/badge.svg)
![GitHub Org's stars](https://img.shields.io/github/stars/Polprzewodnikowy/N64FlashcartMenu)
[![Average time to resolve an issue](http://isitmaintained.com/badge/resolution/Polprzewodnikowy/N64FlashcartMenu.svg)](http://isitmaintained.com/project/Polprzewodnikowy/N64FlashcartMenu "Average time to resolve an issue")
[![Percentage of issues still open](http://isitmaintained.com/badge/open/Polprzewodnikowy/N64FlashcartMenu.svg)](http://isitmaintained.com/project/Polprzewodnikowy/N64FlashcartMenu "Percentage of issues still open")
[![#yourfirstpr](https://img.shields.io/badge/first--timers--only-friendly-blue.svg)](https://github.com/Polprzewodnikowy/N64FlashcartMenu/CONTRIBUTING.md)

# N64 Flashcart Menu
An open source menu for N64 flashcarts and aims to support as many as possible. The menu is not affiliated with any partuclar flashcart and does not necessarily expose all capable firmware features.  
**This project considers flashcart sellers that include this menu or copyrighted ROM's as part of their product offering as licence offenders which may result in future action.**  

## Flashcart Supported
This menu aims to support as many N64 flashcarts as possible. The current state is:

### Supported
* SummerCart64
* 64Drive

### Work in Progress (pre-release only)
* ED64 (X and V series)
* ED64P (clones)

### Not planned
* Doctor V64
* PicoCart
* DaisyDrive


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
* ROM history and favorites (pre-release only).


## Documentation
Please take a moment to browse the current documentation:  
[Index](./docs/00_index.md)


## Aims
* Support as many N64 Flashcarts as possible.
* Be open source, using permissively licensed third-party libraries.
* Be testable in an emulated environment (Ares).
* Encourage active development from community members and N64 FlashCart owners.
* Support as many common mods and features as possible (flashcart dependent).


## Flashcart specific

### SC64
* Ensure the cart has the latest [firmware](https://github.com/Polprzewodnikowy/SummerCart64/releases/latest) installed.
* Download the latest `sc64menu.n64` file from the [releases](https://github.com/Polprzewodnikowy/N64FlashcartMenu/releases/) page, then put it in the root directory of your SD card.

![SC64 flashcart information](./docs/images/sc64-flashcart-information.png "example SC64 flashcart information")


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
## libraries
* [libdragon](https://github.com/DragonMinded/libdragon/tree/preview) (UNLICENSE License)
* [libspng](https://github.com/randy408/libspng) (BSD 2-Clause License)
* [mini.c](https://github.com/univrsal/mini.c) (BSD 2-Clause License)
* [minimp3](https://github.com/lieff/minimp3) (CC0 1.0 Universal)
* [miniz](https://github.com/richgel999/miniz) (MIT License)

## Sounds
See [License](https://pixabay.com/en/service/license-summary/) for the following sounds:
* [Cursor sound](https://pixabay.com/en/sound-effects/click-buttons-ui-menu-sounds-effects-button-7-203601/) by Skyscraper_seven (Free to use)
* [Actions (Enter, Back) sound](https://pixabay.com/en/sound-effects/menu-button-user-interface-pack-190041/) by Liecio (Free to use)
* [Error sound](https://pixabay.com/en/sound-effects/error-call-to-attention-129258/) by Universfield (Free to use)
