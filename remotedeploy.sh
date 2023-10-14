#!/bin/bash

set -e

REMOTE="--remote ${REMOTE:-host.docker.internal:9064}"

sc64deployer $REMOTE upload ./output/N64FlashcartMenu.n64

if [ "$1" = "-d" ]; then
    sc64deployer $REMOTE debug --no-writeback
fi

if [ "$1" = "-du" ]; then
    sc64deployer $REMOTE debug --no-writeback --init "send-file /sc64menu.n64 @output/sc64menu.n64@;reboot"
fi
