#!/usr/bin/env python3
"""Analyze streamed pool evidence; do not infer latency or general stability."""
import json
import os
from pathlib import Path
import statistics
import sys

root=Path(sys.argv[1]); meta=json.loads((root/'meta.json').read_text())
measurement=json.loads((root/'summary.json').read_text())
host=[json.loads(x) for x in (root/'host_samples.jsonl').read_text().splitlines() if x.strip()]
assert host, 'no host samples'
result={'requested_s':meta['duration_requested_s'],'observer_reason':measurement['reason'],
        'host_window_s':host[-1]['monotonic_s']-host[0]['monotonic_s'],
        'limitations':meta['limitations'], 'instances':{}}
for instance in meta['manifest']['instances']:
    name=instance['container'];path=root/(name+'.jsonl')
    rows=[json.loads(x) for x in path.read_text().splitlines() if x.strip()]
    state=measurement['summary'].get(name,{})
    h=[x for x in host if 'ticks' in x['processes'].get(name,{})]
    cpu=None;peak=None
    if len(h)>1:
        dt=h[-1]['monotonic_s']-h[0]['monotonic_s']
        cpu=100*(h[-1]['processes'][name]['ticks']-h[0]['processes'][name]['ticks'])/os.sysconf('SC_CLK_TCK')/dt if dt>0 else None
        peak=max(x['processes'][name]['rss_bytes'] for x in h)/1024**2
    observations={}
    for topic in ('imu','lidar'):
        data=[x for x in rows if x.get('event')=='sample' and x.get('topic')==topic]
        if len(data)<2:
            observations[topic]={'count':len(data)};continue
        arrival=[(b['monotonic_ns']-a['monotonic_ns'])/1e9 for a,b in zip(data,data[1:])]
        stamp=[b['stamp_ns']-a['stamp_ns'] for a,b in zip(data,data[1:])]
        seq=[b['sequence']-a['sequence'] for a,b in zip(data,data[1:])]
        duration=(data[-1]['monotonic_ns']-data[0]['monotonic_ns'])/1e9
        intervals=sorted(arrival)
        observations[topic]={'count':len(data),'span_s':duration,'observed_hz':(len(data)-1)/duration,
                'source_seconds_per_wall_second':(data[-1]['stamp_ns']-data[0]['stamp_ns'])/1e9/duration,
                'median_arrival_interval_s':statistics.median(arrival),'p99_arrival_interval_s':intervals[int(.99*(len(intervals)-1))],
                'max_arrival_interval_s':max(arrival),'stamp_regressions':sum(x<0 for x in stamp),
                'nonzero_stamp_regressions':sum(b['stamp_ns']<a['stamp_ns'] for a,b in zip(data,data[1:]) if a['stamp_ns'] and b['stamp_ns']),
                'zero_stamp_count':sum(x['stamp_ns']==0 for x in data),
                'stamp_equal_adjacent':sum(x==0 for x in stamp),'sequence_gap_sum':sum(max(0,x-1) for x in seq if x>=0),
                'sequence_regressions':sum(x<0 for x in seq)}
    start=next((x for x in rows if x.get('event')=='started'),None)
    finish=next((x for x in rows if x.get('event')=='finished'),None)
    result['instances'][name]={'seed':instance['seed'],'observer_exit_code':state.get('observer_exit_code'),
        'observer_started':bool(start),'observer_finished':bool(finish),
        'observer_duration_s':(finish['monotonic_ns']-start['monotonic_ns'])/1e9 if start and finish else None,
        'host_process_missing_samples':len(host)-len(h),'simulator_cpu_percent_one_core':cpu,
        'simulator_rss_peak_mib':peak,'topics':observations}
result['all_observers_finished']=all(x['observer_started'] and x['observer_finished'] and x['observer_exit_code']==0 for x in result['instances'].values())
result['all_processes_observed']=all(x['host_process_missing_samples']==0 for x in result['instances'].values())
(root/'independent_analysis.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
