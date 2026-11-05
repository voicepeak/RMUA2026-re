#!/usr/bin/env python3
"""Correlate force release, vehicle event pointer, state write and passive samples."""
import json,re,sys
from pathlib import Path
p=Path(sys.argv[1]);man=json.loads((p/'manifest.json').read_text());details=json.loads((p/'contact_detail_analysis.json').read_text());out={}
release=re.compile(r'FORCE_WIND_RELEASE mono_ns=(\d+) state=(0x[0-9a-f]+) state_value=(\d+)')
write=re.compile(r'VEHICLE_STATE_TWELVE_WRITTEN mono_ns=(\d+) tid=(\d+) released=(\d+) state=(0x[0-9a-f]+) current=(\d+) previous=(\d+)')
sample=re.compile(r'VEHICLE_STATE_SAMPLE mono_ns=(\d+) state=(0x[0-9a-f]+) value=(\d+)')
for run in man['runs']:
 name='seed%d_repeat%d'%(run['seed'],run['repeat']);log=(p/name/'gdb.log').read_text(errors='replace');rel=release.search(log)
 if not rel:out[name]={'error':'force release not observed'};continue
 release_ns=int(rel[1]);state=rel[2]
 writes=[{'mono_ns':int(m[1]),'tid':int(m[2]),'released':bool(int(m[3])),'state':m[4],'current':int(m[5]),'previous':int(m[6])} for m in write.finditer(log)]
 samples=[{'mono_ns':int(m[1]),'state':m[2],'value':int(m[3])} for m in sample.finditer(log) if m[2]==state and int(m[1])>=release_ns]
 vehicle=[x for x in details[name]['pairs'] if x['source_matches_vehicle_and_child'] and x['callback_mono_s']*1e9>=release_ns]
 links=[]
 for callback in vehicle:
  t=int(callback['callback_mono_s']*1e9);next_callback=min((int(x['callback_mono_s']*1e9) for x in details[name]['pairs'] if int(x['callback_mono_s']*1e9)>t),default=None)
  candidates=[w for w in writes if w['released'] and w['state']==state and w['tid']==callback['callback_tid'] and w['mono_ns']>=t and (next_callback is None or w['mono_ns']<next_callback)]
  if candidates:links.append({'event':callback,'following_same_thread_state_write':candidates[0]})
 terminal=[x for x in samples if x['value']==12]
 out[name]={'release_mono_ns':release_ns,'release_state':int(rel[3]),'state_pointer':state,'all_writes':writes,
  'post_release_vehicle_callback_count':len(vehicle),'vehicle_callback_to_state_write_links':links,'passive_samples':samples,
  'terminal_sample_count':len(terminal),'terminal_sample_span_s':(terminal[-1]['mono_ns']-terminal[0]['mono_ns'])/1e9 if terminal else None,
  'sampled_values_after_first_terminal':sorted({x['value'] for x in samples if terminal and x['mono_ns']>=terminal[0]['mono_ns']}),
  'limitations':['GDB mutates stage, route and initial state; force is released on fixed timer selected using earlier posthoc observations',
  'event pointer is matched within one process; same-thread ordering is not a general proof of natural race completion',
  'passive samples cannot rule out state changes between samples; simulator, score and episode terminal behavior not fully verified',
  'debugger changes timing and command policy is open-loop without truth input']}
(p/'release_state_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
