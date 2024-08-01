![Build](https://github.com/polprzewodnikowy/N64FlashcartMenu/actions/workflows/build.yml/badge.svg)

# N64 Flashcart Menu
An open source menu for N64 flashcarts.

## Supported Flashcarts

### Fully supported
* SummerCart64
* 64Drive

### Work in Progress
* ED64
* ED64P


## Current (notable) menu features
* Fully Open Source.
* Loads all known N64 games (including iQue and Aleck64 ROMs (even if they are byteswapped)).
* Fully emulates the 64DD and loads 64DD disks (SummerCart64 only).
* Emulator support (NES, SNES, GB, GBC) ROMs.
* N64 ROM box image support.
* Background image (PNG) support.
* Comprehensive ROM save database (including HomeBrew headers).
* Comprehensive ROM information display.
* Real Time Clock support.
* Music playback (MP3).


## Documentation
- [Getting started guide](docs/00_getting_started_sd.md#getting-started)
- [Menu controls](docs/01_menu_controls.md#menu-controls)
- [Developer guide](docs/99_developer_guide.md#developer-documentation)

## Video showcase (as of Oct 12 2023)

[![N64FlashcartMenu Showcase](http://img.youtube.com/vi/6CKImHTifDA/0.jpg)](http://www.youtube.com/watch?v=6CKImHTifDA "N64FlashcartMenu Showcase (Oct 12 2023)")


## Aims
* Support as many N64 FlashCarts as possible.
* Be open source, using permissive licensed third party licensed libraries.
* Be testable, using unit and smoke tests in ares emulated environment.
* Encourage active development from community members and N64 FlashCart owners.
* Support as many common mods and features as possible.


## Experimental features
These features are subject to change:

### ROM Boxart
To use boxart, you need to place png files of size 158x112 in the folder `/menu/boxart` on the SD card.
Each file must be named according to the 2 letter ROM ID, or 3 letter ROM ID including media type.
i.e. for GoldenEye 2 letters, this would be `GE.png`.
i.e. for GoldenEye 3 letters, this would be `NGE.png`.
A known set of PNG files using 2 letter ID's can be downloaded [here](https://mega.nz/file/6cNGwSqI#8X5ukb65n3YMlGaUtSOGXkKo9HxVnnMOgqn94Epcr7w).


### Menu Settings
The Menu creates a `config.ini` file in `sd:/menu/` which contains various settings that are used by the menu.
If required, you can manually adjust the file on the SD card using your computer.


## Flashcart specific

### SC64
- Ensure the cart has the latest [firmware](https://github.com/Polprzewodnikowy/SummerCart64/releases/latest) installed.
- Download the latest `sc64menu.n64` file from the releases page, then put it in the root directory of your SD card.


### 64drive
- Ensure the cart has the latest [firmware](https://64drive.retroactive.be/support.php) installed.
- Download the latest `menu.bin` file from the releases page, then put it in the root directory of your SD card.


### ED64 & ED64P
Currently not supported, but work is in progress (See [PR's](https://github.com/Polprzewodnikowy/N64FlashcartMenu/pulls)).

The aim is to replace [Altra64](https://github.com/networkfusion/altra64) and [ED64-UnofficialOS](https://github.com/n64-tools/ED64-UnofficialOS-binaries).


# Open source software and licenses used
 - [libdragon](https://github.com/DragonMinded/libdragon) (UNLICENSE License)
 - [libspng](https://github.com/randy408/libspng) (BSD 2-Clause License)
 - [mini.c](https://github.com/univrsal/mini.c) (BSD 2-Clause License)
 - [minimp3](https://github.com/lieff/minimp3) (CC0 1.0 Universal)
 - [miniz](https://github.com/richgel999/miniz) (MIT License)
