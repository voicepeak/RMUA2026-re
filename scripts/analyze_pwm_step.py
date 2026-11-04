#!/usr/bin/env python3
"""Match fixed PWM command transitions to observed PWM headers; not end-to-end latency."""
import argparse,json
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();manifest=json.loads((a.directory/'manifest.json').read_text());out={}
for run in manifest['runs']:
 key='seed%d_repeat%d'%(run['seed'],run['repeat']);d=a.directory/key
 def read(name):
  path=d/name
  return [json.loads(x) for x in path.read_text().splitlines()] if path.exists() else []
 cmds=read('pwm_commands.jsonl');observed=[x for x in read('pwm.jsonl') if 'pwm' in x];edges=[];last=None
 transitions=[]
 for c in cmds:
  value=c['command'][0]
  if value!=last: transitions.append(c);last=value
 for i,c in enumerate(transitions):
  value=c['command'][0]; end_ns=transitions[i+1]['header_ns'] if i+1<len(transitions) else None
  candidates=[x for x in observed if x['header_stamp_ns']>=c['header_ns'] and (end_ns is None or x['header_stamp_ns']<end_ns) and abs(x['pwm'][0]-value)<1e-4]
  first=candidates[0] if candidates else None
  edges.append({'command':value,'command_header_ns':c['header_ns'],'first_matching_output_header_ns':first['header_stamp_ns'] if first else None,'header_delta_ms':(first['header_stamp_ns']-c['header_ns'])/1e6 if first else None})
 out[key]={'command_samples':len(cmds),'output_samples':len(observed),'edges':edges,'limitations':'First matching output header during each command phase; not calibrated source-to-actuator latency, and no pairing/interpolation between command samples.'}
(a.directory/'pwm_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
