#!/usr/bin/env python3
"""Summarize explicitly named, independently recorded reset-motion probes."""
import json,math,sys
from pathlib import Path
p=Path(sys.argv[1]); paths=[Path(x) for x in sys.argv[2:]]
if not paths:raise SystemExit('usage: summarize_reset_motion_matrix.py OUTPUT RUN...')
result={'runs':{},'limitations':['GDB forces stage and root z during service; LD_PRELOAD reseeds wind',
 'post-reset zero PWM and debugger timing influence measured dynamics',
 'samples are asynchronous and do not prove full physics, collision or vehicle replay',
 'input sequence is fixed and uses no truth online; its design used prior diagnostic observations']}
for path in paths:
 man=json.loads((path/'manifest.json').read_text());analysis=json.loads((path/'kinematics_analysis.json').read_text());reset=json.loads((path/'reset_analysis.json').read_text())
 for run in man['runs']:
  name='seed%d_repeat%d'%(run['seed'],run['repeat']);a=analysis[name];r=reset[name]
  if 'error' in a:result['runs'][str(path)]={'error':a['error']};continue
  speed=lambda x:math.sqrt(sum(v*v for v in x))
  gens=r.get('generations',[])
  result['runs'][str(path)]={'seed':run['seed'],'stage':man['injected_stage'],'status':run['status'],'reset_success':run.get('reset_response_success'),
   'pre_reset_velocity':a['last_before_reset']['linear_velocity'],'pre_reset_speed':speed(a['last_before_reset']['linear_velocity']),
   'pre_reset_sample_gap_ms':a['sample_before_reset_gap_ms'],'action_max_speed':a['action_max_speed'],
   'post_delay_samples':{k:{'actual_gap_ms':v['actual_gap_ms'],'position_delta':v['position_distance_from_baseline'],
       'velocity_delta':v['velocity_distance_from_baseline']} for k,v in a['post_bridge_delay_samples'].items()},
   'line_counts':[x['line_count'] for x in gens],'line_hashes':[x['line_multiset_sha256'] for x in gens],
   'wind_hashes':[[x['sha256'] for x in g['wind_dumps']] for g in gens]}
p.mkdir(parents=True,exist_ok=False);(p/'summary.json').write_text(json.dumps(result,indent=2)+'\n')
for name,r in result['runs'].items():
 print(name, 'reset',r.get('reset_success'),'pre_speed',round(r.get('pre_reset_speed',0),3), 'post',[(k,round(v['position_delta'],3),round(v['velocity_delta'],3)) for k,v in r.get('post_delay_samples',{}).items()])
