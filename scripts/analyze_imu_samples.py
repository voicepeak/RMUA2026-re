#!/usr/bin/env python3
"""Summarize observed IMU components; variation includes physics and timing, not only noise."""
import argparse,csv,json,statistics
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();m=json.loads((a.directory/'manifest.json').read_text());out={}
for r in m['runs']:
 key='seed%d_repeat%d'%(r['seed'],r['repeat']);path=a.directory/key/'imu.csv'
 if not path.exists():continue
 with path.open() as f:rows=list(csv.DictReader(f))
 vals={}
 for name in ('angular_velocity','linear_acceleration'):
  vals[name]={}
  for axis in 'xyz':
   k='field.'+name+'.'+axis;v=[float(x[k]) for x in rows if k in x]
   vals[name][axis]={'mean':statistics.mean(v) if v else None,'stddev':statistics.pstdev(v) if v else None,'min':min(v) if v else None,'max':max(v) if v else None}
 out[key]={'count':len(rows),'components':vals,'limitation':'Observed variation includes vehicle motion, wind and sensor effects; not a calibrated isolated noise model.'}
(a.directory/'imu_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
