#!/usr/bin/env python3
"""Compare fixed command phases using only sender-return and subscriber arrival."""
import json
from pathlib import Path
import sys

paths=[Path(x) for x in sys.argv[1:]]
if not paths:raise SystemExit('provide explicit directories')
result={'runs':{},'limitations':['sender time follows rospy publish return; subscriber time is callback arrival',
 'output match is correlational, not proof of actuation or physical response',
 'observers and repeated commands alter scheduling; small unpaired samples do not establish instrumentation causality']}
for p in paths:
 manifest=json.loads((p/'manifest.json').read_text())
 commands=[json.loads(x) for x in (p/'commands.jsonl').read_text().splitlines()]
 output=[json.loads(x) for x in (p/'pwm_output.jsonl').read_text().splitlines()]
 output=[x for x in output if len(x.get('pwm',[]))==4]
 phases=[]
 for c in commands:
  if not phases or c['command']!=phases[-1]['command']:phases.append({'command':c['command'],'commands':[]})
  phases[-1]['commands'].append(c)
 for i,phase in enumerate(phases):
  cs=phase.pop('commands');start=cs[0]['mono_ns'];end=phases[i+1]['commands'][0]['mono_ns'] if i+1<len(phases) else cs[-1]['mono_ns']
  matching=[o for o in output if start<=o['arrival_monotonic_ns']<end and all(abs(o['pwm'][j]-phase['command'][j])<1e-4 for j in range(4))]
  previous=next((x for x in reversed(output) if x['arrival_monotonic_ns']<start),None)
  phase.update({'start_mono_ns':start,'end_mono_ns':end,'matching_samples':len(matching),
   'first_match_after_sender_return_ms':(matching[0]['arrival_monotonic_ns']-start)/1e6 if matching else None,
   'previous_output_already_matched':bool(previous and all(abs(previous['pwm'][j]-phase['command'][j])<1e-4 for j in range(4)))})
 result['runs'][str(p)]={'seed':manifest['seed'],'probe_enabled':manifest.get('probe_enabled',True),'status':manifest['status'],'phases':phases}
out=Path('experiments/transport_control_comparison_20261105');out.mkdir(exist_ok=False)
(out/'summary.json').write_text(json.dumps(result,indent=2)+'\n')
for name,run in result['runs'].items():
 print(name,'probe',run['probe_enabled'],[(x['command'][0],x['matching_samples'],round(x['first_match_after_sender_return_ms'],2) if x['first_match_after_sender_return_ms'] is not None else None) for x in run['phases']])
