#!/usr/bin/env python3
"""Summarize actual hit-source callback timings around two PWM phases."""
import json,re,sys
from pathlib import Path
p=Path(sys.argv[1]);man=json.loads((p/'manifest.json').read_text());out={}
pat=re.compile(r'RESET_HIT_SOURCE mono_ns=(\d+) drone=(0x[0-9a-f]+) drone_component=(0x[0-9a-f]+) source_actor=(0x[0-9a-f]+) source_component=(0x[0-9a-f]+)')
for r in man['runs']:
 name='seed%d_repeat%d'%(r['seed'],r['repeat']);d=p/name;ev=[json.loads(x) for x in (d/'pwm_reset_events.jsonl').read_text().splitlines() if x.strip()]
 starts={x['phase']:x['mono_ns'] for x in ev if x.get('event')=='phase_start'};ends={x['phase']:x['mono_ns'] for x in ev if x.get('event')=='phase_end'}
 hits=[{'mono_ns':int(m[1]),'drone':m[2],'drone_component':m[3],'source_actor':m[4],'source_component':m[5]} for m in pat.finditer((d/'gdb.log').read_text(errors='replace'))]
 phases={}
 for phase in ('before','settle','after'):
  selected=[dict(x,elapsed_s=(x['mono_ns']-starts[phase])/1e9) for x in hits if starts.get(phase,0)<=x['mono_ns']<ends.get(phase,0)]
  phases[phase]={'count':len(selected),'actor_component_pairs':sorted({(x['source_actor'],x['source_component']) for x in selected}),'hits':selected}
 out[name]={'status':r['status'],'reset_success':r.get('reset_response_success'),'phases':phases,
   'same_actor_component_pairs':sorted(set(map(tuple,phases['before']['actor_component_pairs']))&set(map(tuple,phases['after']['actor_component_pairs']))),
   'limitations':['hit-source identity is observed in one process and does not prove object type or physical impulse',
    'breakpoints and debugger change timing, matching path does not prove same physical contact event',
    'stage, height hold and explicit RNG reseed are controlled interventions; fixed wait was selected from earlier posthoc observations']}
(p/'collision_replay_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
