#!/usr/bin/env python3
"""Summarize raw liveness samples, keeping their wall/monotonic clock distinction."""
import argparse
import json
from pathlib import Path

p=argparse.ArgumentParser(description=__doc__)
p.add_argument('samples',type=Path)
p.add_argument('--output',type=Path,required=True)
a=p.parse_args()
rows=[json.loads(s) for s in a.samples.read_text().splitlines() if s.strip()]
summary={'samples':len(rows),'time_basis':'CPU uses host clock ticks / monotonic wall seconds, 100%=one core',
         'limitation':'measured simulator process only; short samples do not establish stability', 'containers':{}}
prev={}
for row in rows:
    for name,info in row['containers'].items():
        s=summary['containers'].setdefault(name,{'ros_master_port':info['ros_master_port'],'observed':0,
                                                  'running_samples':0,'states':[],'cpu_percent_samples':[],
                                                  'rss_peak_bytes':None})
        state=info['docker_state'];s['observed']+=1
        if state.get('Running'):s['running_samples']+=1
        if not state.get('Running'):
            s['states'].append({'wall_unix_s':row['wall_unix_s'],'state':state})
        proc=info.get('simulator_proc_stat',{})
        if 'ticks' in proc:
            old=prev.get(name);now=(row['monotonic_s'],proc['ticks'],info['simulator_host_pid'])
            if old and old[2]==now[2] and now[0]>old[0]:
                s['cpu_percent_samples'].append(100*(now[1]-old[1])/row['clock_ticks_per_second']/(now[0]-old[0]))
            prev[name]=now
            s['rss_peak_bytes']=max(s['rss_peak_bytes'] or 0,proc['rss_bytes'])
a.output.write_text(json.dumps(summary,indent=2)+'\n')
