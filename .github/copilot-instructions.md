<!-- Copilot / AI agent instructions for N64FlashcartMenu -->
# N64FlashcartMenu — Practical instructions for AI coding agents (Copilot, ChatGPT, etc.)

This file contains focused, actionable knowledge to help an AI assistant become productive in this repository with minimal guesswork.

---

## Purpose
- High-level: a libdragon-based Nintendo 64 flashcart menu project that builds into one or more ROM images under `output/`.
- Key goal for contributors: build the ROM, run it in an emulator or on hardware, and modify menu behavior or flashcart-specific functionality.
- Primary areas of change:
  - Menu logic and UI: `src/menu/`
  - Flashcart drivers: `src/flashcart/`
- Non-goals:
  - No PC-side UI, installer, or runtime configuration system.
  - No external scripting, plugins, or dynamic content loading at runtime.
  - All behavior is compiled into the ROM.

---

## Before making changes
- Identify which flashcart(s) are affected (64drive, EverDrive64, SummerCart64).
- Locate the relevant menu view or flashcart driver before editing.
- Check the Makefile for existing patterns before adding new build rules, flags, or constants.
- Prefer small, localized changes over large refactors.

---

## How to build (practical)
- Primary build: run `make` from repository root.
  - Produces `output/N64FlashcartMenu.n64` and several vendor-specific copies:
    - `output/menu.bin`
    - `output/OS64.v64`, `output/OS64P.v64`
    - `output/sc64menu.n64`
- Useful targets:
  - `make all` — full build + copies for 64drive / ED64 / SC64
  - `make run` — builds then deploys via `remotedeploy.sh` (or `localdeploy.bat` on Windows)
  - `make run-debug`, `make run-debug-upload`, `make run-debug-reboot` — debug deploy helpers (serial output enabled)
  - `make clean` — removes build and output directories
  - `make format` — runs `clang-format` on `src/` (excludes `src/libs`)

### Host environment notes
- Primary development environment: Linux.
- Windows users typically use WSL or the provided `.bat` deployment helpers.
- Shell scripts (`*.sh`) assume a POSIX-compatible shell.

---

## Toolchain / environment
- The Makefile depends on `N64_INST` (libdragon installation path) and includes:
  - `$(N64_INST)/include/n64.mk`
- Ensure `N64_INST` points to a valid libdragon toolchain directory, or use the libdragon Docker image described in `libdragon/README.md`.
- Asset conversion tools used by the build:
  - `N64_MKFONT`
  - `N64_AUDIOCONV`
  - `N64_MKSPRITE`
- Source assets live under `assets/`.
- These tools are invoked automatically by Makefile rules, which are generated from the source assets:
  - Fonts → `filesystem/*.font64`
  - Audio → `filesystem/*.wav64`, `filesystem/*.xm64`
  - Images → `filesystem/*.sprite`
- Documentation site is generated using DocFX:
  - `docfx docfx.json --serve`
  - Sources live in `docs/`

**Important:**  
Do not invoke libdragon tools directly unless debugging the build system. Prefer modifying Makefile rules instead.

---

## External resources & references
- **libdragon**  
  https://github.com/DragonMinded/libdragon/tree/preview  
  Primary API reference for graphics, input, filesystem, audio, and toolchain usage.  
  Wiki: https://github.com/DragonMinded/libdragon/wiki

- **N64brew**  
  https://n64brew.dev/  
  Community-maintained documentation for low-level N64 hardware, timing, RCP behavior, and system internals.

- **Flashcart documentation**
  - 64drive: https://64drive.retroactive.be/support.php
  - EverDrive64: https://krikzz.com/pub/support/everdrive-64/
  - SummerCart64: https://github.com/Polprzewodnikowy/SummerCart64

---

## Key directories & files to inspect
- `src/` — application source (entry point: `src/main.c`)
- `src/menu/` — **most common place for UI and behavior changes**
  - `menu/views/` — screens, navigation, and user-visible logic
  - `menu/ui_components/` — reusable UI widgets
