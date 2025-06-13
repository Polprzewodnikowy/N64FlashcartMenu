# Rolling release
built from latest commit on main branch.  
**Note**: the attached source code files may be out of date.

- For the SummerCart64, use the `sc64menu.n64` file in the root of your SD card.
- For the 64Drive, use the `menu.bin` file in the root of your SD card.
- For the ares emulator, use the `N64FlashcartMenu.n64` file.

## Release Notes 2025-03-31

- **New Features**
	- Introduced tabs in main menu for ROM favorites and recently played ROM history.
	- Introduced first run check to ensure users are aware of latest changes.
	- Introduced ability to turn off GUI loading bar.
	- BETA_FEATURE: Introduces ROM descriptions from files.
	- BETA_FEATURE: Enabled setting for fast ROM reboots on the SC64.
	- Add macOS metadata to hidden files.
	- Added settings schema version for future change versioning.
	- Added setting for PAL60 compatibility mode (see breaking changes).
	- BETA_FEATURE: Added setting for line doublers that need progressive output, enable using "force_progressive_scan" setting in `config.ini`.


- **Bug Fixes**
	- Menu sound FX issues (hissing, popping and white noise).
	- RTC not showing or setting correct date parameters in certain circumstances.
	- ~~GB / GBC emulator not saving in certain circumstances.~~


- **Documentation**
	- Re-orginised and improved user documentation.
	- Added a lot of doxygen compatible code comments.
	- Added project license.


- **Refactor**
	- RTC subsystem (align with libDragon improvements).
	- Boxart images (Deprecates old boxart image folder layout).
	- Settings (PAL60 compatibility, schema version, fast reboot, first run, progress bar).

- **Other**
	- Updated libDragon SDK.
	- Updated miniz library.

### Breaking changes
* ~~GB /GBC emulator changed save type to SRAM (from FRAM) to improve compatibility with Summercart64 (which only uses H/W compatible FRAM), this may break your ability to load existing saves.~~
* For similar PAL60 functionality, you may need to also enable the new "pal60_compatibility_mode" setting in `config.ini`.


### Current known Issues
* The RTC UI requires improvement (awaiting UI developer).
* Menu sound FX may not work properly when a 64 Disk Drive is also attached (work around: turn sound FX off).
* Fast Rebooting a 64DD disk once will result in a blank screen. Twice will return to menu. This is expected until disk swapping is implemented.
* MP3 Player crashes menu if the MP3 file's sample rate is less than 44100 hz.


### Deprecation notices
* Autoload ROM's will be deprecated in favor of Fast Reboot in a future menu version.
* Old boxart images using filenames for game ID is deprecated and the compatibility mode will be removed in a future release.


## Release Notes 2025-01-10

- **Bug Fixes**
	- Fixed menu display (PAL60) by reverted libdragon to a known working point and re-applying old hacks.

### Current known Issues
* The RTC UI requires improvement (awaiting UI developer).
* Menu sound FX may not work properly when a 64 Disk Drive is also attached (work around: turn sound FX off).
[Pre-release menu]:
* BETA_SETTING: PAL60 when using HDMI mods has regressed (awaiting libdragon fix).
* ALPHA_FEATURE: ED64 X Series detection does not occur properly (however this is not a problem as not tag released asset).
* ALPHA_FEATURE: ED64 V Series only supports loading ROMs (however this is not a problem as not tag released asset).


## Release Notes 2024-12-30

- **New Features**
	- Introduced menu sound effects for enhanced user experience (the default is off).
	- Added N64 ROM autoload functionality, allowing users to set a specific ROM to load automatically.
	- Added menu boot hotkey (hold `start` to return to menu when autoload is enabled).
	- Added context menu and settings management options GUI for managing various settings in `config.ini`.
	- Added functionality for editing the real-time clock (RTC) within the RTC menu view.
	- Improved flashcart info view for showing supported flashcart features and version.
	- Enhanced UI components with new drawing functions and improved organization.
	- Added emulator support for `SMS`, `GG`, and `CHF` ROMs.
	- Enhanced joypad input handling for menu actions, improving responsiveness.
	- Optimized boxart image loading from filesystem.
	- Improved various text to make the functionality more clear.

- **Bug Fixes**
	- Improved error handling in multiple areas, particularly in save loading and ROM management.
	- Enhanced memory management to prevent potential leaks during error conditions.
	- Fixed text flickering in certain circumstances.

- **Documentation**
	- Updated README and various documentation files to reflect new features and usage instructions.
	- Added detailed setup instructions for SD cards and menu customization.
	- Enhanced clarity in documentation for RTC settings and menu customization.
	- Improved organization and clarity of SD card setup instructions for various flashcarts.

- **Refactor**
	- Standardized naming conventions across UI components for better organization.
	- Restructured sound management and input handling for improved responsiveness.
	- Streamlined the loading state management for ROMs and disks within the menu system.
	- Improved clarity and usability of the developer guide and other documentation files.

### Current known Issues
* BETA_SETTING: PAL60 when using HDMI mods has regressed (awaiting libdragon fix).
* The RTC UI requires improvement (awaiting UI developer).
* Menu sound FX may not work properly when a 64 Disk Drive is also attached (work around: turn sound FX off).
* ALPHA_FEATURE: ED64 X Series detection does not occur properly (however this is not a problem as not tag released asset).
* ALPHA_FEATURE: ED64 V Series only supports loading ROMs (however this is not a problem as not tag released asset).

### Breaking changes
* Disk drive expansion ROMs are now loaded with `Z|L` instead of `R` to align with ROM info context menu (and future functionality).
