[Return to the index](./00_index.md)
## Experimental Features (Subject to change)

### ROM info descriptions (pre-release only)
To show a ROM description in the N64 ROM information screen, add a `.TXT` file next to the game ROM file with the same name and the following content:
```
[metadata]
description=This is the ROM description that does X Y Z.
```
Text files must use ASCII characters only, Linux `LF` endings (CRLF is not supported) and the descriptions themselves must be limited to 300 characters.
