[Return to the index](./00_index.md)
## Emulators
N64FlashcartMenu supports multiple in-built emulators that are compatible with the N64. At the time of writing, current emulator support includes NES, SNES, GB, GBC, SMS, GG, and CHF ROMs.

Emulators should be added to the `SD:/menu/emulators` directory if you want to update them from the default in-built versions. 
N64FlashcartMenu currently supports the following emulators and associated ROM file names:
- **NES**: [Neon64 v1.2/v0.3c](https://hcs64.com/neon64.html) by *visor / hcs64* - `neon64bu.rom`
- **SNES**: [sodium64](https://github.com/Hydr8gon/sodium64/releases) by *Hydr8gon* - `sodium64.z64`
- **Game Boy**/**GB Color**: [gb64](https://lambertjamesd.github.io/gb64/romwrapper/romwrapper.html) by *lambertjamesd* - `gb.v64`/`gbc.v64` ("Download Emulator" button)
- **SMS**/**GG**: [smsPlus64](https://github.com/fhoedemakers/smsplus64/releases) by *fhoedmakers* - `smsPlus64.z64`
- **Fairchild Channel F**: [Press-F-Ultra](https://github.com/celerizer/Press-F-Ultra/releases) by *celerizer* - `Press-F.z64`

> [!WARNING]
> **NES**: [Neon64 v1.2/v0.3c](https://hcs64.com/neon64.html) in-game autosaves may fail (work around using save states).

> [!WARNING]
> **NES**: [neon64 v2 beta releases](https://github.com/hcs64/neon64v2/releases) by *hcs64* currently have issues with saves. 

If you are an emulator developer and are interested in adding your emulator, take a look at this [template pull request](https://github.com/Polprzewodnikowy/N64FlashcartMenu/pull/178).
