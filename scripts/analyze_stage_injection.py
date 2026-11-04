#!/usr/bin/env python3
"""Summarize raw GDB stage-injection logs; attempted placements are not success counts."""
import argparse, hashlib, json, re, statistics, struct
from pathlib import Path

p=argparse.ArgumentParser(description=__doc__)
p.add_argument('directory',type=Path)
a=p.parse_args()
m=json.loads((a.directory/'manifest.json').read_text())
out={'stage':m['injected_stage'],'runs':{},'repeat_comparison':{},'limitations':['Debugger-modified initial route setup; not legal stage progression','Placement entry counts are attempts, not successful spawns','Wind at stationary starting location does not measure outdoor exposure']}
for run in m['runs']:
 key='seed%d_repeat%d'%(run['seed'],run['repeat']); path=a.directory/key/'gdb.log'
 if not path.exists(): continue
 s=path.read_text(errors='replace')
 matches=re.findall(r'WIND_ITEM\s+(\d+) hex=([0-9a-f]{32})',s)
 items=[]
 for index,h in matches:
  t,speed,direction,flag=struct.unpack('<fffI',bytes.fromhex(h))
  items.append({'index':int(index),'time_s':t,'speed':speed,'direction_deg':direction,'transition':flag,'raw_hex':h})
 intervals=[items[i+1]['time_s']-items[i]['time_s'] for i in range(len(items)-1) if items[i]['transition']==0]
 transitions=[items[i+1]['time_s']-items[i]['time_s'] for i in range(len(items)-1) if items[i]['transition']==1]
 counts={name:len(re.findall(r'TRACE '+name+r'\b',s)) for name in ('line','thin_place','thick_place','vehicle')}
 out['runs'][key]={'status':run['status'],'wind_items':items,'wind_bytes_sha256':hashlib.sha256(bytes.fromhex(''.join(x['raw_hex'] for x in items))).hexdigest(),'entry_counts':counts,'spawn_collision_warnings':s.count('SpawnActor failed because of collision'),'hold_duration_min_max_s':[min(intervals),max(intervals)] if intervals else None,'transition_duration_min_max_s':[min(transitions),max(transitions)] if transitions else None,'speed_min_max':[min(x['speed'] for x in items),max(x['speed'] for x in items)] if items else None}
for seed in m['seeds']:
 group=[x for k,x in out['runs'].items() if k.startswith('seed%d_'%seed)]
 out['repeat_comparison'][str(seed)]={'runs':len(group),'same_wind_bytes':len(group)>1 and len({x['wind_bytes_sha256'] for x in group})==1,'same_entry_counts':len(group)>1 and all(x['entry_counts']==group[0]['entry_counts'] for x in group)}
(a.directory/'analysis.json').write_text(json.dumps(out,indent=2,ensure_ascii=False)+'\n')
print(json.dumps({'stage':out['stage'],'repeats':out['repeat_comparison'],'counts':{k:v['entry_counts'] for k,v in out['runs'].items()}},indent=2))
