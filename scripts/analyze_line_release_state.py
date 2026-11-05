#!/usr/bin/env python3
"""Link a fixed line instance's hit events to state writes after force release."""
import json,re,sys
from pathlib import Path
p=Path(sys.argv[1]);man=json.loads((p/'manifest.json').read_text());out={}
fixture=re.compile(r'LINE_FIXTURE selector=(-?\d+) component=(0x[0-9a-f]+)')
ret=re.compile(r'LINE_FIXTURE_RESULT index=(-?\d+)')
release=re.compile(r'LINE_FORCE_RELEASE mono_ns=(\d+) state=(0x[0-9a-f]+) value=(\d+)')
alloc=re.compile(r'LINE_EVENT_ALLOC mono_ns=(\d+) tid=(\d+) actor=(0x[0-9a-f]+) component=(0x[0-9a-f]+) candidate_index=(-?\d+) event=(0x[0-9a-f]+)')
callback=re.compile(r'LINE_EVENT_CALLBACK mono_ns=(\d+) tid=(\d+) event=(0x[0-9a-f]+)')
write=re.compile(r'LINE_STATE_TWELVE_WRITTEN mono_ns=(\d+) tid=(\d+) released=(\d+) state=(0x[0-9a-f]+) current=(\d+) previous=(\d+)')
sample=re.compile(r'LINE_STATE_SAMPLE mono_ns=(\d+) state=(0x[0-9a-f]+) value=(\d+)')
for run in man['runs']:
 name='seed%d_repeat%d'%(run['seed'],run['repeat']);log=(p/name/'gdb.log').read_text(errors='replace');f=fixture.search(log);i=ret.search(log);rel=release.search(log)
 if not f or not i or not rel:out[name]={'error':'fixture, return index or release missing'};continue
 component=f[2];index=int(i[1]);release_ns=int(rel[1]);state=rel[2];events=[]
 for m in alloc.finditer(log):events.append((int(m[1]),'alloc',{'tid':int(m[2]),'actor':m[3],'component':m[4],'candidate_index':int(m[5]),'event':m[6]}))
 for m in callback.finditer(log):events.append((int(m[1]),'callback',{'tid':int(m[2]),'event':m[3]}))
 pending={};pairs=[];unmatched=0
 for t,kind,x in sorted(events,key=lambda q:q[0]):
  if kind=='alloc':pending[x['event']]=(t,x)
  else:
   prev=pending.pop(x['event'],None)
   if prev:
    at,a=prev;pairs.append({'alloc_ns':at,'callback_ns':t,'allocation_tid':a['tid'],'callback_tid':x['tid'],
     'actor':a['actor'],'component':a['component'],'candidate_index':a['candidate_index'],'event':x['event'],
     'fixture_component_and_index_match':a['component']==component and a['candidate_index']==index})
   else:unmatched+=1
 writes=[{'mono_ns':int(m[1]),'tid':int(m[2]),'released':bool(int(m[3])),'state':m[4],'current':int(m[5]),'previous':int(m[6])} for m in write.finditer(log)]
 samples=[{'mono_ns':int(m[1]),'value':int(m[3])} for m in sample.finditer(log) if m[2]==state and int(m[1])>=release_ns]
 hits=[x for x in pairs if x['fixture_component_and_index_match'] and x['callback_ns']>=release_ns];links=[]
 for h in hits:
  following=min((x['callback_ns'] for x in pairs if x['callback_ns']>h['callback_ns']),default=None)
  options=[w for w in writes if w['released'] and w['state']==state and w['tid']==h['callback_tid'] and w['mono_ns']>=h['callback_ns'] and (following is None or w['mono_ns']<following)]
  if options:links.append({'hit':h,'following_same_thread_write':options[0]})
 terminal=[x for x in samples if x['value']==12]
 out[name]={'fixture_selector':int(f[1]),'fixture_component':component,'fixture_return_index':index,'release_state':int(rel[3]),
 'release_mono_ns':release_ns,'all_event_pairs':len(pairs),'unmatched_callbacks':unmatched,'post_release_fixture_pairs':len(hits),
 'fixture_callback_to_state_write_links':links,'post_release_state_writes':[x for x in writes if x['released']],
 'passive_samples':samples,'terminal_sample_span_s':(terminal[-1]['mono_ns']-terminal[0]['mono_ns'])/1e9 if terminal else None,
 'terminal_samples':len(terminal),'values_after_first_terminal':sorted({x['value'] for x in samples if terminal and x['mono_ns']>=terminal[0]['mono_ns']}),
 'limitations':['candidate index at hit-result offset 0x58 is inferred; independent metadata unavailable',
 'force release and action timing use prior posthoc observations; stage and fixture are injected',
 'pointer and same-thread order are scoped observational evidence, not proof of force or full episode termination',
 'passive sampling is discontinuous and GDB changes execution timing']}
(p/'line_release_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
