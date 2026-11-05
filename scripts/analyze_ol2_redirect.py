#!/usr/bin/env python3
"""Check post-travel sensor observations using simulator log and arrival wall clocks."""
import argparse
from collections import Counter
from datetime import datetime, timezone
import json
from pathlib import Path
import re

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('directory', type=Path)
args = parser.parse_args()
manifest = json.loads((args.directory / 'manifest.json').read_text())
pattern = re.compile(r'\[(\d{4}\.\d\d\.\d\d-\d\d\.\d\d\.\d\d:\d+)\].*?LogLoad: Took [^\n]*LoadMap\(/Game/Maps/RMUA2024OL/RMUA2024OL2\)')
result = {}
for run in manifest['runs']:
    name = 'seed%d_repeat%d' % (run['seed'], run['repeat'])
    folder = args.directory / name
    log = (folder / 'gdb.log').read_text(errors='replace')
    timestamps = [int(datetime.strptime(m[1], '%Y.%m.%d-%H.%M.%S:%f').replace(tzinfo=timezone.utc).timestamp()*1e9) for m in pattern.finditer(log)]
    sensor = folder / 'sensor_clock.jsonl'
    rows = [json.loads(line) for line in sensor.read_text().splitlines()] if sensor.exists() else []
    cutoff = timestamps[-1] if len(timestamps) >= 2 else None
    after = [row for row in rows if cutoff is not None and row['arrival_wall_ns'] > cutoff]
    result[name] = {'status': run['status'], 'completed_ol2_loads': len(timestamps),
                    'last_load_log_wall_ns': cutoff, 'sensor_samples': dict(Counter(row['topic'] for row in rows)),
                    'post_load_samples': dict(Counter(row['topic'] for row in after)),
                    'post_load_nonempty_lidar': sum(row['topic']=='lidar' and row.get('data_bytes',0)>0 for row in after),
                    'first_sensor_wall_ns': min((row['arrival_wall_ns'] for row in rows),default=None),
                    'limitations': 'Log clock interpreted as UTC and compared with subscriber wall clock; this checks post-load ordering, not calibrated sensor latency or official competition progression.'}
out = args.directory / 'post_load_analysis.json'
out.write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
