[Return to the index](./00_index.md)
## Experimental Features (Subject to change)

### Cheats
See: [Cheats (Gameshark, etc.)](./13_datel_cheats.md)

### Rom information metadata
To show certain (metadata) information on the N64 ROM information screen, add a `.ini` file in the same directory as the game ROM file with the same ROM filename (but excluding the ROM extension i.e. [`.z64`, `.v64`, `.n64`] )

> [!TIP]
> A rom called `my N64 game rom.z64` will load Rom information metadata from `my N64 game rom.ini`.

and the following content under the file content header:

```ini
[metadata]
```
Examples of the "key=values" are:

#### ROM age rating
The age rating of the original ROM as matched to the [ESRB](https://www.esrb.org/) rating (at the time of the game release). 
```
    esrb_age_rating=0   /**< No age rating defined */
    esrb_age_rating=1   /**< Everyone */
    esrb_age_rating=2   /**< Everyone 10+ */
    esrb_age_rating=3   /**< Teen */
    esrb_age_rating=4   /**< Mature */
    esrb_age_rating=5   /**< Adults Only */
```
an example for an age rating of "Everyone 10+" would be:
```ini
[metadata]
esrb_age_rating=2
```

Note: [PEGI](https://pegi.info/) support may be added as an option at a later date.
In future, this can be of use for content filtering.


### Customizing the font
The N64FlashcartMenu allows the ability to test new fonts or adding regional characters without recompiling the menu. However the font is explicitly linked to the currently used version of the libdragon SDK.  
Add a `font64` file to the `sd:/menu/` directory called `custom.font64`.

You can build a font64 file with `Mkfont`, one of `libdragon`'s tools. At the time of writing, you will need to obtain `libdragon`'s [preview branch artifacts](https://github.com/DragonMinded/libdragon/actions/workflows/build-tool-windows.yml) to find out a copy of the prebuilt Windows executable. [Read its related Wiki page](https://github.com/DragonMinded/libdragon/wiki/Mkfont) for usage information.

