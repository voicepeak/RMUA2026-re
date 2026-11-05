#!/usr/bin/env python3
"""Align change-only GDB snapshots to commands using the shared monotonic clock."""
import bisect
import json
import math
from pathlib import Path
import re
import sys

root = Path(sys.argv[1])
manifest = json.loads((root / 'manifest.json').read_text())
pattern = re.compile(r'PWM_CHAIN label=(\w+) mono_ns=(\d+) wall_ns=(\d+) mode=(\d+) values=([^\s]+)')
result = {}
for run in manifest['runs']:
    name = 'seed%d_repeat%d' % (run['seed'], run['repeat'])
    folder = root / name
    if not (folder / 'pwm_commands.jsonl').exists():
        result[name] = {'error': 'no command stream', 'run_status': run.get('status')}
        continue
    commands = [json.loads(x) for x in (folder / 'pwm_commands.jsonl').read_text().splitlines()]
    if not commands:
        result[name] = {'error': 'empty command stream', 'run_status': run.get('status')}
        continue
    changes = [x for i, x in enumerate(commands) if not i or x['command'] != commands[i-1]['command']]
    starts = [x['mono_ns'] for x in changes]
    phases = [{'command': c['command'][0], 'snapshots': {'dispatch': 0, 'scheduled': 0},
               'matching_snapshots': {'dispatch': 0, 'scheduled': 0},
               'first_matching_offset_ms': {'dispatch': None, 'scheduled': None},
               'other_snapshots': {'dispatch': 0, 'scheduled': 0}} for c in changes]
    for m in pattern.finditer((folder / 'gdb.log').read_text(errors='replace')):
        label, ns = m[1], int(m[2])
        i = bisect.bisect_right(starts, ns) - 1
        if label not in ('dispatch', 'scheduled') or i < 0 or ns > commands[-1]['mono_ns']:
            continue
        try:
            values = [float(x) for x in m[5].split(',')]
        except ValueError:
            continue
        phase = phases[i]
        phase['snapshots'][label] += 1
        if len(values) == 4 and all(math.isfinite(v) and abs(v-phase['command']) < 1e-4 for v in values):
            phase['matching_snapshots'][label] += 1
            if phase['first_matching_offset_ms'][label] is None:
                phase['first_matching_offset_ms'][label] = (ns-starts[i])/1e6
        else:
            phase['other_snapshots'][label] += 1
    result[name] = {'phases': phases, 'limitations': 'Change-only snapshots, not call counts; source and target may run on different threads. All-stop GDB perturbs scheduling. Monotonic timestamps are sampled inside the debugger and publisher, not at hardware/physical actuation. Other snapshots are raw observed words, not independently typed or attributed.'}
(root / 'chain_analysis.json').write_text(json.dumps(result, indent=2)+'\n')
print(json.dumps(result, indent=2))
