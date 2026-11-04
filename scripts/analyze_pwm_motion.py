#!/usr/bin/env python3
"""Posthoc position ranges from debug pose; never used as control input."""
import argparse,csv,json
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();m=json.loads((a.directory/'manifest.json').read_text());out={}
for run in m['runs']:
 key='seed%d_repeat%d'%(run['seed'],run['repeat']);p=a.directory/key/'pose_gt_posthoc.csv'
 if not p.exists():continue
 with p.open() as f:rows=list(csv.DictReader(f))
 axes={}
 for axis in 'xyz':
  keyname='field.pose.position.'+axis;v=[float(r[keyname]) for r in rows if keyname in r]
  axes[axis]={'first':v[0] if v else None,'last':v[-1] if v else None,'min':min(v) if v else None,'max':max(v) if v else None}
 out[key]={'samples':len(rows),'position_ned_m':axes,'limitation':'Posthoc debug pose only; forced wind/state and fixed PWM sequence; no claim of open-loop identification or calibrated latency.'}
(a.directory/'motion_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
