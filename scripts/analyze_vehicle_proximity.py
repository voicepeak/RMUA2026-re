#!/usr/bin/env python3
"""Posthoc nearest-time component-origin comparisons and hit-source identity."""
import bisect
from collections import Counter
import json
import math
from pathlib import Path
import re
import sys

root = Path(sys.argv[1])
manifest = json.loads((root/'manifest.json').read_text())
record = re.compile(r'(DRONE_WORLD|VEHICLE_WORLD) mono=([\d.]+) actor=(0x[0-9a-f]+) component=(0x[0-9a-f]+) xyz_cm=([^\s]+) quat=([^\s]+)')
hit = re.compile(r'HIT_SOURCE mono=([\d.]+) drone=(0x[0-9a-f]+) drone_component=(0x[0-9a-f]+) drone_world_cm=([^\s]+) source_actor=(0x[0-9a-f]+) source_component=(0x[0-9a-f]+)')
result = {}
for run in manifest['runs']:
    name = 'seed%d_repeat%d' % (run['seed'], run['repeat'])
    log = (root/name/'gdb.log').read_text(errors='replace')
    tracks = {'DRONE_WORLD': [], 'VEHICLE_WORLD': []}
    for m in record.finditer(log):
        tracks[m[1]].append({'mono_s': float(m[2]), 'actor': m[3], 'component': m[4],
                             'xyz_cm': [float(x) for x in m[5].split(',')],
                             'quat': [float(x) for x in m[6].split(',')]})
    drones = sorted(tracks['DRONE_WORLD'],key=lambda x:x['mono_s'])
    vehicles = sorted(tracks['VEHICLE_WORLD'],key=lambda x:x['mono_s'])
    times = [v['mono_s'] for v in vehicles]
    aligned = []
    for d in drones:
        i = bisect.bisect_left(times,d['mono_s'])
        options = [vehicles[j] for j in (i-1,i) if 0 <= j < len(vehicles)]
        if not options:
            continue
        v = min(options,key=lambda x:abs(x['mono_s']-d['mono_s']))
        gap = abs(v['mono_s']-d['mono_s'])
        if gap <= 0.2:
            aligned.append({'drone': d, 'vehicle': v, 'time_gap_s': gap,
                            'origin_distance_cm': math.dist(d['xyz_cm'],v['xyz_cm'])})
    hits = list(hit.finditer(log))
    vehicle_actors = {v['actor'] for v in vehicles}
    vehicle_components = {v['component'] for v in vehicles}
    sources = Counter((m[5],m[6]) for m in hits)
    nearest = min(aligned,key=lambda x:x['origin_distance_cm']) if aligned else None
    result[name] = {'status': run.get('status'), 'drone_samples': len(drones),
                    'vehicle_samples': len(vehicles), 'matched_within_0_2s': len(aligned),
                    'drone_actors': sorted({d['actor'] for d in drones}),
                    'hit_drone_actors': sorted({m[2] for m in hits}),
                    'closest_sample': nearest,
                    'hit_count': len(hits), 'hit_sources': [{'actor': k[0], 'component': k[1], 'count': n} for k,n in sources.items()],
                    'hit_matching_vehicle_actor_or_sampled_component': sum(m[5] in vehicle_actors or m[6] in vehicle_components for m in hits),
                    'limitations': ['component origins are not collision shapes or contact points; transforms and child components are not fully resolved',
                                    'nearest samples permit up to 0.2 s separation; sampling may miss brief encounters',
                                    'all tracks are posthoc; debugger and forced path/state alter behavior']}
(root/'proximity_analysis.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
