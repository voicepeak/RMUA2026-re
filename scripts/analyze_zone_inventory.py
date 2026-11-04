#!/usr/bin/env python3
"""Match raw runtime actor-root positions to serialized map data, retaining ambiguity."""
import argparse,json,math
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('gdb_log',type=Path);p.add_argument('--map',type=Path,default=Path('artifacts/maps/maps_summary_all.json'));p.add_argument('--output',type=Path,required=True);a=p.parse_args()
m=json.loads(a.map.read_text())['RMUA2024OL1']; loc=lambda v:[v[k] for k in ('X','Y','Z')]
groups={'array30':[(r['spline']['actor'],loc(r['spline']['location'])) for r in m['config']['RouteConfigs'] if r.get('spline')],
 'array40':[(name,loc(v['location'])) for name,v in m['ports'].items() if v and v.get('location')],
 'array60':[(name,loc(v['location'])) for name,v in m['factories'].items() if v and v.get('location')]}
events=[]
for line in a.gdb_log.open(errors='replace'):
 if 'RMUA_PREDICATE_CHANGE ' in line:
  try:events.append(json.loads(line.split('RMUA_PREDICATE_CHANGE ',1)[1]))
  except ValueError:pass
if not events:raise SystemExit('no predicate events')
first=events[0];inventory={}
for name,group in first['lists'].items():
 rows=[]
 for obj in group.get('objects',[]):
  pos=obj.get('root_xyz_cm');choices=[]
  if pos and all(math.isfinite(x) for x in pos):
   distances=sorted((math.dist(pos,p),n) for n,p in groups.get(name,[]))
   choices=[{'name':n,'distance_cm':d} for d,n in distances if d<=.1]
  rows.append({'index':obj['index'],'object':obj['object'],'vtable':obj.get('vtable'),'class_fname':obj.get('class_fname'),'object_fname':obj.get('object_fname'),'root_xyz_cm':pos,'within_0_1cm':choices})
 inventory[name]=rows
out={'map':'RMUA2024OL1','matching':'raw root world location to serialized actor/spline location; multiple candidates and no matches retained, FName not decoded','limitations':['identical root positions make some names ambiguous','positions alone do not establish class identity or runtime collision bounds','array70 has no side-track positions in the static input','single process'], 'inventory':inventory,'predicate_events':len(events)}
a.output.write_text(json.dumps(out,indent=2)+'\n')
for name,rows in inventory.items():print(name,'count',len(rows),'one-match',sum(len(r['within_0_1cm'])==1 for r in rows),'ambiguous',sum(len(r['within_0_1cm'])>1 for r in rows),'unmatched',sum(not r['within_0_1cm'] for r in rows))
