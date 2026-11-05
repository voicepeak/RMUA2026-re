#!/usr/bin/env python3
"""Summarize per-process call overlaps and liveness of an eight-process pilot."""
import json
import sys
from pathlib import Path
p = Path(sys.argv[1])
pool = p / 'batch00_attempt00_pool'
manifest = json.loads((pool / 'pool.json').read_text())
results = json.loads((p / 'results.json').read_text())
liveness = [json.loads(s) for s in (p / 'batch00_attempt00_liveness.jsonl').read_text().splitlines() if s.strip()]
known = {'2b9d00192bd2941a': 'imu', 'd8d4335628b35226': 'frame',
         '29b704a9d5124e35': 'lidar', 'd5235018fae1f0a3': 'gps', 'acffe79470fa10d4': 'rotor_pwm'}
rows = []
for instance in manifest['instances']:
 path = pool / f"frequency-{instance['port']}.jsonl"
 samples = [json.loads(s) for s in path.read_text().splitlines() if s.strip()] if path.exists() else []
 events = []
 for event in samples:
  if event['overlap']:
   events.append(dict(event, candidate_topic=known.get(event['key_hash'])))
 rows.append({'seed': instance['seed'], 'port': instance['port'], 'container': instance['container'],
              'file_exists': path.exists(), 'logged_calls_lower_bound': max((s['calls'] for s in samples), default=0),
              'overlap_counter_lower_bound': max((s['overlaps'] for s in samples), default=0),
              'untracked_counter_lower_bound': max((s['untracked'] for s in samples), default=0),
              'events': events})
summary = {'image': manifest['image'], 'image_id': manifest['image_id'], 'binary_sha256': manifest['binary_sha256'],
           'settings_sha256': manifest['settings_sha256'], 'preload_sha256': manifest.get('frequency_guard_sha256'),
           'run_results': results, 'liveness_samples': len(liveness),
           'liveness_span_s': liveness[-1]['monotonic_s']-liveness[0]['monotonic_s'] if len(liveness)>1 else None,
           'instances': rows,
           'limitations': ['The runner option is called frequency-guard, but this specific preload does not serialize calls.',
                           'Candidate topic is inferred solely from a 64-bit hash and length; collisions are possible.',
                           'A wrapper overlap does not prove simultaneous deque accesses or explain the earlier crash.',
                           'Counter snapshots are lower bounds; brief overlap may be missed and direct accesses are not covered.',
                           'This was an idle, instrumented, finite-duration run, not a training stability test.']}
(p / 'overlap_analysis.json').write_text(json.dumps(summary, indent=2) + '\n')
print(json.dumps(summary, indent=2))
