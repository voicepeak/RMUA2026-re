#!/usr/bin/env python3
"""Summarize explicit state-release runs without merging pointer identities."""
import json,sys
from pathlib import Path
out=Path(sys.argv[1]);paths=[Path(x) for x in sys.argv[2:]]
if not paths:raise SystemExit('usage: summarize_vehicle_release_matrix.py OUTPUT RUN...')
res={'runs':{},'limitations':['state force is released after a fixed timer selected using prior posthoc observations',
 'vehicle path and stage are injected; GDB modifies scheduling and passive samples are not continuous',
 'state field 12 is not proof of score delivery, physical impulse or complete natural-race termination']}
for p in paths:
 man=json.loads((p/'manifest.json').read_text());d=json.loads((p/'release_state_analysis.json').read_text())
 for run in man['runs']:
  key='seed%d_repeat%d'%(run['seed'],run['repeat']);a=d[key]
  if 'error'in a:res['runs'][str(p)]={'error':a['error']};continue
  links=a['vehicle_callback_to_state_write_links']
  res['runs'][str(p)]={'seed':run['seed'],'status':run['status'],'release_state':a['release_state'],
    'vehicle_callback_count':a['post_release_vehicle_callback_count'],'linked_state_writes':len(links),
    'linked_write_values':[(x['following_same_thread_state_write']['previous'],x['following_same_thread_state_write']['current']) for x in links],
    'terminal_samples':a['terminal_sample_count'],'terminal_span_s':a['terminal_sample_span_s'],
    'values_sampled_after_first_terminal':a['sampled_values_after_first_terminal']}
out.mkdir(parents=True,exist_ok=False);(out/'summary.json').write_text(json.dumps(res,indent=2)+'\n')
for k,v in res['runs'].items():print(k,'release',v.get('release_state'),'vehicle',v.get('vehicle_callback_count'),'links',v.get('linked_state_writes'),'values',v.get('linked_write_values'),'terminal_samples',v.get('terminal_samples'),'span',round(v.get('terminal_span_s') or 0,2))
