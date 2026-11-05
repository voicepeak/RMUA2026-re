#!/usr/bin/env python3
"""Approximate wind-change and fixed PWM phase overlap using recorded ROS headers."""
import argparse,bisect,csv,json,math
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();m=json.loads((a.directory/'manifest.json').read_text());out={}
for run in m['runs']:
 key='seed%d_repeat%d'%(run['seed'],run['repeat']);d=a.directory/key
 try:
  commands=[json.loads(x) for x in (d/'pwm_commands.jsonl').read_text().splitlines()]
  with (d/'before_reset_wind.csv').open() as f:wind=list(csv.DictReader(f))
 except FileNotFoundError:out[key]={'error':'missing commands or wind'};continue
 times=[x['header_ns'] for x in commands];previous=None;changes=0;overlap={};first=None;last=None
 for r in wind:
  t=int(r['%time']);v=tuple(float(r['field.vector.'+x]) for x in 'xyz');i=bisect.bisect_right(times,t)-1
  if previous is not None and math.dist(v,previous)>1e-6:
   changes+=1
   if first is None:first=t
   last=t
   if i>=0 and t<=times[-1]:
    phase=str(commands[i]['command'][0]);overlap[phase]=overlap.get(phase,0)+1
  previous=v
 out[key]={'command_samples':len(commands),'wind_samples':len(wind),'wind_change_samples':changes,'change_samples_by_latest_command':overlap,'first_change_header_ns':first,'last_change_header_ns':last,'limitations':'Header-domain nearest previous command is an approximate alignment, bounded by the last recorded command; wind change is thresholded and observations are under repeated state injection, not a causal or calibrated delay measurement.'}
(a.directory/'overlap_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
