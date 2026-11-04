#!/usr/bin/env python3
"""Compare generated wind keyframes to observed vectors using approximate wall-clock alignment."""
import argparse,bisect,csv,json,math,re,struct
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();m=json.loads((a.directory/'manifest.json').read_text());out={}
for run in m['runs']:
 key='seed%d_repeat%d'%(run['seed'],run['repeat']);d=a.directory/key;s=(d/'gdb.log').read_text(errors='replace')
 match=re.search(r'FORCE_WIND_CLOCK monotonic_ns=(\d+) wall_ns=(\d+)',s);records=[]
 for _,raw in re.findall(r'WIND_ITEM\s+(\d+) hex=([0-9a-f]{32})',s): records.append(struct.unpack('<fffI',bytes.fromhex(raw)))
 times=[x[0] for x in records];errors=[];sampled=0;vectors=set()
 if match and len(records)>1:
  with (d/'before_reset_wind.csv').open() as f:
   for row in csv.DictReader(f):
    t=(int(row['%time'])-int(match[2]))/1e9; i=bisect.bisect_right(times,t)-1
    actual=[float(row['field.vector.'+axis]) for axis in 'xy'];vectors.add(tuple(round(v,6) for v in actual))
    if i<0 or i+1>=len(records):continue
    start,speed,direction,flag=records[i];nxt=records[i+1]
    if flag:
     alpha=max(0,min(1,(t-start)/(nxt[0]-start))); speed+=(nxt[1]-speed)*alpha;direction+=(nxt[2]-direction)*alpha
    pred=(speed*math.cos(math.radians(direction)),speed*math.sin(math.radians(direction)))
    errors.append(math.hypot(pred[0]-actual[0],pred[1]-actual[1]));sampled+=1
 out[key]={'keyframes':len(records),'compared_vectors':sampled,'distinct_vectors_rounded_1e-6':len(vectors),'vector_error_rmse':math.sqrt(sum(v*v for v in errors)/len(errors)) if errors else None,'vector_error_max':max(errors) if errors else None,'limitations':'ROS header and GDB wall clocks only approximately aligned; debugger stops and message timing affect differences. This is a diagnostic comparison, not calibrated latency.'}
(a.directory/'wind_interpolation_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
