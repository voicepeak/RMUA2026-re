#!/usr/bin/env python3
"""Summarize sampled deque method wrappers in a single run or pool."""
import json
import sys
from pathlib import Path
p = Path(sys.argv[1])
def summarize(path):
 rows=[json.loads(x) for x in path.read_text().splitlines() if x.strip()] if path.exists() else []
 return {'file_exists':path.exists(), 'samples':len(rows),
         'operations_lower_bound':max((r['operations'] for r in rows),default=0),
         'overlaps_lower_bound':max((r['overlaps'] for r in rows),default=0),
         'untracked_lower_bound':max((r['untracked'] for r in rows),default=0),
         'events':[r for r in rows if r['overlap']]}
if (p/'manifest.json').exists():
 m=json.loads((p/'manifest.json').read_text())
 out={'type':'single','manifest':m,'samples':summarize(p/'frequency_guard.jsonl')}
else:
 pool=p/'batch00_attempt00_pool'
 m=json.loads((pool/'pool.json').read_text())
 live=[json.loads(x) for x in (p/'batch00_attempt00_liveness.jsonl').read_text().splitlines() if x.strip()]
 out={'type':'pool','image':m['image'],'image_id':m['image_id'],
      'binary_sha256':m['binary_sha256'],'settings_sha256':m['settings_sha256'],
      'probe_sha256':m.get('frequency_guard_sha256'),
      'results':json.loads((p/'results.json').read_text()),
      'liveness_samples':len(live),'liveness_span_s':live[-1]['monotonic_s']-live[0]['monotonic_s'] if len(live)>1 else None,
      'instances':[{'seed':i['seed'],'port':i['port'],**summarize(pool/f"frequency-{i['port']}.jsonl")} for i in m['instances']]}
out['limitations']=['Only push_back, pop_front and size calls dynamically bound through the probe inside three frequency functions are counted.',
                    'Exact object address identifies the same object in one process, not topic identity or cross-process equivalence.',
                    'Instrumentation extends wrapper intervals and changes scheduling; overlapping wrappers do not prove overlapping memory instructions.',
                    'Inline operations, iterator dereferences and other call paths are not covered; snapshots are lower bounds and short conflicts can be missed.',
                    'The option called frequency-guard loads this non-serializing development probe; it is not a fix.']
(p/'deque_overlap_analysis.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps(out,indent=2))
