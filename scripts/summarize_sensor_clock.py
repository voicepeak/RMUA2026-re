#!/usr/bin/env python3
"""Summarize ROS header and arrival timings without assuming their clock origins."""
import argparse,collections,json,math,statistics
from pathlib import Path
p=argparse.ArgumentParser();p.add_argument('input',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args()
groups=collections.defaultdict(list)
for line in a.input.open():
 r=json.loads(line);groups[r['topic']].append(r)
def q(xs,f):
 xs=sorted(xs);return xs[min(len(xs)-1,math.ceil(len(xs)*f)-1)] if xs else None
out={'clock_note':'header and arrival_ros use ROS fields; epoch equivalence and sample semantics unverified. age is apparent ROS-clock age, not independently established end-to-end latency. wall and monotonic are host clocks.', 'topics':{}}
for name,rows in groups.items():
 rows.sort(key=lambda x:x['arrival_monotonic_ns']);delta=[(b['arrival_monotonic_ns']-a['arrival_monotonic_ns'])/1e9 for a,b in zip(rows,rows[1:])]
 age=[(r['arrival_ros_ns']-r['header_stamp_ns'])/1e9 for r in rows if r['header_stamp_ns']]
 seq=[b['seq']-a['seq'] for a,b in zip(rows,rows[1:])]
 out['topics'][name]={'count':len(rows),'duration_monotonic_s':(rows[-1]['arrival_monotonic_ns']-rows[0]['arrival_monotonic_ns'])/1e9,
  'arrival_hz':(len(rows)-1)/(sum(delta)) if sum(delta)>0 else None,'arrival_interval_s':{'median':statistics.median(delta) if delta else None,'p95':q(delta,.95),'p99':q(delta,.99),'max':max(delta) if delta else None},
  'apparent_ros_age_s':{'median':statistics.median(age) if age else None,'p95':q(age,.95),'p99':q(age,.99),'min':min(age) if age else None,'max':max(age) if age else None},
  'sequence_gap_sum':sum(max(0,x-1) for x in seq),'sequence_regressions':sum(x<0 for x in seq),
  'stamp_regressions':sum(b['header_stamp_ns']<a['header_stamp_ns'] for a,b in zip(rows,rows[1:])),
  'payload_examples':[ {k:r[k] for k in ('width','height','encoding','data_bytes') if k in r} for r in rows[:1]]}
a.output.write_text(json.dumps(out,indent=2)+'\n')
