# N64 Flashcart Menu
**This repo and its documentation is currently experimental and subject to change without notice.**

## Aims
* Support as many N64 FlashCarts as possible.
* Be open source, using permissive licensed third party licensed libraries.
* Be testable, using unit and smoke tests in ares emulated environment.
* Encourage active development from community members and N64 FlashCart owners.
* Support as many common mods and features as possible.


## Getting started

### SC64
Download the `sc64menu.n64` ROM from the latest action run assets.
Add it to the root folder on your SD card.

### ED64
Currently not supported, but there is an aim to do so.
The aim is to replace [Altra64](https://github.com/networkfusion/altra64) and [ED64-UnofficialOS](https://github.com/n64-tools/ED64-UnofficialOS-binaries).


### Common to all
#### ROM Boxart
To use boxart, you need to place png files of size 158x112 in the folder `sd://menu/boxart/`
Each file must be named according to the 2 letter ROM ID. e.g. for goldeneye, this would be `GE.png`
A known set of PNG files can be downloaded from https://mega.nz/file/6cNGwSqI#8X5ukb65n3YMlGaUtSOGXkKo9HxVnnMOgqn94Epcr7w


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

Then in the dev container, use `./remotedeploy.sh`


#### From your host (Windows) OS

* Run `./localdeploy.bat` from the terminal


Toggle the N64 power switch to load the ROM.

`ms-vscode.makefile-tools` will help (installed automatically in dev container).
TODO: it does not yet work with `F5`: see https://devblogs.microsoft.com/cppblog/now-announcing-makefile-support-in-visual-studio-code/
WORKAROUND: in the dev container terminal, use make directly, i.e.: `make`
The ROM can be found in the `output` directory.

NOTE: a "release" version of the SC64 menu is called `sc64menu.n64` and can be created for when you want to add it directly to the SDCard. This is generated by running `make all` or running `make sc64_minify`.

# Update Libdragon submodule
This repo currently uses the `unstable` branch as a submodule at a specific commit.
To update to the latest version, use `git submodule update --remote ` from the terminal.

# Generate documentation
Run `doxygen` from the dev container terminal.
Make sure you fix the warnings before creating a PR!


# OSS licenses used for libraries
* UNLICENSE (libdragon)
* BSD 2-Clause (libspng)
* CC0 1.0 Universal (minimp3)
* Permissive, unspecific (miniz)
