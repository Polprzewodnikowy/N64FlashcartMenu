[..](./00_index.md)
## Experimental features
These features are subject to change:

### ROM Info descriptions (pre-release only)
To add a ROM description (shown when loading a ROM), add a file with the content
```
[metadata]
description=This is the ROM description that does X Y Z.
```
The file line endings must use the linux `LF` endings only (CRLF is not supported) ASCII characters only and is limited to 300 characters.
