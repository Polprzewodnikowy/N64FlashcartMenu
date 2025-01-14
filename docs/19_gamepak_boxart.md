[..](./00_index.md)
## Gamepak Boxart

### GamePak boxart images
To use N64 GamePak boxart images, place PNG files within the `sd:/menu/boxart/` folder.

#### Supported images
These must be `PNG` files that use the following dimensions:
* Standard N64 GamePak boxart sprites: 158x112
* Japanese N64 GamePak boxart sprites: 112x158
* 64DD boxart sprites: 129x112

They will be loaded by directories using each character (case-sensitive) of the full 4 character Game Code (as identified in the menu ROM information).
i.e. for GoldenEye NTSC USA (NGEE), this would be `sd:/menu/boxart/N/G/E/E/boxart_front.png`.
i.e. for GoldenEye PAL (NGEP), this would be `sd:/menu/boxart/N/G/E/P/boxart_front.png`.

To improve compatibility between regions (as a fallback), you may exclude the region ID (last matched directory) for GamePaks to match with 3 letter IDs instead:  
i.e. for GoldenEye, this would be `sd:/menu/boxart/N/G/E/boxart_front.png`.

**Warning**: Excluding the region ID may show the wrong boxart.  
**Note**: For future support, boxart sprites should also include:  
* `boxart_back.png`
* `boxart_top.png`
* `boxart_bottom.png`
* `boxart_left.png`
* `boxart_right.png`
* `gamepak_front.png`
* `gamepak_back.png`

As a starting point, here is a link to a boxart pack, that has `boxart_front.png`:  
* [third party boxart link](https://drive.google.com/file/d/1IpCmFqmGgGwKKmlRBxYObfFR9XywaC6n/view?usp=drive_link)
