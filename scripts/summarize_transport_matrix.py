#!/usr/bin/env python3
"""Summarize explicitly named transport runs without pooling their distributions."""
import json
from pathlib import Path
import sys
paths=[Path(x) for x in sys.argv[1:]]
if not paths:raise SystemExit('provide explicit run directories')
result={'runs':{},'comparisons':{},'limitations':['per-run quantiles are summarized as ranges, not pooled quantiles',
 'hook instrumentation changes timing; ROS path is not full physical sensor or actuator latency',
 'PWM phase matches are correlational and the initial zero phase is kept even if unmatched']}
step_times=[]
for p in paths:
 manifest=json.loads((p/'manifest.json').read_text());t=json.loads((p/'transport_analysis.json').read_text());phases=json.loads((p/'pwm_phase_transport_analysis.json').read_text())
 if manifest['status']!='observed' or manifest['cleanup_exit_code']!=0:raise ValueError('incomplete run '+str(p))
 result['runs'][str(p)]={'seed':manifest['seed'],'comparisons':t['comparisons'],
                          'clock_offset_check':t.get('wall_minus_monotonic_offset_difference_ms'),
                          'phases':[{'command':x['command'][0],'matching_output_count':x.get('matching_output_count',0),
                                     'callback_to_debug_publish_ms':x.get('callback_to_debug_publish_ms'),
                                     'previous_output_already_matched':x.get('previous_output_already_matched')} for x in phases['phases']]}
 for c,v in t['comparisons'].items():result['comparisons'].setdefault(c,[]).append(v)
 step_times.extend(x['callback_to_debug_publish_ms'] for x in phases['phases'] if x.get('callback_to_debug_publish_ms') is not None)
for c,items in list(result['comparisons'].items()):
 result['comparisons'][c]={'runs':len(items),'matched_min':min(x['unique_matched_stamps'] for x in items),
  'matched_max':max(x['unique_matched_stamps'] for x in items),'median_ms_range':[min(x['median_ms'] for x in items),max(x['median_ms'] for x in items)],
  'p99_ms_range':[min(x['p99_ms'] for x in items),max(x['p99_ms'] for x in items)],
  'negative_delays_total':sum(x['negative_delays'] for x in items)}
result['phase_callback_to_output_ms']={'matched_phases':len(step_times),'min':min(step_times) if step_times else None,'max':max(step_times) if step_times else None}
out=Path('experiments/transport_matrix_20261105');out.mkdir(exist_ok=False)
(out/'summary.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result['comparisons'],indent=2));print(json.dumps(result['phase_callback_to_output_ms']))
