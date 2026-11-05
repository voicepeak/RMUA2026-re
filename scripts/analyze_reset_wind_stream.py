#!/usr/bin/env python3
"""Compare false-reset wind arrays to uninterrupted host-libc random stream.

The model consumes a shared libc RNG and therefore must run single-threaded. Exact
matches demonstrate observational agreement, not the absence of hidden writes.
"""
import argparse, hashlib, json, platform, re
from pathlib import Path
import wind_offline_model as model

p = argparse.ArgumentParser(description=__doc__)
p.add_argument('directory', type=Path)
a = p.parse_args()
m = json.loads((a.directory / 'manifest.json').read_text())
result = {'model_sha256': hashlib.sha256(Path(model.__file__).read_bytes()).hexdigest(),
          'libc': platform.libc_ver(), 'parameters': [200, 350, 2, 1.5],
          'assumption': 'srand once, no intervening calls or state modification', 'runs': {}}
original_randunit = model.randunit
counter = [0]
def counted_randunit():
    counter[0] += 1
    return original_randunit()
model.randunit = counted_randunit
for run in m['runs']:
    key = 'seed%d_repeat%d' % (run['seed'], run['repeat'])
    log = (a.directory / key / 'gdb.log').read_text(errors='replace')
    phases = log.split('INJECT state=')[1:]
    observed = []
    for phase in phases:
        groups = []
        for idx, raw in re.findall(r'WIND_ITEM\s+(\d+) hex=([0-9a-f]{32})', phase):
            if int(idx) == 0: groups.append([])
            if groups: groups[-1].append(bytes.fromhex(raw))
        observed.append(groups[0] if groups else [])
    counter[0] = 0
    first = model.generate(run['seed'], 200, 350, 2, 1.5)
    after_first = counter[0]
    second = model.generate(None, 200, 350, 2, 1.5)
    result['runs'][key] = {'reset_response_success': run.get('reset_response_success'),
        'first_draws': after_first, 'second_draws': counter[0] - after_first,
        'observed_counts': [len(x) for x in observed],
        'first_exact': len(observed) > 0 and first == observed[0],
        'second_exact': len(observed) > 1 and second == observed[1],
        'model_hashes': [hashlib.sha256(b''.join(x)).hexdigest() for x in (first, second)]}
result['limitations'] = ['false reset is not successful scene replay',
    'matching output cannot exclude unobserved calls or modifications producing the same stream',
    'libc algorithm and float32 behavior are platform dependent']
(a.directory / 'wind_stream_analysis.json').write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps(result, indent=2))
