[Return to the index](./00_index.md)
## Datel/InterAct Cheats

The N64 saw two cheat peripherals during its lifespan:
- GameShark
- Action Replay

WARNING: Do not connect a GameShark, Action Replay or similar cheat devices with most flashcarts.

N64FlashcartMenu has underlying support for these cheat codes (an Expansion Pak is required), but is not yet exposed via the graphical user interface.


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

Check the [Pull Requests section](https://github.com/Polprzewodnikowy/N64FlashcartMenu/pulls) for work towards GUI support.

