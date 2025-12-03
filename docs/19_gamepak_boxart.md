[Return to the index](./00_index.md)
## Game Art Images
To use N64 game art images, place your PNG files within the `sd:/menu/metadata/` folder.

> [!WARNING]
> Older menu versions may not show boxart anymore. Game boxart is now stored in `sd:/menu/metadata/`, rather than the previous `sd:/menu/boxart/` folder. Please update your folder name and structure/content to ensure compatibility.

> [!TIP]
> As a starting point, here is a link to the latest, menu metadata: [Metadata link](https://github.com/n64-tools/n64-flashcart-menu-metadata/releases), which requires regular and independent updates.

#### Supported file names
- `boxart_front.png` (DEFAULT)
- `boxart_back.png`
- `boxart_top.png`
- `boxart_bottom.png`
- `boxart_left.png`
- `boxart_right.png`
- `gamepak_front.png`
- `gamepak_back.png`

#### Supported image sizes
Files must be in `PNG` format and use the following dimensions:
* American/European N64 boxart sprites: 158x112
* Japanese N64 boxart sprites: 112x158
* 64DD boxart sprites: 129x112
* GamePak front and back sprites: 158x112
* Boxart top/bottom spines: 158x22
* Boxart left/right spines: 112x22

Images will be loaded by directories using each character (case-sensitive) of the full 4-character Game Code (as identified in the menu ROM information):  
i.e. for GoldenEye NTSC USA (NGEE), this would be `sd:/menu/metadata/N/G/E/E/boxart_front.png`.  
i.e. for GoldenEye PAL (NGEP), this would be `sd:/menu/metadata/N/G/E/P/boxart_front.png`.

To improve compatibility between regions (as a fallback), you may exclude the region ID (last matched directory) for GamePaks to match with 3-character IDs instead:  
i.e. for GoldenEye, this would be `sd:/menu/metadata/N/G/E/boxart_front.png`.

ROMs that use the ["Advanced Homebrew ROM Header"](https://n64brew.dev/wiki/ROM_Header) Game ID (`xEDx`) are matched using the "Game Title" (not the Game Code):
e.g. `sd:/menu/metadata/homebrew/{game title}/boxart_front.png`

**Warning**: Excluding the region ID may show a box art of the wrong region.

#### Itterating through game art images
When added, extra images will be used if available to itterate:
- `boxart_back.png`
- `boxart_top.png`
- `boxart_bottom.png`
- `boxart_left.png`
- `boxart_right.png`
- `gamepak_front.png`
- `gamepak_back.png`

On the ROM load screen, you can cycle through available images using the **D-pad left**/**C-pad left** and **D-pad right**/**C-pad right** buttons. The menu will display any available images from the list above, skipping images that don't exist.
