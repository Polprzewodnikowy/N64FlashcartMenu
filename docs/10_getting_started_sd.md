[Return to the index](./00_index.md)
## Initial Setup of an SD Card

### First steps
Connect the SD card to your PC and ensure it is properly formatted to be compatible with your flashcart.  
**WARNING:** Filenames are expected to be written in ASCII, with Western Europe characters fully compatible. Other Unicode characters, such as those from Eastern Europe, Russia, Asia or Middle East regions (to name just a few examples) are not fully supported and may not be displayed.

**Note:** It is advised to use ROM files in the Big Endian (default, also called "non-byteswapped") format. Although the menu auto-converts byteswapped ROM files, the load time will increase.

#### Preparations for SC64
- FAT32 and EXFAT are fully supported.
- An SD formatted with 128 kiB cluster size is recommended.
- Download the latest `sc64menu.n64` file from the [releases](https://github.com/Polprzewodnikowy/N64FlashcartMenu/releases/) page, then put it in the root directory of your SD card.
- Place your ROM files on the SD card, **in any folder except `menu`**.
**NOTE:** byteswapped ROM's will increase load times.

#### Preparations for other supported flashcarts
- FAT32 recommended.
- An SD formatted with the default cluster size is recommended.
- Download the latest [menu](https://github.com/Polprzewodnikowy/N64FlashcartMenu/releases/) file specific for your flashcart and place it in the expected location.

### Emulator support
See the [Emulators](./18_emulators.md) page.

### 64DD Disk support
See the [64DD](./17_64dd.md) page.

#### So what would the layout of the SD card look like?
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
│        ├── gbc.v64
│        ├── smsPlus64.z64
│        └── Press-F.z64
│
├── (a rom).z64
├── (a rom).n64
├── (some folder with ROMs)\
        │   └── (some folder with ROMs)\
        |       └── (some supported ROM files)
        │
        ├── (some supported ROM files)
        |
        └── (some folder with 64DD disk images)\
            └── (some 64DD disk images)
```


## Save Files
All save files (whether `FlashRam`, `SRAM` or `EEPROM`) will be read from a `/saves` folder located **in the same directory as the ROM** 
and they must share the same file name, but use the `.sav` extension. `.sav` files will be created and modified whenever a ROM file saves to 
the "cartridge save memory".

```plaintext
├── (some folder with ROMs)\
    ├── a_rom.z64
    ├── b_rom_whatever.n64
    └── saves\
        ├── a_rom.sav
        └── b_rom_whatever.sav
```

### Transferring saves from an ED64
If you are transferring a file from a different flashcart, such as the ED64, you must change the file extension to `sav`. 
For example, a save file called `Glover (USA).eep` should have its extension changed to `Glover (USA).sav` to work with N64FlashcartMenu.

You may also need to pad/trim the files to their original size:
- For EEPROM 4Kbit games, remove the padding.
- For others, use a tool such as [Ninjiteu's N64SaveConverter](https://github.com/Ninjiteu/N64SaveConverter).

**WARNING:** Saves created with certain emulators, or created for a different ROM version (as in revisions of the same game) 
or region (as in between NTSC and PAL versions of a same game) may be incompatible.
