[..](./00_index.md)
## Datel Cheats
AKA InterAct (in certain regions)

The N64 supports the peripherals
- GameShark
- Action Replay

Another product by Blaze, called the Xploder64/Xplorer64 also existed in some regions, but these codes are less likely to work.

It is not advised to connect the physical peripheral with most flashcarts.


The menu has underlying support for cheat codes based on datel carts when using an expansion pak.

Caveats:
- Something about cheats and expansion paks.

The current code types are supported:
- 80 (description here)
- D0 (description here)
- Fx (description here)

The codes XX are not supported, because...
- e.g. they rely on the button.

### File parsing support

If a file named the same as the selected rom with the extension `.cht` is found, it will attempt to parse the file for cheat codes and place them in menu->boot_params->cheat_list per the cheat backend API.

The parser ignores lines that start with a `#` or `$`, are under 12 characters or over 15 characters. Every other line needs to be a valid cheat code input with the code on the left, and the value on the right separated by a space.

Cheat files should be formatted this way:
```
# Super mario 64 infinite lives
8033B21D 0064

# 120 stars
80207723 0001
8020770B 00C7
50001101 0000
8020770C 00FF
```

Another example:
```
# Example cheat codes for the game "Majoras Mask USA"
# Enable code
F1096820 2400
FF000220 0000
# Inventory Editor (assigned to L)
D01F9B91 0020
803FDA3F 0002
```

The cheat file needs to be enabled for the specific game (press `R` within the Rom Info).

Check [Pull Requests](https://github.com/Polprzewodnikowy/N64FlashcartMenu/pulls) for work towards GUI editor support.
