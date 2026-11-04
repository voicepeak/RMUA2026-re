#!/usr/bin/env python3
"""Summarize vehicle samples and drone-hit event timing; not collision attribution."""
import argparse,json,math,re,struct
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();m=json.loads((a.directory/'manifest.json').read_text());out={}
for run in m['runs']:
 key='seed%d_repeat%d'%(run['seed'],run['repeat']);s=(a.directory/key/'gdb.log').read_text(errors='replace');world=[];hits=[]
 for mono,x,y,z in re.findall(r'VEHICLE_WORLD mono=([\d.]+) actor=0x[0-9a-f]+ component=0x[0-9a-f]+ xyz_cm=([\d.eE+-]+),([\d.eE+-]+),([\d.eE+-]+)',s):world.append([float(mono),float(x),float(y),float(z)])
 for mono,raw in re.findall(r'DRONE_HIT_EVENT mono=([\d.]+) raw16=([0-9a-f]{32})',s):
  vptr,tag=struct.unpack('<QQ',bytes.fromhex(raw));hits.append({'mono':float(mono),'vptr':hex(vptr),'tag':tag})
 closest=min(world,key=lambda p:math.hypot(p[1],p[2])) if world else None
 out[key]={'vehicle_fixture':m['vehicle_fixture'],'fixture_writes':s.count('VEHICLE_FIXTURE '),'world_samples':len(world),'closest_sample_to_fixed_world_origin':closest,'hit_events':len(hits),'identified_drone_hit_events':sum(x['vptr']=='0xbacf60' and x['tag']==29 for x in hits),'events_within_one_second_of_closest':sum(abs(x['mono']-closest[0])<=1 for x in hits) if closest else None,'limitations':['Closest fixed-world origin is not measured drone position','Event type does not identify contact actor','Fixed test path, forced state and GDB alter behavior']}
(a.directory/'vehicle_fixture_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
