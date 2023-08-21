This folder allows support for the ED64(P) flashcart when using V series hardware.

Initially, this aims to work when loading the official ED64 OS menu, selecting the N64FlashcartMenu and then loading a ROM from there. This works around needing to load the correct firmware. To automate this, it is possible to set this menu to autoload (at least in the unofficial menu). i.e. You can set this ROM to auto load by placing a file called `autoexec.v64` in the `ED64` folder with the content of the file pointing to the path of this ROM.

Notes: 

* `libcart` seems to use a different base register address `0x08040000`, but official implementation uses `0xA8040000`
* When loading directly via `ED64\ED64.v64` The ability to load commercial ROM's is broken.

