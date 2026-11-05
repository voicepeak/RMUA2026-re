#!/usr/bin/env python3
"""Join ROS hook and receiver rows only on unambiguous, nonzero header stamps."""
from collections import Counter,defaultdict
import json
from pathlib import Path
import statistics
import sys

root=Path(sys.argv[1])
def read(name):
 p=root/name
 return [json.loads(x) for x in p.read_text().splitlines() if x.strip()] if p.exists() else []
hooks=read('publish.jsonl');sensors=read('sensor_clock.jsonl');pwm=read('pwm_output.jsonl');commands=read('commands.jsonl')
valid=lambda n: isinstance(n,int) and n>0

def match(left,right):
 L=defaultdict(list);R=defaultdict(list)
 for stamp,t in left:
  if valid(stamp) and valid(t):L[stamp].append(t)
 for stamp,t in right:
  if valid(stamp) and valid(t):R[stamp].append(t)
 overlap=L.keys() & R.keys()
 unique=[(s,L[s][0],R[s][0]) for s in overlap if len(L[s])==len(R[s])==1]
 d=sorted((b-a)/1e6 for _,a,b in unique)
 result={'source_rows':len(left),'receiver_rows':len(right),'unique_matched_stamps':len(unique),
         'ambiguous_shared_stamps':sum(len(L[s])!=1 or len(R[s])!=1 for s in overlap),
         'source_only_stamps':len(L.keys()-R.keys()),'receiver_only_stamps':len(R.keys()-L.keys()),
         'negative_delays':sum(x<0 for x in d)}
 if d:
  result.update({'min_ms':d[0],'median_ms':statistics.median(d),'p95_ms':d[int(.95*(len(d)-1))],
                 'p99_ms':d[int(.99*(len(d)-1))],'max_ms':d[-1]})
 return result

def hook(topic):
 return [(x.get('sec',0)*1000000000+x.get('nsec',0),x.get('mono_ns')) for x in hooks if x.get('type')==topic and 0<=x.get('nsec',-1)<1000000000]
result={'scope':'same-host CLOCK_MONOTONIC on hook entry and observer callback; exact header-stamp joins, not physical acquisition or motor response latency',
        'limitations':['interposition and ROS observers alter timing', 'header stamp is an identifier, not assumed clock-synchronized',
                       'duplicate or zero stamps are excluded, and unmatched rows can be from outside simultaneous windows',
                       'sender command monotonic time is recorded after rospy publish returns, so its comparison is not a full publish-to-callback latency',
                       'one host, seed and run; no general latency bound'],
        'hook_counts':dict(Counter(x.get('type') for x in hooks)),'comparisons':{}}
for topic in ('imu','lidar'):
 result['comparisons'][topic+'_publish_to_subscriber']=match(hook(topic),[(x.get('header_stamp_ns'),x.get('arrival_monotonic_ns')) for x in sensors if x.get('topic')==topic])
result['comparisons']['pwm_publish_to_subscriber']=match(hook('rotor_pwm'),[(x.get('header_stamp_ns'),x.get('arrival_monotonic_ns')) for x in pwm])
result['comparisons']['pwm_command_send_return_to_sim_callback']=match([(x.get('header_ns'),x.get('mono_ns')) for x in commands],hook('rotor_pwm_command_callback'))
source_clock={x['sec']*1000000000+x['nsec']:x for x in hooks if x.get('type')=='imu' and x.get('sec') and 0<=x.get('nsec',-1)<1000000000}
clock_differences=[]
for row in sensors:
 if row.get('topic')!='imu':continue
 source=source_clock.get(row.get('header_stamp_ns'))
 if source and row.get('arrival_wall_ns') and row.get('arrival_monotonic_ns'):
  clock_differences.append(((row['arrival_wall_ns']-row['arrival_monotonic_ns'])-(source['wall_ns']-source['mono_ns']))/1e6)
if clock_differences:
 clock_differences.sort()
 result['wall_minus_monotonic_offset_difference_ms']={'pairs':len(clock_differences),'min':clock_differences[0],
  'median':statistics.median(clock_differences),'max':clock_differences[-1],
  'note':'independent wall/monotonic reads include scheduling skew; this checks clock consistency, not acquisition latency'}
(root/'transport_analysis.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
