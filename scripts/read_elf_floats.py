#!/usr/bin/env python3
"""Read float32 constants from an ELF64 image by virtual address."""
import argparse
import hashlib
import json
import struct
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('binary', type=Path)
    parser.add_argument('addresses', nargs='+', help='virtual addresses, e.g. 0x6c1228')
    args = parser.parse_args()
    data = args.binary.read_bytes()
    if data[:6] != b'\x7fELF\x02\x01':
        raise ValueError('expected little-endian ELF64')
    phoff = struct.unpack_from('<Q', data, 32)[0]
    entsize, count = struct.unpack_from('<HH', data, 54)
    segments = []
    for i in range(count):
        typ, _, off, va, _, filesz, _, _ = struct.unpack_from('<IIQQQQQQ', data, phoff + i * entsize)
        if typ == 1:
            segments.append((va, va + filesz, off))

    def read(va, size):
        matches = [off + va - start for start, end, off in segments if start <= va and va + size <= end]
        if len(matches) != 1:
            raise ValueError('ambiguous/unmapped address %#x' % va)
        return data[matches[0]:matches[0] + size]

    out = {'file': str(args.binary.resolve()),
           'sha256': hashlib.sha256(data).hexdigest(), 'constants': {}}
    for text in args.addresses:
        address = int(text, 0)
        raw = read(address, 4)
        out['constants'][hex(address)] = {
            'float': struct.unpack('<f', raw)[0],
            'uint32': struct.unpack('<I', raw)[0],
        }
    print(json.dumps(out, indent=2))


if __name__ == '__main__':
    main()
