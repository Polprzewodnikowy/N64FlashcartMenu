# N64 Flashcart In-Game Reset (IGR) — SummerCart64 & N64FlashcartMenu

**Research Date:** 2026-05-27  
**Goal:** Determine feasibility of returning to the flashcart menu from a running N64 game via a controller button combination.  
**Target Hardware:** SummerCart64 (SC64) + N64FlashcartMenu  
**Status:** Feasible via ROM patching in N64FlashcartMenu; SC64 firmware already provides the necessary reboot primitive.

---

## 1. The Problem

Commercial/retail N64 games have no built-in mechanism to return to the flashcart menu. Once a ROM boots, the flashcart menu is no longer running. The user must physically power-cycle or reset the console to return to the menu.

Other flashcarts (EverDrive-64, 64drive) support IGR via controller button combos. The SC64 hardware is capable, but the software stack (N64FlashcartMenu) currently lacks this feature.

---

## 2. SummerCart64 Hardware Capabilities

### Expanded Interrupt Support (Firmware v2.19.0+)
The SC64 MCU/FPGA can signal the following events via **cart interrupt** (N64 Int1 / COP0 Cause bit 1):

| Event | Source |
|-------|--------|
| Button press (physical button on cart) | `BUTTON_MODE = 1` in config |
| Command finish | `CMD_IRQ_REQUEST` bit in SCR register |
| USB data waiting | `USB_IRQ_ENABLE` in IRQ register |
| AUX register waiting to be read | `AUX_IRQ_ENABLE` in IRQ register |

### SC64 Registers
| Address | Name | Purpose |
|---------|------|---------|
| `0x1FFF_0000` | SCR | Command/status register |
| `0x1FFF_0004` | DATA0 | Command arg/result |
| `0x1FFF_0008` | DATA1 | Command arg/result |
| `0x1FFF_0014` | IRQ | Interrupt control/clear |
| `0x1FFF_0018` | AUX | 32-bit message register (PC <-> N64) |

### AUX Register Reboot Primitive
The AUX register has **reserved values** for internal SC64 use:

| Value | Meaning |
|-------|---------|
| `0xFF000000` | **Ping** — no-op to test if app is listening |
| `0xFF000001` | **Halt** — stop all activity, prepare for upload |
| `0xFF000002` | **Reboot** — soft reboot: reload IPL3 from ROM and start executing it |

If the SC64 bootloader is configured to load `sc64menu.n64` from SD, writing `0xFF000002` to AUX effectively returns to the menu.

**Write address:** `0xBFFF0018` (uncached)  
**Note:** All `DATA` values with upper 8 bits set to `1` (`0xFFxxxxxx`) are reserved for SC64 internal use.

### Button Config
`BUTTON_MODE` (config option 13) controls the physical button:
- `0` — Does nothing
- `1` — Raises N64 cart interrupt (Int1)
- `2` — Sends USB packet to PC
- `3` — Changes 64DD disk

---

## 3. Why the SC64 Firmware Alone Cannot Solve This

The SC64 sits on the **PI (cartridge) bus**. It can:
- Respond to memory reads/writes from the N64 CPU
- Generate cart interrupts (Int1)
- Transfer data via USB/SD

It **cannot**:
- See the CPU program counter
- Inject instructions into the instruction stream on-the-fly
- Snoop the controller bus (Joybus goes through PIF/SI, not PI)

The "every frame" controller combo check **must run as N64 CPU code**. The flashcart hardware does not have bus-snooping/injection capabilities like some EverDrive models.

---

## 4. The Solution: Pre-Boot ROM Patching in N64FlashcartMenu

`N64FlashcartMenu` runs on the N64 CPU before the game boots. When the user selects a ROM, the menu loads it into the SC64's SDRAM. Before transferring control to the game's entry point, the menu has **full read/write access** to every byte of the loaded ROM image.

### High-Level Flow

```
1. User selects ROM in N64FlashcartMenu
2. Menu loads ROM into SC64 SDRAM
3. Menu scans/patches the ROM in memory:
   a. Locate a safe injection point
   b. Install a VI interrupt hook
   c. Inject payload that polls controller + checks combo
4. Menu jumps to game's entry point
5. Game runs normally; every frame the VI interrupt fires
6. Injected hook checks controller combo
7. On match: write 0xFF000002 to SC64 AUX → soft reboot → back to menu
```

### Why This Works

This is exactly how **GameShark**, **Action Replay**, and **EverDrive IGR** work:
- They intercept the exception/interrupt path
- Run their own code periodically (every VI retrace)
- Check for trigger conditions (cheat codes, button combos)
- Either act or chain to the original handler

---

