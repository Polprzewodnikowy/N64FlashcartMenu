This folder allows support for the ED64 "Official" flashcart when using V series hardware.
For the moment it is unlikely to support clones.

It is based on OS 2.12.x as used in the "Unofficial Menu" (https://github.com/n64-tools/ED64-UnofficialOS-binaries/releases/tag/V2.12.9.3-Release)

It may require further cleansing and updating to use the latest libdragon before it will probably work based on the fact that using the latest libdragon with `libcart` seems to have a detremental effect on the ability to load ROM's.


Initially, this aims to work when loading the ED64 menu, selecting the N64FlashcartMenu and then loading a ROM from there. This works around needing to load the correct firmware. To automate this, it is possible to set the menu to autoload (at least in the unofficial menu). e.g. You can set this ROM to auto load by placing a file called `autoexec.v64` in the `ED64` folder with the content of the file pointing to the path of this ROM.

Notes: 

* `libcart` seems to use a different base register address `0x08040000`, but official implementation uses `0xA8040000`
* The ability to load/save `FLASHRAM` may not yet supported.

