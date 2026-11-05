#!/usr/bin/env python3
"""Summarize recorded reset/hit-source comparisons without inferring object types."""
import json,sys
from pathlib import Path
out=Path(sys.argv[1]);paths=[Path(x) for x in sys.argv[2:]]
if not paths:raise SystemExit('usage: summarize_reset_collision_matrix.py OUTPUT RUN...')
result={'runs':{},'limitations':['event pointers match only within each process and do not identify object type',
 'ordered pairing is descriptive and cannot establish exact physical event identity',
 'timing uses debugger and host monotonic clock rather than lockstep simulation clock',
 'stage, root hold and RNG reseed are development interventions; fixed wait used prior posthoc observations']}
for p in paths:
 man=json.loads((p/'manifest.json').read_text());col=json.loads((p/'collision_replay_analysis.json').read_text());traj=json.loads((p/'trajectory_replay_analysis.json').read_text());reset=json.loads((p/'reset_analysis.json').read_text())
 for run in man['runs']:
  name='seed%d_repeat%d'%(run['seed'],run['repeat']);c=col[name];t=traj[name];r=reset[name]
  ordered=c['ordered_pair_comparison'];result['runs'][str(p)]={'seed':run['seed'],'status':run['status'],'reset_success':run.get('reset_response_success'),
   'hit_counts':{phase:c['phases'][phase]['count'] for phase in ('before','settle','after')},
   'source_counts':{phase:len(c['phases'][phase]['actor_component_pairs']) for phase in ('before','after')},
   'common_source_count':len(c['same_actor_component_pairs']),'ordered_pairs':ordered['count'],
   'same_source_ordered_pairs':sum(x['same_actor_component'] for x in ordered['pairs']),
   'before_unpaired':ordered['before_unpaired'],'after_unpaired':ordered['after_unpaired'],
   'max_ordered_phase_difference_ms':ordered['max_absolute_elapsed_difference_ms'],
   'trajectory_samples':t['matched_samples'],'max_relative_position_difference':t['max_relative_position_difference'],
   'max_velocity_difference':t['max_velocity_difference'],'max_phase_time_disagreement_s':t['max_phase_time_disagreement_s'],
   'pwm_debug_coverage':t.get('pwm_debug_coverage'),'line_counts':[g['line_count'] for g in r['generations']],
   'line_hashes':[g['line_multiset_sha256'] for g in r['generations']],
   'wind_hashes':[[w['sha256'] for w in g['wind_dumps']] for g in r['generations']]}
out.mkdir(parents=True,exist_ok=False);(out/'summary.json').write_text(json.dumps(result,indent=2)+'\n')
for k,v in result['runs'].items():print(k,'hits',v['hit_counts']['before'],v['hit_counts']['after'],'common',v['common_source_count'],'max_ms',round(v['max_ordered_phase_difference_ms'],2) if v['max_ordered_phase_difference_ms'] is not None else None,'pos',round(v['max_relative_position_difference'],3),'vel',round(v['max_velocity_difference'],3))
