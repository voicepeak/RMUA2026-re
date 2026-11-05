#!/usr/bin/env python3
"""Correlate fixed PWM phases with uniquely matched command and output hook entries."""
from collections import Counter
import json
from pathlib import Path
import sys
p=Path(sys.argv[1])
def rows(name):return [json.loads(x) for x in (p/name).read_text().splitlines() if x.strip()]
cmd=rows('commands.jsonl');output=rows('pwm_output.jsonl');hooks=rows('publish.jsonl')

def key(x):return x.get('sec',0)*1000000000+x.get('nsec',0)
def unique_hooks(kind):
 data=[x for x in hooks if x.get('type')==kind and key(x)>0];counts=Counter(key(x) for x in data)
 return {key(x):x for x in data if counts[key(x)]==1}
cb=unique_hooks('rotor_pwm_command_callback');pub=unique_hooks('rotor_pwm')
out_counts=Counter(x.get('header_stamp_ns') for x in output if x.get('header_stamp_ns'))
observed=[]
for x in output:
 k=x.get('header_stamp_ns');h=pub.get(k)
 if h and out_counts[k]==1 and len(x.get('pwm',[]))==4:observed.append({'t':h['mono_ns'],'pwm':x['pwm'],'stamp_ns':k})
observed.sort(key=lambda x:x['t'])
phases=[]
for c in cmd:
 if not phases or c['command']!=phases[-1]['command']:
  phases.append({'command':c['command'],'commands':[]})
 phases[-1]['commands'].append(c)
for i,phase in enumerate(phases):
 cs=phase.pop('commands'); phase['sent_commands']=len(cs);phase['phase_start_mono_ns']=cs[0]['mono_ns'];phase['phase_end_mono_ns']=phases[i+1]['commands'][0]['mono_ns'] if i+1<len(phases) else cs[-1]['mono_ns']
 available=[(c,cb[c['header_ns']]) for c in cs if c.get('header_ns') in cb]
 phase['matched_command_callbacks']=len(available)
 if not available:continue
 c,h=available[0]; t=h['mono_ns'];phase['first_command_send_return_ns']=c['mono_ns'];phase['first_command_callback_ns']=t
 preceding=next((x for x in reversed(observed) if x['t']<t),None)
 candidates=[x for x in observed if t<=x['t']<phase['phase_end_mono_ns'] and all(abs(x['pwm'][j]-phase['command'][j])<1e-4 for j in range(4))]
 phase['previous_debug_pwm']=preceding
 phase['matching_output_count']=len(candidates)
 if candidates:
  first=candidates[0];phase['first_matching_output']=first
  phase['callback_to_debug_publish_ms']=(first['t']-t)/1e6
  phase['previous_output_already_matched']=bool(preceding and all(abs(preceding['pwm'][j]-phase['command'][j])<1e-4 for j in range(4)))
result={'phases':phases,'limitations':['one fixed open-loop sequence; correlation does not prove causality',
 'source/observer records are paired by unique nonzero header stamp; first command may have been missed',
 'callback entry to first matching ROS debug publication is not motor or vehicle physical response',
 'first matching output is restricted to phase and prior output is shown to detect already-matched values',
 'LD_PRELOAD and ROS observers change timing; no universal latency claim']}
(p/'pwm_phase_transport_analysis.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
