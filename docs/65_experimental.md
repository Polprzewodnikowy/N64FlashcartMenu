[Return to the index](./00_index.md)
## Experimental Features (Subject to change)

### ROM info descriptions (pre-release only)
To show a ROM description in the N64 ROM information screen, add a `.ini` file next to the game ROM file with the same name and the following content:
```ini
[metadata]
description=This is the ROM description that does X Y Z.
```
Text files must use ASCII characters only, Linux `LF` endings (CRLF is not supported) and the descriptions themselves must be limited to 300 characters.

### Customizing the font
The N64FlashcartMenu allows the ability to test new fonts or adding regional characters without recompiling the menu. However the font is explicitly linked to the currently used version of the libdragon SDK.  
Add a `font64` file to the `sd:/menu/` directory called `custom.font64`.

<!-- Would be best if we can just link to an actual copy of Mkfont executable file and not expect people to compile libdragon -->
You can build a font64 file with `Mkfont`, one of `libdragon`'s tools. At the time of writing, you will need to obtain `libdragon`'s [preview branch artifacts](https://github.com/DragonMinded/libdragon/actions/workflows/build-tool-windows.yml) to find out a copy of the prebuilt Windows executable. [Read its related Wiki page](https://github.com/DragonMinded/libdragon/wiki/Mkfont) for usage information.

