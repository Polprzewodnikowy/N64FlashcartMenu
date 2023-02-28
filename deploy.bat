@echo off

cd  %~dp0tools\sc64\

:: Make sure we are connected
echo Detecting SC64...
sc64 --print-state

echo:
echo:
:: Load the ROM
echo Loading ROM...
sc64 --boot direct-rom %~dp0build\N64FlashcartMenu.z64

echo:
echo:
:: Toggle the power of the N64 to boot the ROM.
echo !!! Now toggle power to the N64 !!!
echo:
echo:
::pause
