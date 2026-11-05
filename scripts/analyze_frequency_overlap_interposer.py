#!/usr/bin/env python3
"""Summarize the sampled unguarded frequency-call interposer log."""
import json
import sys
from pathlib import Path
p = Path(sys.argv[1])
manifest = json.loads((p / 'manifest.json').read_text())
rows = [json.loads(line) for line in (p / 'frequency_guard.jsonl').read_text().splitlines() if line.strip()]
events = [row for row in rows if row['overlap']]
result = {'run_status': manifest['status'], 'seed': manifest['seed'], 'configured_seconds': manifest['seconds'],
          'counter_samples': len(rows), 'largest_logged_call_counter': max((x['calls'] for x in rows), default=0),
          'largest_logged_overlap_counter': max((x['overlaps'] for x in rows), default=0),
          'largest_logged_untracked_counter': max((x['untracked'] for x in rows), default=0),
          'logged_overlap_events': len(events), 'overlap_events': events,
          'cleanup_exit_code': manifest.get('cleanup_exit_code'),
          'limitations': ['Sampled counters are lower bounds; the process may terminate before the next periodic snapshot.',
                          'A 64-bit key hash may collide; local nesting is excluded but counter operations can miss short overlaps.',
                          'Wrapper overlap does not establish simultaneous access to a deque or identify the cause of a past crash.',
                          'The probe itself changes timing; direct non-interposed accesses are not counted.']}
(p / 'overlap_analysis.json').write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps(result, indent=2))
