#!/usr/bin/env python3
"""Extract bounded static evidence for the version-specific reset location path."""
import argparse
import hashlib
import json
from pathlib import Path
import subprocess

p = argparse.ArgumentParser(description=__doc__)
p.add_argument('binary', type=Path)
p.add_argument('output', type=Path)
a = p.parse_args()
data = a.binary.read_bytes()
if hashlib.sha256(data).hexdigest() != '5231addfa25ddaa9e4473ea9ac40c9c0bc685174e69cd14bb49159f443b97800':
    p.error('unexpected simulator binary')
if data[0xa8a911-0x200000:0xa8a911-0x200000+11] != b'getpose_ue\0':
    p.error('unexpected command string')
a.output.mkdir(parents=True, exist_ok=False)
ranges = {'command_dispatch': (0x26f0750, 0x26f07c0),
          'location_cache_writer': (0x26f2160, 0x26f2175),
          'event_producer': (0x26302b7, 0x26303b0),
          'event_dispatch': (0x2fce5e0, 0x2fce610)}
for name, (start, stop) in ranges.items():
    output = subprocess.run(['objdump', '-d', '--start-address='+hex(start),
                             '--stop-address='+hex(stop), str(a.binary)],
                            check=True, capture_output=True, text=True).stdout
    (a.output/(name+'.disasm')).write_text(output)
symbols = subprocess.run(['nm', '-D', '-n', '-C', str(a.binary)], check=True,
                         capture_output=True, text=True).stdout.splitlines()
selected = [line for line in symbols if 'vtable for SyncDroneLocationEvent' in line]
if len(selected) != 1:
    raise RuntimeError('expected a unique exported vtable symbol')
(a.output/'event_symbol.txt').write_text('\n'.join(selected)+'\n')
manifest = {'binary_sha256': hashlib.sha256(data).hexdigest(),
            'command_string_vaddr': '0xa8a911', 'command_string': 'getpose_ue',
            'ranges': {key: [hex(v[0]),hex(v[1])] for key,v in ranges.items()},
            'limitations': ['addresses and interpretation apply only to this binary',
                            'static instructions do not establish physical reset or event timing',
                            'vptr matching uses the ABI address-point offset, not independent runtime type metadata']}
(a.output/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
