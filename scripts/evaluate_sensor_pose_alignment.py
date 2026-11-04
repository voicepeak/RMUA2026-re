#!/usr/bin/env python3
"""Offline timestamp-matched sensor vs independent diagnostic truth; never a controller."""
import argparse,bisect,hashlib,json,math,statistics
from pathlib import Path

def read(path):
 rows=[json.loads(line) for line in path.open() if line.strip()]
 if any(not math.isfinite(float(r['stamp'])) for r in rows):raise ValueError('nonfinite source stamp')
 if any(a['stamp']>b['stamp'] for a,b in zip(rows,rows[1:])):raise ValueError('source stamps regress: '+str(path))
 return rows

def digest(path):return hashlib.sha256(path.read_bytes()).hexdigest()
def nearest(rows,ts,stamp,tolerance):
 i=bisect.bisect_left(ts,stamp);valid=[j for j in (i-1,i) if 0<=j<len(rows)]
 if not valid:return None
 j=min(valid,key=lambda k:abs(ts[k]-stamp))
 return rows[j] if abs(ts[j]-stamp)<=tolerance else None

def angle(a,b):
 n=math.sqrt(sum(x*x for x in a)*sum(x*x for x in b))
 if not n:return None
 return 2*math.acos(min(1.,abs(sum(x*y for x,y in zip(a,b))/n)))
def stats(v):
 if not v:return None
 s=sorted(v)
 return {'count':len(v),'mean':statistics.mean(v),'rmse':math.sqrt(statistics.mean(x*x for x in v)),
         'median':statistics.median(v),'p95':s[math.ceil(.95*len(s))-1],'max':max(v)}
p=argparse.ArgumentParser(description=__doc__);p.add_argument('run',type=Path);p.add_argument('--output',type=Path,required=True);p.add_argument('--tolerance',type=float,default=.02);a=p.parse_args()
paths={k:a.run/('evaluation_'+k+'.jsonl') for k in ('gps','imu','truth')}; data={k:read(v) for k,v in paths.items()}; truth=data['truth'];ts=[x['stamp'] for x in truth]
results={};allrows=[]
for kind in ('gps','imu'):
 errors=[];skews=[];axis=[[],[],[]];unmatched=0
 for row in data[kind]:
  tr=nearest(truth,ts,row['stamp'],a.tolerance)
  if tr is None:unmatched+=1;continue
  skew=tr['stamp']-row['stamp'];skews.append(skew)
  if kind=='gps':
   delta=[x-y for x,y in zip(row['position'],tr['position'])];err=math.sqrt(sum(x*x for x in delta))
   for j in range(3):axis[j].append(delta[j])
  else:err=angle(row['imu_orientation_wxyz'],tr['orientation']);delta=None
  if err is None:unmatched+=1;continue
  errors.append(err);allrows.append({'type':kind,'source_stamp':row['stamp'],'truth_stamp':tr['stamp'],'truth_minus_sensor_s':skew,'error_norm':err,'delta_xyz':delta})
 results[kind]={'source_count':len(data[kind]),'matched_count':len(errors),'unmatched_count':unmatched,'signed_truth_minus_sensor_s':stats(skews),'error_norm':stats(errors)}
 if kind=='gps':results[kind]['position_axis_error_m']=[stats(x) for x in axis]
positions=[r['position'] for r in truth]
summary={'inputs_sha256':{k:digest(v) for k,v in paths.items()},'method':'offline nearest truth by recorded source stamp, no interpolation; quaternion wxyz sign-invariant; truth used only after run','tolerance_s':a.tolerance,'results':results,'truth_position_range_m':[[min(x[i] for x in positions),max(x[i] for x in positions)] for i in range(3)],'limitations':['single seed and short closed-loop reference-step flight','source timestamp semantics and frame conventions are not independently calibrated','offline truth matching does not measure online latency or prove safe control','nearest sample mismatch and sensor noise contribute to residual']}
a.output.mkdir(parents=True,exist_ok=False)
(a.output/'summary.json').write_text(json.dumps(summary,indent=2)+'\n')
with (a.output/'matched.jsonl').open('w') as f:
 for r in allrows:f.write(json.dumps(r,separators=(',',':'))+'\n')
print(json.dumps(summary,indent=2))
