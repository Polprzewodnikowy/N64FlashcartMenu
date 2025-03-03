[Return to the index](./00_index.md)
## Cheats (Gameshark, etc.)

The N64FlashcartMenu supports the cheat code types made popular by the peripherals:
- GameShark
- Action Replay

Another product by Blaze, called the Xploder64/Xplorer64 also existed in some regions, but these codes are less likely to work.

**WARNING**: It is not advised to connect a physical cheat cartridge in conjunction with most flashcarts.


The N64FlashcartMenu can only support cheat codes based on Datel carts when also using an Expansion Pak.

Caveats:
- Something about cheats and expansion paks.

The current code types are supported:
- 80 (description here)
- D0 (description here)
- Fx (description here)
- ...

The codes XX are not supported, because...
- e.g. they rely on the button.

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

And pass this array as a boot parameter: `menu->boot_params->cheat_list = cheats;`

Check the [Pull Requests](https://github.com/Polprzewodnikowy/N64FlashcartMenu/pulls) for work towards GUI editor support.
