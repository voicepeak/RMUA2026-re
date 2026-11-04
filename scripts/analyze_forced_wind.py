#!/usr/bin/env python3
"""Summarize forced wind and passive ROS streams without using debug truth for control."""
import argparse,csv,json,math,statistics
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();m=json.loads((a.directory/'manifest.json').read_text());out={'stage':m['injected_stage'],'runs':{},'limitations':['forced game state at start, not natural outdoor traversal','initially grounded; no PWM commands applied by this probe','ROS timestamp semantics and transport latency not calibrated']}
def csvrows(p):
 if not p.exists() or p.stat().st_size==0:return []
 with p.open() as f:return list(csv.DictReader(f))
for r in m['runs']:
 key='seed%d_repeat%d'%(r['seed'],r['repeat']);d=a.directory/key
 wind=csvrows(d/'before_reset_wind.csv');vectors=[]
 for row in wind:
  try:vectors.append([float(row['field.vector.'+x]) for x in 'xyz'])
  except (KeyError,ValueError):pass
 pwm=[];pwmerr=0
 if (d/'pwm.jsonl').exists():
  for line in (d/'pwm.jsonl').read_text().splitlines():
   o=json.loads(line)
   if 'pwm' in o:pwm.append(o['pwm'])
   else:pwmerr+=1
 clock={}
 if (d/'sensor_clock.jsonl').exists():
  for line in (d/'sensor_clock.jsonl').read_text().splitlines():
   x=json.loads(line);clock[x['topic']]=clock.get(x['topic'],0)+1
 speeds=[math.sqrt(sum(x*x for x in v)) for v in vectors]
 out['runs'][key]={'status':r['status'],'wind_samples':len(vectors),'wind_nonzero':sum(s>1e-9 for s in speeds),'wind_speed_minmax':[min(speeds),max(speeds)] if speeds else None,'wind_first':vectors[0] if vectors else None,'wind_last':vectors[-1] if vectors else None,'imu_rows':len(csvrows(d/'imu.csv')),'gps_rows':len(csvrows(d/'gps.csv')),'posthoc_truth_rows':len(csvrows(d/'pose_gt_posthoc.csv')),'pwm_rows':len(pwm),'pwm_errors':pwmerr,'pwm_first':pwm[0] if pwm else None,'sensor_clock_counts':clock,'exit_codes':{k:v for k,v in r.items() if k.endswith('_exit_code')}}
(a.directory/'analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
