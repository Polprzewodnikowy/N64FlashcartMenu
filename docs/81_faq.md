[Return to the index](./00_index.md)
## Frequently Asked Questions (FAQ)

### ROM hack (insert hack name here) does not work
- Most ROM hacks rely on an Expansion Pak, or might be only compatible with emulators.
- Some (very old) hacks override the CIC or save type that is expected from the internal N64FlashcartMenu database. (for more information on how to change the expected types, [read here](./12_rom_configuration.md)).

### My Roms are all in individual ZIP files and it is hassle to extract them before adding them to the SD card
You can try running a powershell script to extract them before adding them to the SD card:
```
$exts = @("*.n64", "*.z64", "*.v64"); Get-ChildItem -Filter "*.zip" | ForEach-Object { Expand-Archive $_.FullName -DestinationPath "$($_.BaseName)_temp" -Force; Get-ChildItem "$($_.BaseName)_temp\*" -File -Include $exts | Move-Item -Destination .; Remove-Item "$($_.BaseName)_temp" -Recurse -Force }
```

### I am using macOS and want to clean unwanted files before adding them to the SD card
On macOS, if you have extracted ROM's from ZIP or other compressed files, run `dot_clean -m /Volumes/SummerCart` to clear those awful dotfiles.

### I have changed the menu/config.ini file manually and things are not working.
Delete the file. It will be re-created automatically with the default settings.

### Filenames have incorrect or missing characters
Filenames are expected to be written in ASCII, with Western Europe characters fully compatible. Other Unicode characters, such as those from Eastern Europe, Russia, Asia or Middle East regions (to name just a few examples) are not fully supported and may not be displayed.

### Loading ROM's are slower than on other flashcart menus
> [!TIP]
> An SD formatted to EXFAT with 128 kiB cluster size may improve load speeds.

> [!WARNING]
> On the first load of a ROM, the menu may need to create a save file. This may lead to a one time only increase in ROM load speed.
