#!/usr/bin/env python3
"""Audit the diagnostic root hold, reset response, and post-release observations."""
import csv
import json
import re
from pathlib import Path
import sys

root = Path(sys.argv[1])
manifest = json.loads((root / 'manifest.json').read_text())
out = {}
for run in manifest['runs']:
    key = 'seed%d_repeat%d' % (run['seed'], run['repeat'])
    directory = root / key
    log = (directory / 'gdb.log').read_text(errors='replace')
    hold = re.findall(r'RESET_ROOT_HOLD_END count=(\d+)', log)
    poses = re.findall(r'RESET_BRIDGE_POSE mono_ns=(\d+) values=([^\s]+)', log)
    item = {'status': run.get('status'), 'service_success': run.get('reset_response_success'),
            'hold_counts': [int(x) for x in hold],
            'bridge_poses': [{'mono_ns': int(t), 'values': [float(x) for x in values.split(',')]}
                             for t, values in poses]}
    path = directory / 'pose_posthoc_after.csv'
    if path.exists():
        with path.open() as stream:
            rows = list(csv.DictReader(stream))
        item['posthoc_pose_count'] = len(rows)
        item['posthoc_position_z_m'] = {}
        values = [float(row['field.pose.position.z']) for row in rows]
        if values:
            item['posthoc_position_z_m'] = {'first': values[0], 'last': values[-1],
                                            'min': min(values), 'max': max(values)}
    out[key] = item
out['_limitations'] = ['root world z is overwritten during the ROS reset callback, affecting the physical scene and pose check',
                       'posthoc debug pose is not used for control and is not an independent physics validation',
                       'a true ROS response and matching spawn submissions do not establish complete scene or dynamics replay']
(root / 'root_hold_analysis.json').write_text(json.dumps(out, indent=2)+'\n')
print(json.dumps(out, indent=2))
