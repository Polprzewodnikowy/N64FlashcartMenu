[Return to the index](./00_index.md)
## Game Art Images
To use N64 game box art images, place your PNG files within the `sd:/menu/boxart/` folder.

#### Supported images
Files must be in `PNG` format and use the following dimensions:
* American/European N64 box art sprites: 158x112
* Japanese N64 box art sprites: 112x158
* 64DD box art sprites: 129x112

Images will be loaded by directories using each character (case-sensitive) of the full 4-character Game Code (as identified in the menu ROM information):  
i.e. for GoldenEye NTSC USA (NGEE), this would be `sd:/menu/boxart/N/G/E/E/boxart_front.png`.  
i.e. for GoldenEye PAL (NGEP), this would be `sd:/menu/boxart/N/G/E/P/boxart_front.png`.

To improve compatibility between regions (as a fallback), you may exclude the region ID (last matched directory) for GamePaks to match with 3-character IDs instead:  
i.e. for GoldenEye, this would be `sd:/menu/boxart/N/G/E/boxart_front.png`.

**Warning**: Excluding the region ID may show a box art of the wrong region.  
**Note**: For future support, box art sprites should also include:  
- `boxart_back.png`
- `boxart_top.png`
- `boxart_bottom.png`
- `boxart_left.png`
- `boxart_right.png`
- `gamepak_front.png`
- `gamepak_back.png`

As a starting point, here is a link to a box art pack, that has `boxart_front.png`:  
- [Third party box art](https://drive.google.com/file/d/1IpCmFqmGgGwKKmlRBxYObfFR9XywaC6n/view?usp=drive_link)
