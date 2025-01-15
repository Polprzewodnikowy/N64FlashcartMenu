[Return to the index](./00_index.md)
## ROM Configuration

N64FlashcartMenu allows overriding the ROM's default configuration that is provided from the internal database.

The internal database is contained within `rom_info.c`.

If you override the defaults and want to go back to the default ones, delete the `<rom name>.ini` file.

### Available Overrides

#### CIC type
The CIC is a security chip used by Nintendo 64 games that is required for booting any game. WARNING: changing the CIC type could hang your N64!
For more detailed information regarding the various CIC chips, please visit [micro-64.com's game CIC database](http://micro-64.com/database/gamecic.shtml).

#### Save type
Games that have been programmed to include an internal save system might use various types of chips, methods and sizes. WARNING: using the wrong save type can cause unwanted behaviors on games and even hang your N64.
For more detailed information regarding the various saving methods, please visit [micro-64.com's game save database](http://micro-64.com/database/gamesave.shtml)

#### TV Region type
<!-- Should we quickly explain regions? -->
All ROMs are programmed to work with only one type of television output setting, whether it's NTSC, or PAL in its multiple variants.

Changing the TV Region type could make games work at your monitor's refresh rate, but it will also cause speed issues, audiovisual desynchronizations and might cause unwanted behaviors.

Make sure your monitor is compatible with the TV Region you want to force before changing the setting on N64FlashcartMenu. CRTs from the 1990s and 2000s won't always have support for both NTSC and PAL standards, and flat TVs from the 2020s might have other issues, such as when a game changes its internal resolution during gameplay (i.e. Resident Evil 2 with Expansion Pak).

### Autoload
See the [Autoload N64 ROMs](./22_autoload_roms.md) page.
