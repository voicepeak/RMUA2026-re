#!/usr/bin/env python3
"""Link separately traced hit-source allocations to collision callbacks by event pointer."""
import ast,json,re,sys
from pathlib import Path
p=Path(sys.argv[1]);manifest=json.loads((p/'manifest.json').read_text());res={}
alloc=re.compile(r'VEHICLE_EVENT_ALLOC mono=([\d.]+) tid=(\d+) source_actor=(0x[0-9a-f]+) source_component=(0x[0-9a-f]+) event=(0x[0-9a-f]+)')
callback=re.compile(r'VEHICLE_EVENT_CALLBACK mono=([\d.]+) tid=(\d+) event=(0x[0-9a-f]+)')
contact=re.compile(r'VEHICLE_CONTACT_RAW mono=([\d.]+) source_actor=(0x[0-9a-f]+) source_component=(0x[0-9a-f]+) hitptr=(0x[0-9a-f]+) self_moved=(\d+) xmm=(\[\[.*?\]\]) hit_bytes=([0-9a-f]+|unreadable)')
for run in manifest['runs']:
 name='seed%d_repeat%d'%(run['seed'],run['repeat']);root=p/name;log=(root/'gdb.log').read_text(errors='replace')
 vehicle=json.loads((p/'parked_contact_analysis.json').read_text())[name];actors=set(vehicle['vehicle_actor_ids']);children=set(vehicle['candidate_child_ids'])
 events=[]
 for m in alloc.finditer(log):events.append((float(m[1]),'alloc',{'tid':int(m[2]),'source_actor':m[3],'source_component':m[4],'event':m[5]}))
 for m in callback.finditer(log):events.append((float(m[1]),'callback',{'tid':int(m[2]),'event':m[3]}))
 pending={};pairs=[];unmatched_callbacks=0
 for t,kind,x in sorted(events,key=lambda z:z[0]):
  if kind=='alloc':pending[x['event']]=(t,x)
  else:
   prior=pending.pop(x['event'],None)
   if prior:
    at,a=prior;pairs.append({'alloc_mono_s':at,'callback_mono_s':t,'elapsed_ms':(t-at)*1000,
      'event':x['event'],'source_actor':a['source_actor'],'source_component':a['source_component'],
      'source_matches_vehicle_and_child':a['source_actor'] in actors and a['source_component'] in children,
      'allocation_tid':a['tid'],'callback_tid':x['tid']})
   else:unmatched_callbacks+=1
 raw=[]
 for m in contact.finditer(log):
  v=ast.literal_eval(m[6]);raw.append({'mono_s':float(m[1]),'source_actor':m[2],'source_component':m[3],'hitptr':m[4],
   'self_moved_raw':int(m[5]),'xmm_pairs':v,'candidate_location':[v[0][0],v[0][1],v[1][0]],
   'candidate_normal':[v[2][0],v[2][1],v[3][0]],'candidate_impulse':[v[4][0],v[4][1],v[5][0]],'hit_bytes_hex':m[7]})
 res[name]={'vehicle_actor_ids':sorted(actors),'candidate_child_ids':sorted(children),'alloc_callback_pairs':len(pairs),
 'vehicle_pairs':sum(x['source_matches_vehicle_and_child'] for x in pairs),'unmatched_callbacks':unmatched_callbacks,
 'pending_allocations':len(pending),'pairs':pairs,'raw_contacts':raw,
 'limitations':['allocation-to-callback pointer identity is a scoped observed chain, not a unique identity across all time',
 'candidate vector labels inferred from caller register setup; hit bytes retained for independent ABI verification',
 'zero candidate impulse passed by this call path does not establish zero physical force',
 'state and path forced by GDB, which perturbs scheduling and can overwrite terminal state']}
(p/'contact_detail_analysis.json').write_text(json.dumps(res,indent=2)+'\n');print(json.dumps(res,indent=2))
