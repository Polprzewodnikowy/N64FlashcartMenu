## Emulators
N64FlashcartMenu supports multiple emulators that are compatible with the N64. At the time of writing, current emulator support includes NES, SNES, GB, GBC, SMS, GG, and CHF ROMs.

> [!TIP]
> All emulators supported DO NOT require additional ROM processing or adjustment steps as may be suggested in their readme and original ROMs can be used out of the box.

Emulators should be added to the `sd:/menu/emulators/` directory on the SD card. N64FlashcartMenu currently supports the following emulators and associated ROM file names:

| System | Emulator | ROM extensions | Emulator filename |
|--------|----------|----------------|-------------------|
| **NES** | [Neon64 v1.2/v0.3c/v2](https://github.com/hcs64/neon64v2/releases) by *visor / hcs64* | `.nes` | `neon64bu.rom` |
| **SNES** | [sodium64](https://github.com/Hydr8gon/sodium64/releases) by *Hydr8gon* | `.sfc` `.smc` | `sodium64.z64` |
| **Game Boy** | [gb64](https://lambertjamesd.github.io/gb64/romwrapper/romwrapper.html) by *lambertjamesd* ("Download Emulator" button) | `.gb` | `gb.v64` |
| **Game Boy Color** | [gb64](https://lambertjamesd.github.io/gb64/romwrapper/romwrapper.html) by *lambertjamesd* ("Download Emulator" button) | `.gbc` | `gbc.v64` |
| **SMS / Game Gear / SG-1000** | [smsPlus64](https://github.com/fhoedemakers/smsplus64/releases) by *fhoedemakers* | `.sms` `.gg` `.sg` | `smsPlus64.z64` |
| **Fairchild Channel F** | [Press-F-Ultra](https://github.com/celerizer/Press-F-Ultra/releases) by *celerizer* | `.chf` | `Press-F.z64` |

> [!WARNING]
> **NES**: [Neon64 v1.2/v0.3c](https://hcs64.com/neon64.html) in-game autosaves may fail (work around using save states).

## Per-emulator overrides

You can override the emulator ROM filename, save type, and ROM load address without modifying the menu by creating `sd:/menu/emulators.ini` on the SD card.  The file is optional — when absent, all built-in defaults apply.

Each section corresponds to an emulator system.  Only keys you wish to override need to be present.

```ini
[nes]
rom        = neon64bu.rom   ; emulator filename inside /menu/emulators/
save_type  = 5              ; flashcart_save_type_t integer (see table below)
rom_offset = 0x200000       ; ROM load address in RDRAM (decimal or 0x hex)

[snes]
rom = sodium64.z64

[gb]
rom = gb.v64

[gbc]
rom = gbc.v64

[sega8bit]
rom = smsPlus64.z64

[channelf]
rom = Press-F.z64

[zxspectrum]
rom = zx-spectrum.z64
```

### `save_type` values

| Value | Type |
|-------|------|
| 0 | None |
| 1 | EEPROM 4Kbit |
| 2 | EEPROM 16Kbit |
| 3 | SRAM 256Kbit |
| 4 | SRAM Banked |
| 5 | SRAM 1Mbit |
| 6 | FlashRAM 1Mbit |
| 7 | FlashRAM PKST2 |

If you are an emulator developer and are interested in adding your emulator, take a look at this [template pull request](https://github.com/Polprzewodnikowy/N64FlashcartMenu/pull/178).