- `src/flashcart/` — **cart-specific behavior only**
  - Separate subdirectories per flashcart (64drive, ed64, sc64)
- `src/libs/` — third-party or shared code; avoid modifying unless necessary
- `assets/` — fonts, images, and sounds converted into `filesystem/` at build time
- `Makefile` — canonical source for build flags, targets, asset conversion, and ROM packaging
- `libdragon/README.md` — toolchain setup and Docker usage
- `docs/` and `docfx.json` — user and developer documentation sources

---

## Project-specific conventions and patterns
- Single binary output:
  - The project builds into one ROM image, with multiple vendor-specific copies generated from it.
- Asset pipeline:
  - Modify source assets under `assets/` (PNG / WAV/ XM / TTF).
  - Let the Makefile handle conversion and embedding.
- C code layout:
  - Organized by feature rather than layer.
  - Follow existing grouping when adding new views or components.
- Flashcart selection:
  - Platform-specific logic lives under `src/flashcart/*`.
  - To add or fix a flashcart, modify the relevant driver files and update the Makefile `SRCS` list.
- Build-time constants:
  - Compiler flags and symbols are defined in the Makefile.
  - Examples: `-DMENU_VERSION`, `-DBUILD_TIMESTAMP`
  - Prefer Makefile definitions over hardcoded values in C.

### Things to avoid
- Introducing C++ or non-libdragon dependencies.
- Adding runtime configuration files or dynamic filesystem writes.
- Creating new memory allocation patterns inconsistent with existing code.
- Bypassing the Makefile to “simplify” the build.

---

## Integration points & outputs
- ROM outputs:
  - `output/N64FlashcartMenu.n64` — main ROM
  - `output/menu.bin` — 64drive menu copy
  - `output/OS64.v64`, `output/OS64P.v64` — EverDrive64 / clone outputs
  - `output/sc64menu.n64` — SummerCart64 build
- These outputs are functionally identical builds with different headers or packaging for specific flashcarts.
- Runtime dependencies:
  - libdragon toolchain and runtime
  - Emulator (Ares or Gopher64 recommended) or physical flashcart hardware

---

## Code examples (where to make small changes)
- Change menu text, layout, or navigation:
  - Edit files under `src/menu/views/` (e.g., `load_rom.c`, `settings.c`)
- Add a new asset:
  - Place `my.png` into `assets/images/`
  - Run `make`; the Makefile converts it into `filesystem/*.sprite`
- Update flashcart low-level routines:
  - Edit `src/flashcart/sc64/sc64_ll.c` or the corresponding `*_ll.c` file
- Application flow:
  - Entry point: `src/main.c`
  - Initialization → menu system → active view in `src/menu/views/`

---

## Tests, formatting & debugging
- No automated unit tests are currently present.
- `Makefile` contains a commented-out `test` placeholder for future use.
- Formatting:
  - Use `make format` to apply `clang-format` across repository sources.
- Manual runtime testing:
  - Build the ROM and run it in an emulator or on real hardware.
  - Use `make run-debug` to enable serial output.
  - `debugf()` output is visible via serial-capable emulators or hardware tools.

---

## Safe vs risky changes
### Safe first changes
- Modify menu text, layout, and navigation logic.
- Add or update assets under `assets/`.
- Adjust behavior in individual menu views.

### Risky changes (review carefully)
- Low-level flashcart drivers under `src/flashcart/*`.
- Timing-sensitive or hardware-facing code.
- Asset conversion or Makefile rule changes.

---

## Guidance for AI coding agents
- Prefer small, incremental changes.
- Follow existing code style, naming, and patterns.
- Do not invent new abstractions unless clearly necessary.
- If behavior is unclear, request clarification instead of guessing.
- Document any errors encountered and steps taken to work around them.

---

## Where to look for documentation and help
- User and developer docs: `docs/*.md` and generated `_site/` HTML
- Toolchain and low-level N64 details: `libdragon/README.md`

If any area above is unclear or you’d like more detail (examples, specific functions to inspect, CI notes, or debugging workflows), indicate which section to expand and iterate.
