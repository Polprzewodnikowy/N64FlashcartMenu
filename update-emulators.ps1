$ErrorActionPreference = "Stop"

$emulatorDir = "filesystem/menu/emulators"
New-Item -ItemType Directory -Path $emulatorDir -Force | Out-Null

# Get neon64
$neon64Url = "https://github.com/hcs64/neon64v2/releases/download/v2.0-beta.4/neon64v2b4.zip"
$neon64Zip = "$env:TEMP/neon64v2b4.zip"
Invoke-WebRequest -Uri $neon64Url -OutFile $neon64Zip
Expand-Archive -Path $neon64Zip -DestinationPath $env:TEMP/neon64v2b4 -Force
Copy-Item -Path (Get-ChildItem -Path "$env:TEMP/neon64v2b4" -Recurse -Filter "neon64bu.rom").FullName -Destination $emulatorDir -Force

# Get sodium64
$sodium64Url = "https://github.com/Hydr8gon/sodium64/releases/download/release/sodium64.zip"
$sodium64Zip = "$env:TEMP/sodium64.zip"
Invoke-WebRequest -Uri $sodium64Url -OutFile $sodium64Zip
Expand-Archive -Path $sodium64Zip -DestinationPath $env:TEMP/sodium64 -Force
Copy-Item -Path (Get-ChildItem -Path "$env:TEMP/sodium64" -Recurse -Filter "sodium64.z64").FullName -Destination $emulatorDir -Force

# Get smsPlus64
$smsplus64Url = "https://github.com/fhoedemakers/smsplus64/releases/download/v0.7/smsPlus64.z64"
Invoke-WebRequest -Uri $smsplus64Url -OutFile "$emulatorDir/smsplus64.z64"

# Get Press-F Ultra
$pressfUrl = "https://github.com/celerizer/Press-F-Ultra/releases/download/r4/Press-F.z64"
Invoke-WebRequest -Uri $pressfUrl -OutFile "$emulatorDir/pressf.z64"

# Get gb64
# Note: The gb64 emulator requires manual download and extraction of gb.v64 and gbc.v64 files.
# The following source provides the ability to download the emulator and ROM files:
# https://github.com/lambertjamesd/gb64/blob/gh-pages/romwrapper/romwrapper.html
# we now need to work out how to download the files automatically using the `downloadEV()` event.
# Currently we need to manually open: https://lambertjamesd.github.io/gb64/romwrapper/romwrapper.html" and click the "Download Emulator" button to get the gb.v64 and gbc.v64 files.

Write-Host "All emulators downloaded and placed in $emulatorDir."

# # Delete .gitkeep file if it exists as the directory is now populated
# $gitkeep = Join-Path $emulatorDir ".gitkeep"
# if (Test-Path $gitkeep) {
#     Remove-Item $gitkeep
# }
