#!/bin/bash

set -e

REMOTE="--remote host.docker.internal:9064"

## FIXME: this does not work!
# Make sure we are connected
#echo Detecting SC64...
#sc64deployer $REMOTE list

# Get the information
echo SC64 info...:
sc64deployer $REMOTE info
echo
echo

# Load the ROM
echo Loading ROM...:
sc64deployer $REMOTE upload ./output/N64FlashcartMenu.z64

echo
echo
# Toggle the power of the N64 to boot the ROM.
echo !!! Now toggle power to the N64 !!!
echo
echo

if [ "$1" = "-d" ]; then
    sc64deployer $REMOTE debug
fi
