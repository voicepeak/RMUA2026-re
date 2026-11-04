#!/usr/bin/env python3
"""Summarize injected vehicle path, progress and optional world component samples."""
import argparse,hashlib,json,re,statistics
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();m=json.loads((a.directory/'manifest.json').read_text());out={}
for run in m['runs']:
 key='seed%d_repeat%d'%(run['seed'],run['repeat']); path=a.directory/key/'gdb.log'
 if not path.exists():continue
 s=path.read_text(errors='replace'); paths=re.findall(r'VEHICLE_PATH actor=0x[0-9a-f]+ index=\d+ hex=([0-9a-f]{24})',s)
 ticks=[list(map(float,x)) for x in re.findall(r'VEHICLE_TICK mono=([\d.]+) actor=0x[0-9a-f]+ progress=([\d.eE+-]+) speed=([\d.eE+-]+) dt=([\d.eE+-]+)',s)]
 worlds=re.findall(r'VEHICLE_WORLD mono=([\d.]+) actor=0x[0-9a-f]+ component=0x[0-9a-f]+ xyz_cm=([^\s]+)',s)
 out[key]={'status':run['status'],'gate_checks':s.count('VEHICLE_FORCE_GATE route='),'forced_indices':s.count('VEHICLE_FORCE_GATE_INDEX_ZERO'),'initializations':s.count('VEHICLE_INIT '),'path_points':len(paths),'path_sha256':hashlib.sha256(bytes.fromhex(''.join(paths))).hexdigest() if paths else None,'ticks':len(ticks),'first_tick':ticks[0] if ticks else None,'last_tick':ticks[-1] if ticks else None,'world_samples':len(worlds),'first_world':worlds[0] if worlds else None,'last_world':worlds[-1] if worlds else None,'warning':'Injected branch/route, not natural progression; wall-time sample rate is not simulation step rate.'}
(a.directory/'analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
