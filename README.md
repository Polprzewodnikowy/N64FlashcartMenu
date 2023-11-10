![Build](https://github.com/polprzewodnikowy/N64FlashcartMenu/actions/workflows/build.yml/badge.svg)

# N64 Flashcart Menu
An open source menu for N64 flashcarts.


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


### Video showcase (as of Oct 12 2023)

[![N64FlashcartMenu Showcase](http://img.youtube.com/vi/6CKImHTifDA/0.jpg)](http://www.youtube.com/watch?v=6CKImHTifDA "N64FlashcartMenu Showcase (Oct 12 2023)")


## Aims
* Support as many N64 FlashCarts as possible.
* Be open source, using permissive licensed third party licensed libraries.
* Be testable, using unit and smoke tests in ares emulated environment.
* Encourage active development from community members and N64 FlashCart owners.
* Support as many common mods and features as possible.


## Getting started
Using your PC, insert the SD card and ensure it is formatted for compatibility (We recommend FAT32 in most instances).


### ROM Boxart
To use boxart, you need to place png files of size 158x112 in the folder `/menu/boxart` on the SD card.
Each file must be named according to the 2 letter ROM ID, or 3 letter ROM ID including media type.
i.e. for GoldenEye 2 letters, this would be `GE.png`.
i.e. for GoldenEye 3 letters, this would be `NGE.png`.
A known set of PNG files using 2 letter ID's can be downloaded [here](https://mega.nz/file/6cNGwSqI#8X5ukb65n3YMlGaUtSOGXkKo9HxVnnMOgqn94Epcr7w).


### Emulator support
Emulators should be added to the `/menu/emulators` directory on the SD card.

Menu currently supports the following emulators and associated ROM file names:
 - **NES**: [neon64v2](https://github.com/hcs64/neon64v2) by *hcs64* - `neon64bu.rom`
 - **SNES**: [sodium64](https://github.com/Hydr8gon/sodium64) by *Hydr8gon* - `sodium64.z64`
 - **Game Boy** / **GB Color**: [gb64](https://lambertjamesd.github.io/gb64/romwrapper/romwrapper.html) by *lambertjamesd* - `gb.v64` / `gbc.v64`
 - **Sega Master System** / **Sega Game Gear** / **Sg1000**: [TotalSMS](https://github.com/ITotalJustice/TotalSMS) - `TotalSMS.z64` (Currently broken)


### SC64 Specific
- Ensure the cart has the latest [firmware](https://github.com/Polprzewodnikowy/SummerCart64/releases/latest) installed.
- Download the latest `sc64menu.n64` file from the releases page, then put it in the root directory of your SD card.

##### 64DD disk support
For the ability to load and run 64DD disk images, you need to add the folder `/menu/64ddipl` on the SD card.
Download and add the relevant ipl files and rename them before adding them to the folder:
- `NDDE0.n64` the US Prototype IPL can be downloaded from [here](https://64dd.org/dumps/64DD_IPL_US_MJR.n64)
- `NDXJ0.n64` the JPN Development IPL can be downloaded from [here](https://64dd.org/dumps/64DD_IPL_DEV_H4G.n64)
- `NDDJ2.n64` the JPN Retail IPL can be downloaded from [here](https://64dd.org/dumps/N64DD_IPLROM_(J).zip)

Note: to load an expansion disk (e.g. F-Zero X) browse to the N64 ROM and load it (but not start it) and then browse to the DD expansion file and press the `R` button.


### 64drive Specific
- Ensure the cart has the latest [firmware](https://64drive.retroactive.be/support.php) installed.
- Download the latest `menu.bin` file from the releases page, then put it in the root directory of your SD card.


### ED64 & ED64P Specific
#### ED64
Download the `ED64.v64` ROM from the latest action run assets and place it in the `/ED64` folder.

**Untested** The aim is to replace [ED64-UnofficialOS](https://github.com/n64-tools/ED64-UnofficialOS-binaries).

#### ED64P
Download the `ED64P.v64` ROM from the latest action run assets and place it in the `/ED64P` folder.


# Developer documentation

You can use a dev container in VSCode to ease development.


## To deploy:
### SC64
* Download the deployer [here](https://github.com/Polprzewodnikowy/SummerCart64/releases/download/v2.17.0/sc64-deployer-windows-v2.17.0.zip)
* Extract and place `sc64deployer.exe` in the `tools/sc64` directory.

Make sure that your firmware is compatible (currently v2.17.0+)
See: [here](https://github.com/Polprzewodnikowy/SummerCart64/blob/v2.17.0/docs/00_quick_startup_guide.md#firmware-backupupdate)


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


### Others
* Add the required file to the correct folder on your SD card.


# Update Libdragon submodule
This repo currently uses the `unstable` branch as a submodule at a specific commit.
To update to the latest version, use `git submodule update --remote ` from the terminal.

# Generate documentation
Run `doxygen` from the dev container terminal.
Make sure you fix the warnings before creating a PR!
Generated documentation is located in `output/docs` folder and auto published to the `gh-pages` branch when merged with `main`.

Once merged, they can be viewed [here](https://polprzewodnikowy.github.io/N64FlashcartMenu/)

# Open source software and licenses used
 - [libdragon](https://github.com/DragonMinded/libdragon) (UNLICENSE License)
 - [libspng](https://github.com/randy408/libspng) (BSD 2-Clause License)
 - [mini.c](https://github.com/univrsal/mini.c) (BSD 2-Clause License)
 - [minimp3](https://github.com/lieff/minimp3) (CC0 1.0 Universal)
 - [miniz](https://github.com/richgel999/miniz) (MIT License)
