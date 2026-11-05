#!/usr/bin/env python3
"""Posthoc matching of kinematic snapshots around identical open-loop phases."""
import bisect,json,math,re,sys
from pathlib import Path
root=Path(sys.argv[1]);man=json.loads((root/'manifest.json').read_text());out={}
pattern=re.compile(r'RESET_KINEMATICS mono_ns=(\d+) ptr=(0x[0-9a-f]+) raw=([0-9a-f]+) values=([^\s]+)')
for run in man['runs']:
 name='seed%d_repeat%d'%(run['seed'],run['repeat']);p=root/name
 events=[json.loads(x) for x in (p/'pwm_reset_events.jsonl').read_text().splitlines() if x.strip()]
 starts={x['phase']:x['mono_ns'] for x in events if x.get('event')=='phase_start'}
 ends={x['phase']:x['mono_ns'] for x in events if x.get('event')=='phase_end'}
 commands={phase:[x for x in events if x.get('event')=='command' and x.get('phase')==phase] for phase in ('before','after')}
 snapshots=[]
 for m in pattern.finditer((p/'gdb.log').read_text(errors='replace')):
  v=[float(x) for x in m[4].split(',')]
  if len(v)==19 and all(math.isfinite(x) for x in v):snapshots.append({'t':int(m[1]),'position':v[:3],'orientation_raw':v[3:7],'velocity':v[7:10]})
 snapshots.sort(key=lambda x:x['t']);times=[x['t'] for x in snapshots]
 def nearest(t):
  i=bisect.bisect_left(times,t);opts=[snapshots[j] for j in (i-1,i) if 0<=j<len(snapshots)]
  return min(opts,key=lambda x:abs(x['t']-t)) if opts else None
 if not all(x in starts and x in ends and commands[x] for x in ('before','after')):
  out[name]={'error':'missing phase records or commands'};continue
 reference={k:nearest(starts[k]) for k in ('before','after')}
 matches=[]
 for i in range(18):
  elapsed=i*.2; samples={k:nearest(starts[k]+int(elapsed*1e9)) for k in ('before','after')}
  if any(x is None for x in samples.values()):continue
  errors={k:abs(samples[k]['t']-(starts[k]+int(elapsed*1e9)))/1e9 for k in samples}
  if max(errors.values())>.075:continue
  a,b=samples['before'],samples['after']; ar=reference['before'];br=reference['after']
  if not ar or not br:continue
  apos=[x-y for x,y in zip(a['position'],ar['position'])];bpos=[x-y for x,y in zip(b['position'],br['position'])]
  matches.append({'elapsed_s':elapsed,'sample_time_error_s':errors,'before':a,'after':b,
    'absolute_position_difference':math.dist(a['position'],b['position']),
    'relative_position_difference':math.dist(apos,bpos),'velocity_difference':math.dist(a['velocity'],b['velocity'])})
 def max_or_none(field):return max((x[field] for x in matches),default=None)
 out[name]={'status':run.get('status'),'reset_success':run.get('reset_response_success'),
  'phase_start_ns':starts,'phase_end_ns':ends,'command_counts':{k:len(v) for k,v in commands.items()},
  'command_values':{k:sorted({tuple(x['command']) for x in v}) for k,v in commands.items()},
  'reference_samples':reference,'matched_samples':len(matches),'matches':matches,
  'max_absolute_position_difference':max_or_none('absolute_position_difference'),
  'max_relative_position_difference':max_or_none('relative_position_difference'),
  'max_velocity_difference':max_or_none('velocity_difference')}
out['_limitations']=['matching uses wall monotonic phase time with up to 75 ms sample error, not lockstep physics time',
 'command publication is observed but motor application and exact continuous input equivalence are not proven',
 'API state is not full scene, contacts, vehicle or all simulator physics state',
 'fixed 3-second wait chosen using prior posthoc observations; no debug truth used online',
 'stage, height hold and wind reseed are diagnostic interventions']
(root/'trajectory_replay_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
