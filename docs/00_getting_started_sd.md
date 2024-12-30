## Initial Setup of SD Card

### First Steps
Connect the SD card to your PC and ensure it is properly formatted to be compatible with your flashcart.  
**WARNING:** Filenames are expected to be written in ASCII. Unicode characters, such as accented characters or characters from languages such as Russian, Chinese, Japanese, Korean, etc., are not fully supported and may cause a crash screen.

#### Preparations for SC64
- FAT32 and EXFAT are fully supported.
- An SD formatted with 128 kiB cluster size is recommended.

- Download the latest `sc64menu.n64` file from the [releases](https://github.com/Polprzewodnikowy/N64FlashcartMenu/releases/) page, then put it in the root directory of your SD card.
- Create a folder in the root of your SD card called `menu`.
- Place your ROM files on the SD card, **in any folder except `menu`**.

#### Preparations for other supported flashcarts
- FAT32 recommended.
- An SD formatted with the default cluster size is recommended.

(TBW)


### Emulator Support
Emulators should be added to the `/menu/emulators` directory on the SD card.

N64FlashcartMenu currently supports the following emulators and associated ROM file names:
- **NES**: [neon64v2](https://github.com/hcs64/neon64v2/releases) by *hcs64* - `neon64bu.rom`
- **SNES**: [sodium64](https://github.com/Hydr8gon/sodium64/releases) by *Hydr8gon* - `sodium64.z64`
- **Game Boy**/**GB Color**: [gb64](https://lambertjamesd.github.io/gb64/romwrapper/romwrapper.html) by *lambertjamesd* - `gb.v64`/`gbc.v64` ("Download Emulator" button)
- **SMS**/**GG**: [smsPlus64](https://github.com/fhoedemakers/smsplus64/releases) by *fhoedmakers* - `smsPlus64.z64`
- **Fairchild Channel F**: [Press-F-Ultra](https://github.com/celerizer/Press-F-Ultra/releases) by *celerizer* - `Press-F.z64`


### 64DD Disk Support
In order to be able to load and run 64DD disk images, you need to place the required 64DD IPL dumps in the `/menu/64ddipl` folder on the SD card.
For more details, follow [this guide on the 64dd.org website](https://64dd.org/tutorial_sc64.html).


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
├── (some folder with roms)\
        │   └── (some folder with roms)\
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
├── (some folder with roms)\
    ├── a_rom.z64
    ├── b_rom_whatevs.n64
    └── saves\
        ├── a_rom.sav
        └── b_rom_whatevs.sav
```

### Transfering Saves From An ED64
If you are transferring a file from a different flashcart, such as the ED64, you must change the file extension to `sav`. 
For example, a save file called `Glover (USA).eep` should have its extension changed to `Glover (USA).sav` in order to work with N64FlashcartMenu.

You may also need to pad/trim the files to their original size:
- For EEPROM 4Kbit games, remove the padding.
- For others, use a tool such as [Ninjiteu's N64SaveConverter](https://github.com/Ninjiteu/N64SaveConverter).

**WARNING:** Saves created with certain emulators, or created for a different ROM version (as in revisions of the same game) 
or region (as in between NTSC and PAL versions of a same game) may be incompatible.