## 5. The N64 Interrupt Path

### MIPS Interface (MI) Registers
| Register | Address | Purpose |
|----------|---------|---------|
| `MI_INTR_REG` | `0x04300008` | Pending interrupts (read-only, per-component) |
| `MI_INTR_MASK_REG` | `0x0430000C` | Interrupt enable mask (read/write) |

Interrupt fires when `(MI_INTR_REG & MI_INTR_MASK_REG) != 0`.

### MI Interrupt Bits
| Bit | Source |
|-----|--------|
| 0 | SP (RSP) |
| 1 | SI (Serial Interface / PIF) |
| 2 | AI (Audio Interface) |
| 3 | VI (Video Interface / vertical retrace) |
| 4 | PI (Peripheral Interface / DMA) |
| 5 | DP (RDP / Display Processor) |

**VI interrupt (bit 3)** fires at ~60 Hz (NTSC) or ~50 Hz (PAL), making it the ideal "every frame" hook.

### CPU Exception Flow
1. Exception occurs → CPU sets `EXL=1`, saves PC to `EPC`
2. CPU jumps to exception vector:
   - `BEV=0` (normal): `0x80000180` (general exceptions)
   - `BEV=1` (early boot): `0xBFC00380`
3. `COP0 $Cause` register contains exception code:
   - Code `0` = Interrupt
   - Bits 8-15 = Interrupt Pending (IP) field

### N64 Cause Register Interrupt Bits
| Bit | Name | Source |
|-----|------|--------|
| 8 | IP0 | RCP (Int0) — MI interrupts |
| 9 | IP1 | Cart/DD (Int1) — SC64 cart interrupt |
| 10 | IP2 | Pre-NMI / Reset button |
| 11 | IP3 | Pre-NMI (software) |
| 12-15 | IP4-IP7 | Unused on N64 |

The SC64 cart interrupt maps to **Int1 (Cause bit 9)**.

### libdragon CART Interrupt Support
libdragon exposes CART interrupt callbacks:
```c
void register_CART_handler(void (*callback)());
void unregister_CART_handler(void (*callback)());
```

This is how homebrew apps can listen to SC64 events (button press, AUX, USB).

---

## 6. Injection Strategies

### Strategy A: Hook the General Exception Vector

**Target:** `0x80000180` (default general exception vector)

**How:**
- Most libultra games leave the exception vector at `0x80000180`
- Overwrite the first instruction to jump to your payload
- Your payload:
  1. Check `Cause` register for VI interrupt (code 0, IP0 set)
  2. If VI: poll controller, check combo
  3. If combo matched: write `0xFF000002` to `0xBFFF0018`
  4. Otherwise: jump to original handler

**Coverage:** ~70% of libultra games that don't relocate the vector.

### Strategy B: Hook libultra __osException

**Target:** The libultra interrupt dispatcher function `__osException`

**How:**
- libultra installs `__osException` as the handler at `0x80000180`
- It reads `Cause`, dispatches to registered callbacks (VI, PI, etc.)
- Patch the prologue of `__osException` to call your code first

**Coverage:** ~85% of retail games (covers most libultra versions).

**Challenge:** Multiple libultra versions exist with different function signatures and addresses. Need per-version signatures.

### Strategy C: Hook the Game's VI Retrace Callback

**Target:** The game's own callback registered with `osViSetEvent` or `osSetEventMesg(OS_EVENT_VI)`

**How:**
- libultra games register a function pointer for VI events
- Find the callback table or function pointer in RAM
- Redirect to your wrapper that checks the combo then chains

**Coverage:** ~75% of games that use the standard VI callback mechanism.

### Strategy D: Patch the Game's Entry Point

**Target:** The very first instruction the game executes after IPL3

**How:**
- Overwrite entry point to jump to your setup code
- Your setup code installs the VI hook, then jumps to the original entry point
- Game proceeds normally but your hook is active

**Coverage:** Near-universal for boot, but the game may later replace your hook.

---

## 7. The Payload

### What the Injected Code Must Do

