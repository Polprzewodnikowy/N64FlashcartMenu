#!/usr/bin/env python3

import sys
from subprocess import Popen, PIPE


def get_rom_end(elf):
    p1 = Popen(f'readelf -s {elf}'.split(), stdout=PIPE)
    p2 = Popen('grep -m 1 __rom_end'.split(), stdin=p1.stdout, stdout=PIPE)
    stdout, _ = p2.communicate()
    rom_end = int(stdout.decode('UTF-8').split()[1], 16)
    rom_end &= 0x1FFFFFFF
    rom_end -= 0x400
    rom_end += 0x1000
    return rom_end


if __name__ == '__main__':
    if (len(sys.argv) != 4):
        print(f'Usage: python {sys.argv[0]} elf input output')
        sys.exit(1)

    elf_file = sys.argv[1]
    input_file = sys.argv[2]
    output_file = sys.argv[3]

    ALIGN = 512

    rom_end = get_rom_end(elf_file)
    modulo = rom_end % ALIGN
    if (modulo > 0):
        rom_end += (ALIGN - modulo)

    minified_data = b''
    with open(input_file, 'rb') as f:
        minified_data = f.read(rom_end)

    with open(output_file, 'wb') as f:
        f.write(minified_data)
