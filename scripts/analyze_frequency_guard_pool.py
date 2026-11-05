#!/usr/bin/env python3
"""Analyze a completed pool batch with or without the frequency guard."""
import json,sys
from pathlib import Path
p=Path(sys.argv[1]);result=json.loads((p/'results.json').read_text());attempt=result['batches'][0]['attempts'][0]
pool=p/'batch00_attempt00_pool';manifest=json.loads((pool/'pool.json').read_text())
live=p/'batch00_attempt00_liveness.jsonl';rows=[json.loads(x) for x in live.read_text().splitlines() if x.strip()] if live.exists() else []
summary={'batch_success':result['batches'][0]['success'],'attempt':attempt,'requested_wall_s':result['seconds_wall'],
 'frequency_guard':result.get('frequency_guard'),'liveness_samples':len(rows),
 'liveness_span_s':rows[-1]['monotonic_s']-rows[0]['monotonic_s'] if len(rows)>1 else None,
 'instances':{},'limitations':['idle NullRHI with separate ROS masters; not a full training workload',
  'one run, finite wall-clock observation; cannot establish stability probability',
  'guard serializes three exported frequency functions only and changes scheduling; unknown direct accesses remain unprotected',
  'guard counters are periodic snapshots and are lower bounds, not final total calls or contention',
  'failure stack and root cause are not established without separate crash capture']}
for item in manifest['instances']:
 name=item['container'];states=[x['containers'].get(name,{}) for x in rows];log=pool/('frequency-%d.jsonl'%item['port'])
 records=[json.loads(x) for x in log.read_text().splitlines() if x.strip()] if log.exists() else []
 summary['instances'][name]={'seed':item['seed'],'port':item['port'],'liveness_samples':len(states),
  'running_samples':sum(bool(x.get('Running')) for x in states),'nonrunning_states':[x for x in states if not x.get('Running')],
  'guard_snapshots':len(records),'last_guard_snapshot':records[-1] if records else None}
(p/'guard_analysis.json').write_text(json.dumps(summary,indent=2)+'\n');print(json.dumps(summary,indent=2))
