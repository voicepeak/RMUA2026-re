#!/usr/bin/env python3
"""Summarize captured sensor values without using them for control."""
import csv
import json
import math
from pathlib import Path
import sys

run = Path(sys.argv[1])
result = {}
for name in ('imu_samples', 'gps_samples', 'pose_posthoc'):
    path = run / (name + '.csv')
    if not path.exists():
        result[name] = {'error': 'file missing'}
        continue
    with path.open() as stream:
        rows = list(csv.DictReader(stream))
    prefix = 'field.orientation.' if name == 'imu_samples' else 'field.pose.orientation.'
    qnorm = [math.sqrt(sum(float(row[prefix + axis]) ** 2 for axis in 'xyzw')) for row in rows]
    item = {'count': len(rows), 'quaternion_norm_min': min(qnorm, default=None),
            'quaternion_norm_max': max(qnorm, default=None)}
    if name != 'imu_samples':
        item['position_range'] = {}
        for axis in 'xyz':
            values = [float(row['field.pose.position.' + axis]) for row in rows]
            item['position_range'][axis] = [min(values, default=None), max(values, default=None)]
    result[name] = item
result['limitations'] = 'Posthoc summary of one stationary diagnostic scene, not a calibration or proof of sensor origin; zero position alone is not an error.'
(run / 'sample_analysis.json').write_text(json.dumps(result, indent=2)+'\n')
print(json.dumps(result, indent=2))
