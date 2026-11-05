#!/usr/bin/env python3
"""Compare hit-source actor/component pointers with initialized vehicles in one run."""
import argparse,collections,json,re
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args()
m=json.loads((a.directory/'manifest.json').read_text());out={}
for run in m['runs']:
 key='seed%d_repeat%d'%(run['seed'],run['repeat']);log=a.directory/key/'gdb.log'
 if not log.exists():continue
 s=log.read_text(errors='replace')
 actors=sorted(set(re.findall(r'VEHICLE_INIT actor=(0x[0-9a-f]+)',s)))
 pairs=sorted(set(re.findall(r'VEHICLE_WORLD mono=[\d.]+ actor=(0x[0-9a-f]+) component=(0x[0-9a-f]+)',s)))
 matches=re.findall(r'HIT_SOURCE mono=([\d.]+) drone=(0x[0-9a-f]+) drone_component=(0x[0-9a-f]+) drone_world_cm=([^\s]+) source_actor=(0x[0-9a-f]+) source_component=(0x[0-9a-f]+)',s)
 counts=collections.Counter((v[4],v[5]) for v in matches)
 target=[v for v in matches if v[4] in actors or any(v[5]==pair[1] for pair in pairs)]
 out[key]={'vehicle_actors':actors,'sampled_actor_components':pairs,'fixture_writes':s.count('VEHICLE_FIXTURE coordinate_source='),
           'hit_source_count':len(matches),'hit_sources':[{'actor':k[0],'component':k[1],'count':v} for k,v in counts.items()],
           'same_actor_or_sampled_component_count':len(target),'same_actor_or_sampled_component':target,
           'fixed_pwm_command_exit_code':run.get('fixed_pwm_command_exit_code'),
           'limitations':['a nonmatching actor or sampled component does not exclude other child components or unobserved contacts',
                          'forced route, state, debugger and finite sampling differ from natural progression',
                          'positions are recorded only for posthoc evaluation']}
(a.directory/'contact_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
