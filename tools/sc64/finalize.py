#!/usr/bin/env python3

import sys



if __name__ == '__main__':
    if (len(sys.argv) != 2):
        print(f'Usage: python {sys.argv[0]} input')
        sys.exit(1)

    ALIGN = 512
    STRIP_BYTE = b'\x00'

    input_file = sys.argv[1]
    sc64_output = 'sc64menu.n64'

    try:
        bin_data = b''

        with open(input_file, 'rb') as f:
            bin_data = f.read()

        bin_data = bin_data.strip(STRIP_BYTE)
        modulo = len(bin_data) % ALIGN
        if (modulo > 0):
            bin_data += STRIP_BYTE * (ALIGN - modulo)

        with open(sc64_output, 'wb') as f:
            f.write(bin_data)

    except FileNotFoundError as e:
        print(f'Couldn\'t open file: {e}')
        sys.exit(2)

    except Exception as e:
        print(e)
        sys.exit(3)
