#!/usr/bin/env python3
"""Read the selected class strings and constants using ELF64 PT_LOAD mappings."""
import argparse
import hashlib
import json
from pathlib import Path
import struct


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('binary', type=Path)
    args = parser.parse_args()
    data = args.binary.read_bytes()
    if data[:6] != b'\x7fELF\x02\x01':
        raise ValueError('expected little-endian ELF64')
    phoff = struct.unpack_from('<Q', data, 32)[0]
    entsize, count = struct.unpack_from('<HH', data, 54)
    ranges = []
    for i in range(count):
        typ, _, off, va, _, filesz, _, _ = struct.unpack_from('<IIQQQQQQ', data, phoff + i*entsize)
        if typ == 1:
            ranges.append((va, va+filesz, off))

    def read(va, n):
        matches = [off + va-start for start,end,off in ranges if start <= va and va+n <= end]
        if len(matches) != 1:
            raise ValueError('ambiguous/unmapped address %#x' % va)
        return data[matches[0]:matches[0]+n]

    def utf16(va):
        chars = []
        for i in range(128):
            code = read(va+2*i, 2)
            if code == b'\x00\x00': return b''.join(chars).decode('utf-16le')
            chars.append(code)
        raise ValueError('unterminated string')

    addresses = [0x6c0d00, 0x6c0a14, 0x6c0910, 0x6c1394, 0x6c1640,
                 0x6c1438, 0x6c0f1c, 0x6c1b34, 0x6c14d8]
    out = {'file': str(args.binary.resolve()), 'sha256': hashlib.sha256(data).hexdigest(),
           'strings_utf16': {hex(a): utf16(a) for a in (0x91a3a8,0x775d0e,0x7d3fa8)},
           'floats': {hex(a): struct.unpack('<f',read(a,4))[0] for a in addresses}}
    print(json.dumps(out, indent=2))


if __name__ == '__main__':
    main()