```asm
; Simplified MIPS assembly for the injected hook
; Runs every VI interrupt (every frame)

hook_entry:
    ; Save registers (minimal set)
    addiu $sp, $sp, -32
    sw    $ra, 28($sp)
    sw    $t0, 24($sp)
    sw    $t1, 20($sp)
    sw    $t2, 16($sp)

    ; Check if this is a VI interrupt
    mfc0  $t0, $13           ; Read Cause register
    andi  $t1, $t0, 0xFF00   ; Isolate IP field
    beq   $t1, $zero, not_vi ; If no interrupt pending, bail
    nop

    ; Check specifically for IP0 (RCP/Int0 — MI interrupts)
    andi  $t1, $t0, 0x0100   ; IP0 bit
    beq   $t1, $zero, not_vi
    nop

    ; Check MI_INTR_REG for VI specifically
    lui   $t0, 0x0430
    lw    $t1, 0x0008($t0)   ; MI_INTR_REG
    andi  $t2, $t1, 0x0008   ; VI bit (bit 3)
    beq   $t2, $zero, not_vi
    nop

    ; --- This is a VI interrupt ---
    ; Poll controller state
    ; (Implementation depends on whether game uses libultra SI DMA or direct)
    ; For simplicity, read from game's controller buffer if known,
    ; or poll SI directly

    jal   poll_controller_combo
    nop

    beq   $v0, $zero, combo_not_pressed  ; $v0 = 0 if no match
    nop

    ; Combo matched! Trigger reboot via SC64 AUX
    lui   $t0, 0xBFFF        ; Uncached KSEG1 address for SC64 registers
    li    $t1, 0xFF000002   ; SC64 AUX Reboot value
    sw    $t1, 0x0018($t0)  ; Write to AUX register at 0xBFFF0018
    nop                      ; SC64 handles the rest (soft reboot)

combo_not_pressed:
not_vi:
    ; Restore registers
    lw    $ra, 28($sp)
    lw    $t0, 24($sp)
    lw    $t1, 20($sp)
    lw    $t2, 16($sp)
    addiu $sp, $sp, 32

    ; Jump to original handler
    j     original_handler
    nop
```

### Controller Combo Detection

The simplest approach is to read the game's own controller data structure. libultra games store controller state in a known format (`OSContPad`). The patcher would need to locate this buffer or read it directly.

**Typical combo:** `L + R + Z + Start` (common across flashcarts and IGR mods)

---

## 8. Space for the Payload

### Finding Room in Memory

The payload needs a place to live. Options:

| Location | Pros | Cons |
|----------|------|------|
| End of ROM padding | Easy, no RAM used | Not all ROMs have padding |
| Expansion Pak RAM (`0x80400000+`) | 4-8 MB free if Pak present | Not all games use Expansion Pak; may conflict |
| Overwrite unused game data | Always space somewhere | Game-specific; may break something |
| libultra padding/bss | Known safe areas in libultra games | Per-version |

### Practical Approach

1. Check if Expansion Pak is present — if so, use `0x80400000+` (safest)
2. If no Expansion Pak, scan ROM for zero-filled padding regions
3. Append payload to end of ROM data if space exists

---

## 9. What Breaks the "One Size Fits All" Dream

| Issue | Impact |
|-------|--------|
| **Games that relocate exception vector** | Your hook at `0x80000180` is ignored |
| **libdragon games** | Different interrupt system, no libultra patterns |
| **Games with tight memory packing** | No obvious gap for payload |
| **Anti-piracy checksums** | Game detects ROM modification and crashes/errors |
| **Games that don't use VI interrupts** | Rare; usually test ROMs or unusual homebrew |
| **Custom/proprietary SDKs** | Midway, Rare, and some Japanese devs used custom middleware |
| **Multiple libultra versions** | `__osException` signature varies; need per-version detection |

### Coverage Estimate

| Approach | Estimated Coverage |
|----------|-------------------|
| Generic exception vector hook | ~60-70% |
| libultra `__osException` hook (5-10 version signatures) | ~80-90% |
| Per-game database (like EverDrive) | ~99% |

---

## 10. How Other Solutions Work

### EverDrive-64 X7 IGR
- The EverDrive FPGA has more sophisticated bus-snooping/injection
- Can monitor CPU activity and inject without full ROM patches
- Maintains a per-game compatibility database

### 64drive IGR
- Similar to EverDrive; hardware-assisted
- Uses different FPGA architecture

### N64RGB (Borti) IGR
- **Hardware mod**, not flashcart-based
- A CPLD sits between the controller bus and the console
- Sniffs controller protocol directly
- On combo detection, triggers console reset line
- Completely game-agnostic because it's hardware-level

### GameShark / Action Replay
- **Software patching** approach (same as what N64FlashcartMenu would do)
- Hooks the exception/interrupt path
- Code handler runs every frame
- Applies cheat codes or trigger actions

---

## 11. N64FlashcartMenu Context

### Existing Patching Infrastructure
- The menu's changelog mentions `FEATURE_PATCHER_GUI_ENABLED` (Alpha, v0.3.0)
- This suggests nascent interest in patching infrastructure
- Current docs explicitly state: *"N64FlashcartMenu does not support patching on-the-fly"*

