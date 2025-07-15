[Return to the index](./00_index.md)
## Cheats (Gameshark, etc.)

The N64FlashcartMenu has underlying support for the cheat code types made popular by the peripherals:
- GameShark
- Action Replay

Another product by Blaze, called the Xploder64/Xplorer64 also existed in some regions, but these codes are less likely to work.

> [!WARNING]
> It is not advised to connect a physical cheat cartridge in conjunction with most flashcarts.

> [!CAUTION]
> The N64FlashcartMenu can only support cheat codes based on Datel carts when **also** using an Expansion Pak.

Caveats:
- Requires an Expansion Pak.
- Codes that require a button to pressed are not yet supported.

Certain codes are not yet supported, because, for instance:
- they rely on the button.

> [!TIP]
> a good reference for generating codes is: https://gamehacking.org

```
// Example cheat codes for the game "Majoras Mask USA"
uint32_t cheats[] = {
    // Enable code
    0xF1096820,
    0x2400,
    0xFF000220,
    0x0000,
    // Inventory Editor (assigned to L)
    0xD01F9B91,
    0x0020,
    0x803FDA3F,
    0x0002,
    // Last 2 entries must be 0
    0,
    0,
};
```

Currently it is necessary to load the `Datel Code Editor` whilst in the ROM info screen AND then press `A` to `Apply ROM with these cheats` AND then re-select the ROM from the broswer and press `R` to `use cheats` whilst in the ROM info screen, before finally running the ROM by pressing `A`.
Unfortunatly this currently results in a blank screen (triage in progress).

This array is parsed as a boot parameter: `menu->boot_params->cheat_list = cheats;`
