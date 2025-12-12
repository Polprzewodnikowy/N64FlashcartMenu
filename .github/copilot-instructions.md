<!-- Copilot / AI agent instructions for N64FlashcartMenu -->
# N64FlashcartMenu — Quick instructions for AI coding agents

This file contains focused, actionable knowledge to help an assistant become productive in this repository.

### Purpose
- Top-level: a libdragon-based N64 menu project that builds to ROMs in `output/`.
- Key goal for contributors: build the ROM, run in emulator/hardware, and modify menu behavior across `src/menu` and `src/flashcart` drivers.

### How to build (practical)
- Primary build: run `make` from repository root. This produces `output/N64FlashcartMenu.n64` and several vendor-specific copies (`output/menu.bin`, `output/OS64.v64`, `output/sc64menu.n64`).
- Useful targets:
  - `make all` — full build + copies for 64drive/ED64/SC64
  - `make run` — builds then deploys via `remotedeploy.sh` (or `localdeploy.bat` on Windows)
  - `make run-debug`, `make run-debug-upload` — debug-deploy helpers
  - `make clean` — removes build and output directories
  - `make format` — runs `clang-format` on `src` (excludes `src/libs`)

### Toolchain / environment
- The Makefile depends on `N64_INST` (libdragon installation) and includes `$(N64_INST)/include/n64.mk`. Ensure `N64_INST` points to a libdragon toolchain directory or use the libdragon Docker image described in `libdragon/README.md`.
- Building assets uses `N64_MKFONT`, `N64_AUDIOCONV`, and `N64_MKSPRITE` tools (see `Makefile` rules that produce `filesystem/*.font64`, `*.wav64`, `*.sprite`). Source assets live under `assets/`.
- Documentation site is produced with DocFX: use `docfx docfx.json --serve` (see `docfx.json` and `docs/` markdown files).

### Key directories & files to inspect
- [src/](src/) — application source (entry: `src/main.c`).
- [src/flashcart/](src/flashcart/) — per-flashcart drivers (64drive, sc64, ed64). Update here for cart-specific behavior.
- [src/menu/](src/menu/) — UI, views and logic for the menu.
- [assets/] — fonts, images, sounds converted into `filesystem/` at build time.
- [Makefile](Makefile) — canonical build targets, flags, and asset conversion rules.
- [libdragon/README.md](libdragon/README.md) — describes required toolchain and Docker option.
- [docs/](docs/) and `docfx.json` — user/developer documentation sources.

### Project-specific conventions and patterns
- Single binary: the project compiles into one ROM image. Many assets are converted and embedded into `filesystem/` via Makefile rules — prefer modifying source PNG/WAV/TTF under `assets/` and let the build convert them.
- C layout: source is organized by feature (e.g., `menu/views/*`, `menu/ui_components/*`). Follow existing file grouping when adding new screens/components.
- Flashcart selection: platform-specific behavior is guarded by separate files under `src/flashcart/*`. To add or fix a flashcart, add/modify the relevant driver files and Makefile sources list (`SRCS`).
- Compiler flags and symbols are set in Makefile (e.g., `-DMENU_VERSION`, `-DBUILD_TIMESTAMP` in `menu/views/credits.c` object rule). Use Makefile for any build-time constants.

### Integration points & outputs
- ROM outputs and packaging:
  - `output/N64FlashcartMenu.n64` — main ROM
  - `output/menu.bin` — 64drive/menu copy created by the `64drive` target
  - `output/OS64.v64`, `output/OS64P.v64` — ED64/clone outputs
  - `output/sc64menu.n64` — SummerCart64 build
- Runtime dependencies: libdragon (toolchain and runtime), emulator (Ares/Gopher64 recommended) or physical flashcart.

### Code examples (where to make small changes)
- Change menu text or structure: edit views under `src/menu/views/` (e.g., `load_rom.c`, `settings.c`).
- Add a new asset: put `my.png` into `assets/images/`, then run `make` — the Makefile converts it to `filesystem/*.sprite` automatically.
- Update a flashcart low-level routine: edit `src/flashcart/sc64/sc64_ll.c` or the corresponding `*_ll.c` file.

### Tests & linting
- There are no automated unit tests present. `Makefile` has a commented `test` placeholder.
- Use `make format` to apply `clang-format` across the repository sources.

### Where to look for documentation and help
- User/developer docs: `docs/*.md` and generated `_site/` HTML.
- Toolchain and low-level N64 details: [libdragon/README.md](libdragon/README.md)

If any area above is unclear or you'd like more detail (examples, specific functions to inspect, or CI notes), tell me which section to expand and I will iterate.
