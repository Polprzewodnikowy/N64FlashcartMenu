#!/usr/bin/env python3

import sys
from subprocess import Popen, PIPE


def get_symbol_address(elf, symbol):
    p1 = Popen(f'readelf -s --wide {elf}'.split(), stdout=PIPE)
    p2 = Popen(f'grep -m 1 {symbol}'.split(), stdin=p1.stdout, stdout=PIPE)
    stdout, _ = p2.communicate()

    symbol_data = stdout.decode('UTF-8').split()
    address = symbol_data[1]
    name = symbol_data[7]

    if (symbol != name):
        raise Exception(f'Inexact symbol name found [{symbol} != {name}]')

    return int(address, 16)


def get_rom_data_end_offset(elf):
    ROM_ENTRY_OFFSET = 0x1000

    libdragon_text_start = get_symbol_address(elf, '__libdragon_text_start')
    rom_end = get_symbol_address(elf, '__rom_end')

    return ROM_ENTRY_OFFSET + (rom_end - libdragon_text_start)



if __name__ == '__main__':
    if (len(sys.argv) != 4):
        print(f'Usage: python {sys.argv[0]} elf input output')
        sys.exit(1)

    elf_file = sys.argv[1]
    input_file = sys.argv[2]
    output_file = sys.argv[3]

    ALIGN = 512

    try:
        length = get_rom_data_end_offset(elf_file)
    except Exception as e:
        print(e)
        sys.exit(2)

    stripped_data = b''
    with open(input_file, 'rb') as f:
        stripped_data = f.read(length)

    modulo = (length % ALIGN)
    if (modulo > 0):
        stripped_data += b'\x00' * (ALIGN - modulo)

    with open(output_file, 'wb') as f:
        f.write(stripped_data)
