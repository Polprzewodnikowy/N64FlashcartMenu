## 64drive developer notes

### Official documentation

http://64drive.retroactive.be/64drive_hardware_spec.pdf


### Save location offset in SDRAM

| Type                         | HW1          | HW2          |
| ---------------------------- | ------------ | ------------ |
| SRAM                         | `0x03FE0000` | `0x0FFC0000` |
| FlashRAM                     | `0x03FE0000` | `0x0FFC0000` |
| SRAM banked                  | `0x03FE0000` | `0x0FFC0000` |
| FlashRAM (Pokemon Stadium 2) | `0x01606560` | `0x0FFC0000` |

EEPROM save types are stored in separate memory inside FPGA, rest of the save types are stored inside SDRAM memory.
EEPROM save types need manual load as this memory space can't be written with "Read multiple sectors to SDRAM" command.


### "Persistent variable storage" register

| Bits      | Meaning                                                    |
| --------- | ---------------------------------------------------------- |
| `[31:17]` | _Unused_                                                   |
| `[16]`    | Reset behavior: `0` - boot to menu / `1` - quick boot game |
| `[15:10]` | _Unused_                                                   |
| `[9:8]`   | Force TV type                                              |
| `[7:0]`   | CIC seed                                                   |

It's used by the bootloader to quickly load game without running menu again.
Should contain bootloader version but it's zeroed if ROM wasn't ran through bootloader (e.g. ROM was loaded via USB).


### "Enable/disable save writeback" command

Does not work when USB cable is connected - writeback is forced to be in disabled state.
Curiously, official 64drive menu never calls this command, save writeback might be enabled by default.


### "Enable/disable byteswap on load" command

Annoyingly, this command affects both loading single sector into the buffer and loading multiple sectors to the SDRAM.


### "Enable/disable extended address mode" command

As of latest available firmware version 2.05 this command is not implemented.
Documentation specifies it's supported on firmwares 2.06+ but this version (or anything newer) was never published.
