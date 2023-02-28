:: Make sure we are connected
cd  %~dp0tools\sc64\
sc64 --print-state

:: Boot the menu
sc64 --boot direct-rom %~dp0build\N64FlashcartMenu.z64

::pause
