## Experimental Features (Subject to change)

### Controller Paks
See: [Controller Paks](15_controller_paks.md)

### Cheats
See: [Cheats (Gameshark, etc.)](./13_datel_cheats.md)

### Rom information metadata
To show certain (metadata) information on the N64 ROM information screen, add a `.meta.txt` file in the same directory as the game ROM file with the same ROM filename (but excluding the ROM extension i.e. [`.z64`, `.v64`, `.n64`] )
Although the filename is `.meta.txt`, its content is based on an `ini` file.

The aim is to add full support for [https://n64brew.dev/wiki/ROM_Metadata](https://n64brew.dev/wiki/ROM_Metadata)

> [!TIP]
> A rom called `my N64 game rom.z64` will load Rom information metadata from `my N64 game rom.meta.txt`.

and the following content under the file content header:

> [!WARNING]
> The line endings must be `LF`.

```ini
[meta]
```
Examples of the "key=values" are:

#### ROM minimum age rating
The minimum age rating of the game.  

In future, this can be used for content filtering.  
Any number can be used and the menu will (currently) try to align it with the [ESRB](https://www.esrb.org/) rating;  
```ini
    age-rating=0    /**< No age rating defined */
    age-rating=1    /**< Everyone */
    age-rating=10   /**< Everyone 10+ */
    age-rating=13   /**< Teen */
    age-rating=17   /**< Mature */
    age-rating=18   /**< Adults Only */
```
an example for an age rating of "Everyone 10+" would be:
```ini
[meta]
; Self-certified age rating for the game. This is a number
; identifying the minimum suggested age for a player (0-18).
; This allows readers to provide some kind of content filtering
; capability. You can follow ESBR/PEGI guidelines to get
; some suggestions on this.
age-rating=10
```

#### ROM release date
The release date of the game.
This will not yet be shown in the menu.  
```ini
[meta]
; Release date of the game. Use YYYY-MM-DD format.
release-date=2025-11-04
```

#### ROM Short description
A short description with a length of 120 characters.
```ini
[meta]
; Short description of the game, to be displayed in
; menu or ROM managers. Suggested maximum length is
; 120 chars.
short-desc=Shoot your way to the heaven, fighting against birds and aliens
```

### Customizing the font
The N64FlashcartMenu allows the ability to test new fonts or adding regional characters without recompiling the menu. However the font is explicitly linked to the currently used version of the libdragon SDK.  
Add a `font64` file to the `sd:/menu/` directory called `custom.font64`.

You can build a font64 file with `Mkfont`, one of `libdragon`'s tools. At the time of writing, you will need to obtain `libdragon`'s [preview branch artifacts](https://github.com/DragonMinded/libdragon/actions/workflows/build-tool-windows.yml) to find out a copy of the prebuilt Windows executable. [Read its related Wiki page](https://github.com/DragonMinded/libdragon/wiki/Mkfont) for usage information.
