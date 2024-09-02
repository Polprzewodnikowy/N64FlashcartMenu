## First time setup of SD card

Using your PC, insert the SD card and ensure it is formatted for compatibility with your flashcart (*FAT32 and EXFAT are fully supported on the SC64*).

- Download the latest `sc64menu.n64` (assuming you are using an *sc64*) file from the [releases](https://github.com/Polprzewodnikowy/N64FlashcartMenu/releases/) page, then put it in the root directory of your SD card.
- Create a folder in the root of your SD card called `menu`.
- Place your ROMs on the SD Card, in any folder (**except for `menu`**).


### Emulator support
Emulators should be added to the `/menu/emulators` directory on the SD card.

Menu currently supports the following emulators and associated ROM file names:
- **NES**: [neon64v2](https://github.com/hcs64/neon64v2/releases) by *hcs64* - `neon64bu.rom`
- **SNES**: [sodium64](https://github.com/Hydr8gon/sodium64/releases) by *Hydr8gon* - `sodium64.z64`
- **Game Boy** / **GB Color**: [gb64](https://lambertjamesd.github.io/gb64/romwrapper/romwrapper.html) by *lambertjamesd* - `gb.v64` / `gbc.v64` ("Download Emulator" button)


### 64DD disk support
For the ability to load and run 64DD disk images, you need to place the required 64DD IPL dumps in the `/menu/64ddipl` folder on the SD card.
For more details, follow [this guide on the 64dd.org website](https://64dd.org/tutorial_sc64.html).


#### So what would the layout of the SD Card look like?
```plaintext
SD:\
│
├── sc64menu.n64
│
│
├── menu\
│   │
│   │
│   ├── 64ddipl\
│   │    ├── NDDE0.n64
│   │    ├── NDDJ2.n64
│   │    └── NDXJ0.n64
│   │    
│   └── emulators\
│        ├── neon64bu.rom
│        ├── sodium64.z64
│        ├── gb.v64
│        └── gbc.v64
│
├── (a rom).z64
├── (a rom).n64
├── (some folder with roms)\
        │   └── (some folder with roms)\
        |       └── (Some supported ROM files)
        │
        ├── (Some supported ROM files)
        |
        └── (Some folder with 64DD disk images)\
            └── (Some 64DD disk images)
```


## Save Files
All save files (whether `FlashRam`, `SRAM` or `EEPROM`) use the `.sav` extension and match the filename of a ROM.

Each save file can be found in the `/saves` folder located in the **same directory** as the ROM and shares the same file name, apart from the extension.
These files are created and modified when a "game" saves.

```plaintext
├── (some folder with roms)\
    ├── a_rom.z64
    ├── b_rom.n64
    └── saves\
        ├── a_rom.sav
        └── b_rom.sav
```

### Transfering saves from an ED64
If transferring a file from a different flashcart, such as the ED64, it will be necessary to change the extension of the file to `sav`.

i.e. for `Glover (USA).eep` you would need to change the extension to `Glover (USA).sav`

You may also need to pad/trim the files to their original size:
- For EEPROM 4Kbit games, remove the padding.
- For others, use a tool such as Ninjiteu's N64 Save converter.

**NOTE:** certain emulator saves or saves created for a different ROM version or region may be incompatible.
