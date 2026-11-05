#!/usr/bin/env python3
"""Summarize explicit trajectory replay runs, retaining unmatched/failed phases."""
import json,sys
from pathlib import Path
out=Path(sys.argv[1]);paths=[Path(x) for x in sys.argv[2:]]
if not paths:raise SystemExit('usage: summarize_reset_trajectory_matrix.py OUTPUT RUN...')
result={'runs':{},'limitations':['asynchronous nearest samples are matched in host time, not lockstep simulation time',
 'recorded PWM samples do not prove continuous actuator input; debug truth is posthoc only',
 'fixed wait was informed by prior diagnostics; no truth used for online policy or reset trigger',
 'development root hold, stage injection and wind reseed change execution; no full scene replay claim']}
for p in paths:
 m=json.loads((p/'manifest.json').read_text());a=json.loads((p/'trajectory_replay_analysis.json').read_text());reset=json.loads((p/'reset_analysis.json').read_text())
 for r in m['runs']:
  name='seed%d_repeat%d'%(r['seed'],r['repeat']);x=a[name];v=reset[name]
  if 'error' in x:result['runs'][str(p)]={'error':x['error']};continue
  result['runs'][str(p)]={'seed':r['seed'],'status':r['status'],'reset_success':r.get('reset_response_success'),
   'matched_samples':x['matched_samples'],'max_relative_position_difference':x['max_relative_position_difference'],
   'max_velocity_difference':x['max_velocity_difference'],'max_phase_time_disagreement_s':x['max_phase_time_disagreement_s'],
   'pwm_debug_coverage':x.get('pwm_debug_coverage'),'command_counts':x['command_counts'],
   'line_counts':[g['line_count'] for g in v['generations']],'line_hashes':[g['line_multiset_sha256'] for g in v['generations']],
   'wind_hashes':[[w['sha256'] for w in g['wind_dumps']] for g in v['generations']]}
out.mkdir(parents=True,exist_ok=False);(out/'summary.json').write_text(json.dumps(result,indent=2)+'\n')
for k,v in result['runs'].items():
 print(k,'success',v.get('reset_success'),'pairs',v.get('matched_samples'),'pos_max',round(v.get('max_relative_position_difference') or 0,4),'vel_max',round(v.get('max_velocity_difference') or 0,4),'pwm',v.get('pwm_debug_coverage'))
