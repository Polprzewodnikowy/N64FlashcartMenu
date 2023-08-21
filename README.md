# N64 Flashcart Menu
An open source menu for N64 flashcarts.

## Aims
* Support as many N64 FlashCarts as possible.
* Be open source, using permissive licensed third party licensed libraries.
* Be testable, using unit and smoke tests in ares emulated environment.
* Encourage active development from community members and N64 FlashCart owners.
* Support as many common mods and features as possible.


## Current (notable) menu features
* Fully Open Source.
* Loads all known N64 games (including iQue and byteswapped ROMs).
* Emulator support (NES, SNES, GB, GBC) ROMs.
* N64 ROM box image support.
* Background image (PNG) support.
* Comprehensive ROM save database (including HomeBrew headers).
* Comprehensive ROM information display.
* Real Time Clock support.
* Music playback (MP3).


## Getting started

### SC64
Ensure the cart is running the latest [firmware](https://github.com/Polprzewodnikowy/SummerCart64/releases/latest).
Download the `sc64menu.n64` ROM from the latest action run assets.
Add it to the root folder on your SD card.

### ED64 & ED64P
Currently not supported, but there is an aim to do so.
The aim is to replace [Altra64](https://github.com/networkfusion/altra64) and [ED64-UnofficialOS](https://github.com/n64-tools/ED64-UnofficialOS-binaries).


### Common to all

#### ROM Boxart
To use boxart, you need to place png files of size 158x112 in the folder `/menu/boxart` on the SD card.
Each file must be named according to the 2 letter ROM ID. e.g. for goldeneye, this would be `GE.png`.
A known set of PNG files can be downloaded [here](https://mega.nz/file/6cNGwSqI#8X5ukb65n3YMlGaUtSOGXkKo9HxVnnMOgqn94Epcr7w).

#### Emulator support
Emulators should be added to the `/emulators` directory on the SD card.

Menu currently supports the following emulators and associated ROM file names:
 - **NES**: [neon64v2](https://github.com/hcs64/neon64v2) by *hcs64* - `neon64bu.rom`
 - **SNES**: [sodium64](https://github.com/Hydr8gon/sodium64) by *Hydr8gon* - `sodium64.z64`
 - **Game Boy** / **GB Color**: [gb64](https://lambertjamesd.github.io/gb64/romwrapper/romwrapper.html) by *lambertjamesd* - `gb.v64` / `gbc.v64`


# Developer documentation
**Work in progress!**

You can use a dev container in VSCode to ease development.


## To deploy:
### SC64
* Download the deployer [here](https://github.com/Polprzewodnikowy/SummerCart64/releases/download/v2.16.0/sc64-deployer-windows-v2.16.0.zip)
* Extract and place `sc64deployer.exe` in the `tools/sc64` directory.

Make sure that your firmware is compatible (currently v2.16.0+)
See: https://github.com/Polprzewodnikowy/SummerCart64/blob/v2.16.0/docs/00_quick_startup_guide.md#firmware-backupupdate


#### From the devcontainer
It is not currently possible to directly communicate with USB devices.
BUT, as a work around you can use a proxy TCP/IP connection
Set up a proxy: open a terminal window, `cd ./tools/sc64` and then `./sc64deployer.exe server`

Then in the dev container, use `make run` or `make run-debug`


#### From your host (Windows) OS

* Run `./localdeploy.bat` from the terminal


Toggle the N64 power switch to load the ROM.

`ms-vscode.makefile-tools` will help (installed automatically in dev container).
TODO: it does not yet work with `F5`: see https://devblogs.microsoft.com/cppblog/now-announcing-makefile-support-in-visual-studio-code/
WORKAROUND: in the dev container terminal, use make directly, i.e.: `make`
The ROM can be found in the `output` directory.

NOTE: a "release" version of the SC64 menu is called `sc64menu.n64` and can be created for when you want to add it directly to the SDCard. This is generated by running `make all` or running `make sc64`.

# Update Libdragon submodule
This repo currently uses the `unstable` branch as a submodule at a specific commit.
To update to the latest version, use `git submodule update --remote ` from the terminal.

# Generate documentation
Run `doxygen` from the dev container terminal.
Make sure you fix the warnings before creating a PR!
Generated documentation is located in `output/docs` folder.

# Open source software and licenses used
 - [libdragon](https://github.com/DragonMinded/libdragon) (UNLICENSE License)
 - [libspng](https://github.com/randy408/libspng) (BSD 2-Clause License)
 - [mini.c](https://github.com/univrsal/mini.c) (BSD 2-Clause License)
 - [minimp3](https://github.com/lieff/minimp3) (CC0 1.0 Universal)
 - [miniz](https://github.com/richgel999/miniz) (MIT License)
