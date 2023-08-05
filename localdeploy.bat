@echo off

:: Make sure we are connected
echo Detecting SC64...
%~dp0tools\sc64\sc64deployer list

:: Get the information
echo SC64 info...
%~dp0tools\sc64\sc64deployer info

echo:
echo:
:: Load the ROM
echo Loading ROM...
%~dp0tools\sc64\sc64deployer upload %~dp0output\N64FlashcartMenu.n64

echo:
echo:
:: Toggle the power of the N64 to boot the ROM.
echo !!! Now toggle power to the N64 !!!
echo:
echo:

if not "%1" == "/d" goto :exit

%~dp0tools\sc64\sc64deployer debug --no-writeback

:exit