### Relevant Source Files
- `src/menu/cart_load.c` — ROM loading logic
- `src/flashcart/` — Flashcart abstraction layer (SC64, 64drive, etc.)

### What a PR Would Need

1. **Pre-boot patcher module**
   - Runs after ROM is loaded into SDRAM but before boot
   - Scans ROM for known patterns (libultra version, entry point, exception vector)
   - Applies the appropriate generic patch

2. **Signature database**
   - 5-10 libultra version signatures
   - 1-2 libdragon patterns
   - Detection heuristics for unknown ROMs

3. **Payload assembler**
   - MIPS assembly generator for the VI hook
   - Configurable button combo
   - SC64 AUX reboot trigger

4. **Settings integration**
   - Enable/disable IGR per-ROM or globally
   - Configurable button combo
   - Fallback behavior when patch fails

5. **Flashcart-agnostic design**
   - SC64 uses AUX register (`0xBFFF0018`) for reboot
   - Other flashcarts may need different reboot mechanisms
   - Abstract the "return to menu" primitive

---

## 12. Recommended Button Combo

Based on existing IGR implementations:

| Flashcart/Mod | Default Combo |
|---------------|---------------|
| EverDrive-64 | Varies by firmware |
| N64RGB (Borti) | Configurable (default: `Z + R + Start` or similar) |
| PS1 IGR mods | `L + R + Select + Start` |
| SNES sd2snes | `L + R + Select + Start` |
| Common N64 preference | `L + R + Z + Start` |

**Suggestion:** Make it configurable, default to `L + R + Z + Start`.

---

## 13. Key Technical References

### SummerCart64 Documentation
- [SC64 Memory Map](https://github.com/Polprzewodnikowy/SummerCart64/blob/main/docs/01_memory_map.md)
- [SC64 USB Interface](https://github.com/Polprzewodnikowy/SummerCart64/blob/main/docs/03_usb_interface.md)
- [SC64 Config Options](https://github.com/Polprzewodnikowy/SummerCart64/blob/main/docs/04_config_options.md)

### N64FlashcartMenu
- [GitHub Repo](https://github.com/Polprzewodnikowy/N64FlashcartMenu)
- [Menu Documentation](https://menu.summercart64.dev)
- `FEATURE_PATCHER_GUI_ENABLED` build flag (Alpha)

### N64 Architecture
- [N64brew MIPS Interface](https://n64brew.dev/wiki/MIPS_Interface)
- [n64docs Interrupts](https://n64.readthedocs.io/)
- [libdragon Interrupt Source](https://github.com/DragonMinded/libdragon/blob/trunk/src/interrupt.c)

### Existing IGR Implementations
- [N64RGB IGR](https://github.com/RetroGamerStuff/n64rgb/blob/master/IGR.README.md) — Hardware CPLD approach
- [N64 IGR Arduino](https://www.reddit.com/r/n64/comments/awpb4d/) — Controller bus sniffing mod

---

## 14. Summary & Recommendation

| Question | Answer |
|----------|--------|
| Can SC64 firmware do this alone? | **No** — needs N64 CPU code to poll controller |
| Can N64FlashcartMenu do this? | **Yes** — via pre-boot ROM patching |
| Is a truly universal (100%) patch possible? | **No** — but 80-90% coverage is achievable with generic signatures |
| What does the menu need? | A ROM patcher that installs a VI interrupt hook before boot |
| What does the SC64 provide? | `AUX` register reboot primitive (`0xFF000002` → soft reboot) |
| Is this how other flashcarts do it? | **Yes** — EverDrive/GameShark use exception-path hooks |
| What's the hardest part? | Making it reliable across diverse ROMs without per-game databases |

### Suggested PR Scope

**Phase 1:** Framework + SC64 support
- Generic VI hook installer
- libultra signature detection (top 3-5 versions)
- Configurable button combo
- Expansion Pak payload placement
- Settings UI toggle

**Phase 2:** Broader coverage
- More libultra versions
- libdragon detection
- Fallback for unsupported ROMs (skip IGR, log warning)
- Support for other flashcarts (64drive, EverDrive)

**Phase 3:** Per-game database (if needed)
- Community-maintained per-ROM patches for outliers
- Auto-download or bundled database

---

## 15. Open Questions

1. Does the SC64 bootloader always reload `sc64menu.n64` on soft reboot, or does it depend on `BOOT_MODE` config?
2. What is the exact state of `FEATURE_PATCHER_GUI_ENABLED`? Is there any existing patching code to build on?
3. Are there known safe RAM regions across libultra versions for payload placement?
4. Can the menu detect Expansion Pak presence reliably before patching?
5. How does the menu currently handle ROMs that use the full 64 MiB SDRAM (leaving no room for payload)?
