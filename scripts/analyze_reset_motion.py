#!/usr/bin/env python3
"""Compare sampled API kinematics around a controlled reset, posthoc only."""
import json,math,re,sys
from pathlib import Path
root=Path(sys.argv[1]);manifest=json.loads((root/'manifest.json').read_text());results={}
pattern=re.compile(r'RESET_KINEMATICS mono_ns=(\d+) ptr=(0x[0-9a-f]+) raw=([0-9a-f]+) values=([^\s]+)')
for run in manifest['runs']:
 name='seed%d_repeat%d'%(run['seed'],run['repeat']);p=root/name;log=(p/'gdb.log').read_text(errors='replace')
 rows=[]
 for m in pattern.finditer(log):
  v=[float(x) for x in m[4].split(',')]
  if len(v)==19 and all(math.isfinite(x) for x in v):rows.append({'mono_ns':int(m[1]),'ptr':m[2],'position':v[:3],'orientation_raw':v[3:7],'linear_velocity':v[7:10],'other_raw':v[10:],'raw_hex':m[3]})
 cmd_path=p/'pwm_commands.jsonl'
 if not cmd_path.exists():cmd_path=p/'pwm_reset_events.jsonl'
 events=[json.loads(x) for x in cmd_path.read_text().splitlines() if x.strip()]
 cmds=[x for x in events if 'command' in x]
 entry=re.search(r'RESET_SERVICE_ENTRY mono_ns=(\d+)',log);bridge=re.search(r'RESET_BRIDGE_POSE mono_ns=(\d+)',log)
 if not rows or not cmds or not entry or not bridge:
  results[name]={'error':'missing kinematics, commands or reset markers','kinematics_count':len(rows),'command_count':len(cmds)};continue
 start=cmds[0]['mono_ns'];stop=int(entry[1]);done=int(bridge[1])
 base=[r for r in rows if r['mono_ns']<start];before=[r for r in rows if r['mono_ns']<stop];after=[r for r in rows if r['mono_ns']>=done+1000000000];during=[r for r in rows if start<=r['mono_ns']<stop]
 if not base or not before or not after or not during:
  results[name]={'error':'one or more evaluation windows empty','kinematics_count':len(rows)};continue
 baseline=base[-1];pre=before[-1];post=after[0]
 bridge_nearest=min(rows,key=lambda x:abs(x['mono_ns']-done))
 speed=lambda r:math.sqrt(sum(x*x for x in r['linear_velocity']))
 post_samples={}
 for delay in (1,2,3,4):
  selected=next((r for r in rows if r['mono_ns']>=done+delay*1000000000),None)
  if selected:
   post_samples[str(delay)]={'sample':selected,'actual_gap_ms':(selected['mono_ns']-done)/1e6,
    'position_distance_from_baseline':math.dist(baseline['position'],selected['position']),
    'velocity_distance_from_baseline':math.dist(baseline['linear_velocity'],selected['linear_velocity'])}
 results[name]={'status':run.get('status'),'reset_response_success':run.get('reset_response_success'),
  'sample_count':len(rows),'command_first_mono_ns':start,'command_last_mono_ns':cmds[-1]['mono_ns'],
  'reset_service_call_event':next((x for x in events if x.get('event')=='reset_call'),None),
  'reset_service_response_event':next((x for x in events if x.get('event')=='reset_response'),None),
  'reset_entry_mono_ns':stop,'bridge_marker_mono_ns':done,'baseline_before_action':baseline,
  'last_before_reset':pre,'first_at_least_1s_after_bridge_marker':post,
  'nearest_bridge_marker_sample':bridge_nearest,
  'nearest_bridge_marker_gap_ms':abs(bridge_nearest['mono_ns']-done)/1e6,
  'post_bridge_delay_samples':post_samples,
  'baseline_post_position_distance':math.dist(baseline['position'],post['position']),
  'baseline_post_velocity_distance':math.dist(baseline['linear_velocity'],post['linear_velocity']),
  'action_max_speed':max(speed(x) for x in during),'action_position_min':[min(x['position'][i] for x in during) for i in range(3)],
  'action_position_max':[max(x['position'][i] for x in during) for i in range(3)],
  'sample_before_reset_gap_ms':(stop-pre['mono_ns'])/1e6,'post_bridge_gap_ms':(post['mono_ns']-done)/1e6}
results['_limitations']=['values are the API getter snapshot, not independently all physics bodies or full environment state',
 'position, orientation and velocity offsets supported by exported accessor disassembly; coordinate frame and units need independent verification',
 'fixed PWM and timing are open-loop but experiment design was informed by earlier posthoc observations',
 'GDB stage/root height interventions and explicit wind reseed are required for this test',
 'asynchronous samples around reset cannot establish instantaneous velocity, collision manifold, vehicles, or full trajectory replay']
(root/'kinematics_analysis.json').write_text(json.dumps(results,indent=2)+'\n');print(json.dumps(results,indent=2))
