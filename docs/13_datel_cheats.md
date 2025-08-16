[Return to the index](./00_index.md)
## Cheats (Gameshark, etc.)

> [!WARNING]
> **THIS FEATURE IS EXPERIMENTAL**

The N64FlashcartMenu has underlying support for the cheat code types made popular by the peripherals:
- GameShark
- Action Replay

Another product by Blaze, called the Xploder64/Xplorer64 also existed in some regions, but these codes are unlikely to work (yet).

> [!CAUTION]
> It is not advised to connect a physical cheat cartridge in conjunction with most flashcarts.

> [!WARNING]
> The N64FlashcartMenu can only support cheat codes based on Datel carts when **also** using an Expansion Pak. Codes will not be loaded when the Expansion Pak is not found.

Caveats:
- Requires an Expansion Pak to work.
- Codes that require a button to pressed are not yet supported (TODO: possible with the SC64, requires hook).
- `DE` enable codes for dealing with CICs that change the entrypoint are not yet supported.
- Games that were developed using the libDragon SDK are not yet supported.
- The maximum number of cheat codes that will work is dependent on the available space in the N64 RAM (especially with game ROM's that use an Expansion Pak), exceeding them may cause unwanted side effects.
 - Whilst an experimental feature, the UI is currently hardcoded to limit the amount of codes to 34 (keeping it low allows us to collect caveats). This will be increased once enough feedback is collected.
- The cheat engine is subtly different from an actual GameShark and ED64 implementation so working enable and cheat codes may vary.

### Enable Cheats
Ensure you have a cheat file and have accessed the Datel Code Editor screen, then from the ROM info screen, press `R` to set `Use Cheats` to `Enabled`.

#### Datel Code Editor
When within the ROM info screen, Press `R` and load the `Datel Code Editor`. Adjust the cheat codes as needed then press `A` to `Apply ROM with these cheats`.

> [!WARNING]
> Any changes made in the code editor are totally volitile (i.e. not saved for when returning to the current screen).


#### Datel Cheat Files

> [!TIP]
> A good reference for cheat codes is: https://gamehacking.org and downloading ED64 text files, though note that usage may vary, as these may have been "corrupted" by the ability to use them in an in-accurate emulator.

Cheat files should be contained in the same directory as the ROM, but with the file extension `datel.txt` (instead of the ROM extension).
The files use the following content format (as aligned to text files made popular by the ED64):
(one cheat per line).
- An 8 character address, followed by a space, and then a 4 character value.
- An optional description can be added after the value (with a preceeding space).

> [!TIP]
> All codes in the file will be presumed active by default. Use the Datel Code Editor to disable them.

> [!WARNING]
> Incorrectly formatted files may cause unknown side effects.

e.g. For Majoras Mask (USA):
```
F1096820 2400 Activator
FF000220 0000
811F05AA FFFF Complete Bomber's Notebook
811F05AC FFFF
811F05AE FFFF
811F05B0 FFFF
810F6C3C 2400 Disable 3-day Timer
811EF6B6 FFFF Enable All Owl Statues
810A6A0E 6A40 Get Items From Anywhere
```

e.g. For Super Mario 64:
```
8033B21D 0064 Super mario 64 infinite lives
80207723 0001 120 stars part 1
8020770B 00C7 120 stars part 2
50001101 0000 120 stars part 3
8020770C 00FF 120 stars part 4
```


### Current community reported issues
These issues have been reported as differences when comparing a real GS device (or proved working on an ED64):
 - Exceeding 12 cheats in Majoras Mask will cause issues (Find better Enable Code?!).
 - Exceeding 15 cheats in DK64 will cause issues (Find better Enable Code?!).
 - Paper Mario cheats do not work.
 - WWF No Mercy (Rev 1) hangs at a black screen.
 - Xeno Crises cheats do not work, unless you increase each address by 0x01 (see https://github.com/Polprzewodnikowy/N64FlashcartMenu/pull/256#discussion_r2260752613).
 - F-Zero X (with and without the 64DD Expansion Kit) cheats do not work.
 - AeroGauge (EUR) no damage hangs the game on impact.
