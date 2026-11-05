#!/usr/bin/env python3
"""Check a recorded failed reseed against fixed-position libc draws, without seed search."""
import ctypes
import json
from pathlib import Path
import re
import sys

root = Path(sys.argv[1])
manifest = json.loads((root/'manifest.json').read_text())
out = {}
lib = ctypes.CDLL(None)
for run in manifest['runs']:
    key = 'seed%d_repeat%d' % (run['seed'], run['repeat'])
    text = (root/key/'gdb.log').read_text(errors='replace')
    begin = text.find('WIND_RESEED_BEGIN')
    rejoin = text.find('WIND_RESEED_REJOIN', begin)
    window = text[begin:rejoin] if begin >= 0 and rejoin >= 0 else ''
    actual = [int(x) for x in re.findall(r'RNG_RAND_RETURN value=(\d+)', window)]
    srand = [int(x) for x in re.findall(r'RNG_SRAND_ENTRY seed=(\d+)', window)]
    lib.srand(ctypes.c_uint(run['seed']))
    reference = [lib.rand() for _ in range(54+len(actual))]
    out[key] = {'srand_entries_in_window': srand, 'rand_returns_in_window': actual,
                'match_fixed_positions_55_onward': bool(actual) and actual == reference[54:],
                'limitations': 'Only executable PLT entries are traced; debugger stops perturb scheduling. Matching output does not prove absence of hidden RNG calls.'}
(root/'failed_reseed_analysis.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps(out,indent=2))
