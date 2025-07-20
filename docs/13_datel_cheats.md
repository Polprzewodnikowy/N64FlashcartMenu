[Return to the index](./00_index.md)
## Cheats (Gameshark, etc.)

> [!WARNING]
> THIS FEATURE IS EXPERIMENTAL

The N64FlashcartMenu has underlying support for the cheat code types made popular by the peripherals:
- GameShark
- Action Replay

Another product by Blaze, called the Xploder64/Xplorer64 also existed in some regions, but these codes are unlikely to work.

> [!WARNING]
> It is not advised to connect a physical cheat cartridge in conjunction with most flashcarts.

> [!CAUTION]
> The N64FlashcartMenu can only support cheat codes based on Datel carts when **also** using an Expansion Pak. Codes will not be loaded when the Expansion Pak is not found.

Caveats:
- Requires an Expansion Pak.
- Codes that require a button to pressed are not yet supported.

Certain codes are not yet supported, because, for instance:
- they rely on the button.

### Enable Cheats
Ensure you have a cheat file and/or have used the Datel Code Editor, then from the ROM info screen, press `R` to set `Use Cheats` to `Enabled`.

#### Datel Code Editor
When within the ROM info screen, Press `R` and load the `Datel Code Editor`. Adjust the cheat codes as needed then press `A` to `Apply ROM with these cheats`.

> [!CAUTION]
> The Code editor does not work properly when using ROM's loaded from the favorites or history tabs (yet).


#### Datel Cheat Files

> [!TIP]
> a good reference for generating codes is: https://gamehacking.org and downloading ED64 text files.

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
