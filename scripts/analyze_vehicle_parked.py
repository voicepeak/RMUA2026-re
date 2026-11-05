#!/usr/bin/env python3
"""Associate vehicle-source hit events with the fixed-path diagnostic timeline."""
import json
from pathlib import Path
import re
import sys
p=Path(sys.argv[1]);manifest=json.loads((p/'manifest.json').read_text());out={}
hit_re=re.compile(r'HIT_SOURCE mono=([\d.]+) drone=(0x[0-9a-f]+) drone_component=(0x[0-9a-f]+) drone_world_cm=([^\s]+) source_actor=(0x[0-9a-f]+) source_component=(0x[0-9a-f]+)')
child_re=re.compile(r'VEHICLE_CHILD mono=([\d.]+) actor=(0x[0-9a-f]+) offset=0x240 component=(0x[0-9a-f]+) outer=(0x[0-9a-f]+) fname=([^\s]+) xyz_cm=([^\s]+) quat=([^\s]+) scale=([^\s]+)')
tick_re=re.compile(r'VEHICLE_TICK mono=([\d.]+) actor=(0x[0-9a-f]+) progress=([^\s]+) speed=([^\s]+) dt=([^\s]+)')
for run in manifest['runs']:
 name='seed%d_repeat%d'%(run['seed'],run['repeat']);d=p/name;log=(d/'gdb.log').read_text(errors='replace')
 children=[{'t':float(m[1]),'actor':m[2],'component':m[3],'outer':m[4]} for m in child_re.finditer(log)]
 actors={x['actor'] for x in children};components={x['component'] for x in children}
 ticks=[{'t':float(m[1]),'actor':m[2],'progress_cm':float(m[3]),'speed_field_cm_s':float(m[4])} for m in tick_re.finditer(log)]
 hits=[]
 for m in hit_re.finditer(log):
  x={'mono_s':float(m[1]),'drone':m[2],'drone_component':m[3],'drone_world_cm':[float(v) for v in m[4].split(',')],
     'source_actor':m[5],'source_component':m[6],'actor_match':m[5] in actors,'child_match':m[6] in components}
  if ticks:
   nearest=min(ticks,key=lambda k:abs(k['t']-x['mono_s']));x['nearest_tick']=nearest;x['tick_gap_s']=abs(nearest['t']-x['mono_s'])
  hits.append(x)
 commands_path=d/'pwm_commands.jsonl'; commands=[json.loads(x) for x in commands_path.read_text().splitlines()] if commands_path.exists() else []
 transitions=[]
 for c in commands:
  if not transitions or c['command']!=transitions[-1]['command']:
   transitions.append({'mono_s':c['mono_ns']/1e9,'command':c['command']})
 matched=[x for x in hits if x['actor_match'] or x['child_match']]
 out[name]={'status':run.get('status'),'vehicle_actor_ids':sorted(actors),'candidate_child_ids':sorted(components),
 'hit_count':len(hits),'matched_actor_or_child_count':len(matched),
 'matched_both_count':sum(x['actor_match'] and x['child_match'] for x in hits),
 'matched_hits':matched,'command_transitions':transitions,
 'limitations':['stage, path and state gate are debugger interventions; not ordinary race progression',
 'candidate child inferred from pointer ownership and matching asset transform, not independently typed',
 'HIT_SOURCE callback argument matches attribute event source but does not establish force or complete physical contact manifold',
 'debugger changes scheduling; continuously forced state may overwrite terminal state',
 'path and action timing were designed using prior posthoc observations; controller does not read truth online']}
(p/'parked_contact_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
