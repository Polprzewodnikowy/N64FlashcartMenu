[..](./00_index.md)
## Datel Cheats
AKA InterAct (in certain regions)

The N64 supports the peripherals
- GameShark
- Action Replay

It is not advised to connect the physical peripheral with most flashcarts.

The menu has underlying support for these codes (when using an expansion pak), but is not yet exposed via the menu graphical user interface.


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
    // Last 2 entrys must be 0
    0,
    0,
};
```

And pass this array as a boot parameter: `menu->boot_params->cheat_list = cheats;`

Check [Pull Requests](https://github.com/Polprzewodnikowy/N64FlashcartMenu/pulls) for work towards GUI support.

