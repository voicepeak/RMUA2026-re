#!/usr/bin/env python3
"""Compare posthoc sampled poses around a diagnostic reset; no control use."""
import csv
import json
import math
from pathlib import Path
import sys

root = Path(sys.argv[1])
manifest = json.loads((root/'manifest.json').read_text())
result = {}
for run in manifest['runs']:
    key = 'seed%d_repeat%d' % (run['seed'], run['repeat'])
    phases = {}
    for phase in ('before', 'after'):
        path = root/key/('pose_posthoc_'+phase+'.csv')
        if not path.exists():
            phases[phase] = {'error': 'missing pose file'}
            continue
        with path.open() as stream:
            rows = list(csv.DictReader(stream))
        xyz = [[float(row['field.pose.position.'+axis]) for axis in 'xyz'] for row in rows]
        norms = [math.sqrt(sum(float(row['field.pose.orientation.'+axis])**2 for axis in 'xyzw')) for row in rows]
        phases[phase] = {'count': len(rows),
                         'position_first': xyz[0] if xyz else None,
                         'position_last': xyz[-1] if xyz else None,
                         'position_min': [min(row[i] for row in xyz) for i in range(3)] if xyz else None,
                         'position_max': [max(row[i] for row in xyz) for i in range(3)] if xyz else None,
                         'quaternion_norm_min': min(norms, default=None),
                         'quaternion_norm_max': max(norms, default=None)}
    before = phases['before'].get('position_last')
    after = phases['after'].get('position_first')
    result[key] = {'phases': phases,
                   'sampled_position_delta_m': math.dist(before, after) if before is not None and after is not None else None}
result['_limitations'] = ['debug pose is used only posthoc; samples are separated in time',
                          'stationary pose similarity is not a reset of velocity, contacts, dynamics or entire scene',
                          'root hold, wind reseed and forced stage are development interventions']
(root/'pose_prepost_analysis.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
