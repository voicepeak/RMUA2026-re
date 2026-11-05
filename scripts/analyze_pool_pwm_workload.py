#!/usr/bin/env python3
"""Summarize a fixed-PWM pool workload, keeping action/observer overlap explicit."""
import json,sys
from pathlib import Path
p=Path(sys.argv[1]);state=json.loads((p/'workload_manifest.json').read_text());pool=json.loads((p/'pool/pool.json').read_text());out={'status':state['status'],'requested_workload_s':state['requested_workload_s'],'instances':{},'limitations':state['limitations']+['published commands are not proof of actuator application','successful finite observation does not establish a universal stable concurrency limit']}
for item in pool['instances']:
 name=item['container'];act=p/(name+'.commands.jsonl');obs=p/'stream'/(name+'.jsonl')
 actions=[json.loads(x) for x in act.read_text().splitlines() if x.strip()] if act.exists() else []
 sensors=[json.loads(x) for x in obs.read_text().splitlines() if x.strip()] if obs.exists() else []
 started=next((x for x in actions if x.get('event')=='started'),None);finished=next((x for x in actions if x.get('event')=='finished'),None)
 commands=[x for x in actions if x.get('event')=='command'];topics={}
 for topic in ('imu','lidar'):
  data=[x for x in sensors if x.get('event')=='sample' and x.get('topic')==topic]
  if len(data)>1:
   span=(data[-1]['monotonic_ns']-data[0]['monotonic_ns'])/1e9
   overlap=(max(0,min(data[-1]['monotonic_ns'],finished['mono_ns'])-max(data[0]['monotonic_ns'],started['mono_ns']))/1e9
            if started and finished else None)
   stamps=[b['stamp_ns']-a['stamp_ns'] for a,b in zip(data,data[1:])]
   topics[topic]={'samples':len(data),'span_s':span,'arrival_hz':(len(data)-1)/span,'overlap_with_publisher_s':overlap,
    'zero_stamps':sum(x['stamp_ns']==0 for x in data),'equal_adjacent':sum(x==0 for x in stamps),'regressions':sum(x<0 for x in stamps),
    'nonzero_stamp_regressions':sum(b['stamp_ns']<a['stamp_ns'] for a,b in zip(data,data[1:]) if a['stamp_ns'] and b['stamp_ns'])}
  else:topics[topic]={'samples':len(data)}
 out['instances'][name]={'seed':item['seed'],'publisher_ready':next((x for x in actions if x.get('event')=='publisher_ready'),None),
  'started':started,'finished':finished,'command_count':len(commands),'command_values':sorted({x['value'] for x in commands}),
  'publisher_exit_code':state.get('publisher_exit_codes',{}).get(name),'topics':topics}
(p/'workload_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
