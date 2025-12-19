## Frequently Asked Questions (FAQ)

### ROM hack (insert hack name here) does not work
- Most ROM hacks rely on an Expansion Pak, or might be only compatible with emulators.
- Some (very old) hacks override the CIC or save type that is expected from the internal N64FlashcartMenu database. (for more information on how to change the expected types, [read here](./12_rom_configuration.md)).

### My Roms are all in individual ZIP files and it is hassle to extract them before adding them to the SD card
The menu supports the ability to extract a file from a zipped folder, however it is best to extract them on a PC first.
You can try running a powershell script to extract them before adding them to the SD card:
```powershell
$exts = @("*.n64", "*.z64", "*.v64"); Get-ChildItem -Filter "*.zip" | ForEach-Object { Expand-Archive $_.FullName -DestinationPath "$($_.BaseName)_temp" -Force; Get-ChildItem "$($_.BaseName)_temp\*" -File -Include $exts | Move-Item -Destination .; Remove-Item "$($_.BaseName)_temp" -Recurse -Force }
```

### I am using macOS and want to clean unwanted files before adding them to the SD card
The menu should filter out macOS shadow files, but for a clean SD card:
On macOS, if you have extracted ROM's from ZIP or other compressed files, run `dot_clean -m /Volumes/SummerCart` to clear those awful dotfiles.

### I have changed the menu/config.ini file manually and things are not working.
Delete the file. It will be re-created automatically with the default settings.

### Filenames have incorrect or missing characters
Western Europe and most Japanese characters are fully compatible. Other Unicode characters, such as those from Eastern Europe, Russia or Middle East regions (to name just a few examples) may not be fully supported and may not be displayed.

### Loading ROM's are slower than on other flashcart menus
> [!WARNING]
> On the first load of a ROM, the menu may need to create a save file. This may lead to a one-time decrease in ROM load speed.

> [!TIP]
> An SD formatted to EXFAT with 128 kiB cluster size may improve load speeds.

### I am getting file operation failures 
- Make sure there is enough space on the SD card and that the file is not write-protected.
- If the flashcart included a Micro SD card as part of the purchase and a file operation fails, follow this section in full.

### N64 plays the game for xxx minutes before blank screen
The flashcart uses more power than a stock game and (especially when used in conjunction with other mods or accessories) may cause weak power supplies to "brownout".

### My games crash
The flashcart uses more power than an original cartridge. Check the power supply first.
If this still happens, and it is an original known dump of a commercial game, an anti-piracy check may be occurring (please raise an issue).

### My Clock keeps resetting
> [!CAUTION]
> The battery was inserted incorrectly (you have likely flattened the positive tabs by inserting it incorrectly),
> [![Watch how to install the battery correctly](https://img.youtube.com/vi/LV5Aj_P3U_E/default.jpg)](https://www.youtube.com/shorts/LV5Aj_P3U_E)
