## Experimental Features (Subject to change)

### Controller Paks
See: [Controller Paks](15_controller_paks.md)

### Cheats
See: [Cheats (Gameshark, etc.)](./13_datel_cheats.md)



### Customizing the font
The N64FlashcartMenu allows the ability to test new fonts or adding regional characters without recompiling the menu. However the font is explicitly linked to the currently used version of the libdragon SDK.  
Add a `font64` file to the `sd:/menu/` directory called `custom.font64`.

You can build a font64 file with `Mkfont`, one of `libdragon`'s tools. At the time of writing, you will need to obtain `libdragon`'s [preview branch artifacts](https://github.com/DragonMinded/libdragon/actions/workflows/build-tool-windows.yml) to find out a copy of the prebuilt Windows executable. [Read its related Wiki page](https://github.com/DragonMinded/libdragon/wiki/Mkfont) for usage information.

### Customise the BGM
Use the libdragon `AUDIOCONV` tool to convert a wav to wav64 with compression level 1 and save to `sd:/menu/custom_bgm.wav64`
