#!/usr/bin/env python3
"""Compare each reset generation to the separately seeded float32 wind model."""
import hashlib
import json
from pathlib import Path
import platform
import re
import sys
import wind_offline_model as model

root = Path(sys.argv[1])
manifest = json.loads((root/'manifest.json').read_text())
stage = manifest.get('injected_stage')
params = {4: (200, 350, 2, 1.5), 5: (220, 660, 4, 2), 6: (200, 1800, 15, 4)}[stage]
result = {'experiment_scope': manifest.get('stage_coverage'),
          'model_sha256': hashlib.sha256(Path(model.__file__).read_bytes()).hexdigest(),
          'libc': platform.libc_ver(), 'parameters': params, 'runs': {},
          'limitations': ['reseed and root hold are diagnostic interventions, not native reset semantics',
                          'wind and submission bytes do not prove physics, vehicle or full scene replay',
                          'matching outputs cannot exclude unobserved equivalent random-state operations',
                          'model uses host libc and float32 semantics']}
for run in manifest['runs']:
    key = 'seed%d_repeat%d' % (run['seed'], run['repeat'])
    path = root/key/'gdb.log'
    if not path.exists():
        result['runs'][key] = {'error': 'missing GDB log'}
        continue
    text = path.read_text(errors='replace')
    phases = text.split('INJECT state=')[1:]
    groups = []
    for phase in phases:
        dumps = []
        for idx, raw in re.findall(r'WIND_ITEM\s+(\d+) hex=([0-9a-f]{32})', phase):
            if int(idx) == 0:
                dumps.append([])
            if dumps:
                dumps[-1].append(bytes.fromhex(raw))
        groups.append(dumps[0] if dumps else [])
    expected = model.generate(run['seed'], *params)
    markers = [int(x) for x in re.findall(r'WIND_INTERPOSE_RESEED seed=(\d+)', text)]
    result['runs'][key] = {'status': run.get('status'), 'reset_response_success': run.get('reset_response_success'),
                           'reseed_markers': markers, 'observed_counts': [len(x) for x in groups],
                           'expected_sha256': hashlib.sha256(b''.join(expected)).hexdigest(),
                           'phase_exact': [x == expected and bool(x) for x in groups]}
(root/'reseeded_wind_analysis.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
