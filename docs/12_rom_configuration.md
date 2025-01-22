[Return to the index](./00_index.md)
## ROM Configuration

The N64FlashcartMenu allows overriding the ROM's default configuration that is provided from the internal database.

The internal database is contained within `rom_info.c`.

The N64FlashcartMenu expects that you are using a flashcart that has an [UltraCIC](https://n64brew.dev/wiki/Checking_Integrated_Circuit) available.

NOTE: Some old ROM hacks may have adjusted the ROM code to manipulate the expected CIC and save type in order to allow compatibility with more available chips (usually 6102) as was used on flashcarts prior to 2018. If it does, you may need to override the internal database using the override settings.

If you override the defaults and want to go back to the default ones, delete the `<rom name>.ini` file.

### Homebrew Header
The N64FlashcartMenu fully supports the [homebrew header](https://n64brew.dev/wiki/ROM_Header#Advanced_Homebrew_ROM_Header)

### Available Overrides

#### CIC type
The Checking Integrated Circuit [CIC](https://n64brew.dev/wiki/Checking_Integrated_Circuit) was a physical security chip used by retail Nintendo 64 game cartridges that prevented unlicensed and pirated game cartridges from running and used in conjunction with the [PIF](https://n64brew.dev/wiki/PIF-NUS). 

WARNING: Changing the CIC type to an unsupported one may result in a blank screen for that particular ROM until you manually delete the override file!

For more detailed information regarding the various CIC chips, please visit [micro-64.com's game CIC database](http://micro-64.com/database/gamecic.shtml).

#### Save type
Games that have been programmed to include an internal save system might use various types of chips, methods and sizes.

WARNING: Using the wrong save type can cause unwanted behaviors on games and/or corrupt existing ones!

For more detailed information regarding the various saving methods, please visit [micro-64.com's game save database](http://micro-64.com/database/gamesave.shtml)

#### TV Region type
<!-- Should we quickly explain regions? -->
All ROMs are generally programmed to work with a single type of television output setting, whether it's NTSC, PAL or MPAL or their multiple variants. Forcing the region will generally make the ROM work on your display, however:

1. Be aware that not every CRT from the 1990s and 2000s is compatible with both NTSC and PAL standards.
2. Be aware that flat TVs from late 2010s-2020s might have other issues, such as when a game changes its internal resolution during gameplay (i.e. Resident Evil 2 with Expansion Pak).
3. Expect potential side effects:
   - Speed issues
   - Audio/visual desynchronization
   - Other unexpected/unwanted behaviors

### Autoload
See the [Autoload N64 ROMs](./22_autoload_roms.md) page.
