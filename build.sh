#!/bin/bash

set -e

if [ "$1" = "-c" ]; then
    libdragon make clean
fi

libdragon make -j all

pushd ./build > /dev/null
../tools/finalize.py N64FlashcartMenu.z64
popd > /dev/null
