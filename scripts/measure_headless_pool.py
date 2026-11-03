#!/usr/bin/env python3
"""Measure an already-running NullRHI pool; records raw timestamps and CPU/RSS."""
import argparse
from concurrent.futures import ThreadPoolExecutor
import json
import os
from pathlib import Path
import statistics
import subprocess
import time

def sim_pid(name):
    text=subprocess.check_output(['docker','top',name,'-eo','pid,args'],text=True)
    candidates=[int(line.split(None,1)[0]) for line in text.splitlines()[1:] if 'RMUA-Linux-Shipping' in line]
    matches=[]
    for pid in candidates:
        try:
            if os.readlink('/proc/%d/exe'%pid).endswith('/RMUA-Linux-Shipping'): matches.append(pid)
        except OSError:
            continue
    if len(matches)!=1: raise RuntimeError('%s: expected one simulator, got %s' % (name,matches))
    return matches[0]

def stat(pid):
    try:
        content=Path('/proc/%d/stat'%pid).read_text()
    except OSError as exc:
        return {'error':str(exc)}
    after=content[content.rfind(')')+2:].split()
    ticks=int(after[11])+int(after[12])
    rss_pages=int(after[21])
    return {'ticks':ticks,'rss_bytes':rss_pages*os.sysconf('SC_PAGE_SIZE')}

def sample_node(name,seconds,source):
    cmd=['docker','exec','-i','-u',str(os.getuid())+':'+str(os.getgid()),
         '-e','ROS_HOME=/tmp/probe-ros',name,'bash','-c',
         'source /opt/ros/noetic/setup.bash; exec python3 - "$1"','pool-probe',str(seconds)]
    try:
        result=subprocess.run(cmd,input=source,text=True,capture_output=True,timeout=seconds+35)
    except subprocess.TimeoutExpired as exc:
        return {'error':'observer timed out','seconds':exc.timeout}
    if result.returncode: return {'error':'observer exited %d'%result.returncode,'stderr':result.stderr}
    try: return json.loads(result.stdout)
    except json.JSONDecodeError: return {'error':'invalid observer JSON','stdout':result.stdout,'stderr':result.stderr}

def summarize(rows,duration):
    if len(rows)<2: return {'count':len(rows),'error':'insufficient samples'}
    arrival=[b[0]-a[0] for a,b in zip(rows,rows[1:])]
    stamp=[b[2]-a[2] for a,b in zip(rows,rows[1:])]
    ordered=sorted(arrival)
    quant=lambda q:ordered[min(len(ordered)-1,int(q*(len(ordered)-1)))]
    deltas=[b[3]-a[3] for a,b in zip(rows,rows[1:])]
    return {'count':len(rows),'observed_hz':(len(rows)-1)/(rows[-1][0]-rows[0][0]),
            'sim_seconds_per_wall_second':(rows[-1][2]-rows[0][2])/(rows[-1][0]-rows[0][0]),
            'arrival_interval_median_s':statistics.median(arrival),'arrival_interval_p99_s':quant(.99),
            'arrival_interval_max_s':max(arrival),'stamp_regressions':sum(x<=0 for x in stamp),
            'sequence_gap_sum':sum(max(0,x-1) for x in deltas if x>=0),
            'sequence_regressions':sum(x<0 for x in deltas),
            'note':'seq gaps are observer-side missing sequence numbers, not localized packet loss'}

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('manifest',type=Path)
    p.add_argument('--seconds',type=float,default=12.)
    p.add_argument('--output',type=Path,required=True)
    a=p.parse_args()
    if not 2 <= a.seconds <= 60: p.error('seconds must be between 2 and 60')
    manifest=json.loads(a.manifest.read_text())
    if not manifest.get('ready'): raise ValueError('pool not ready')
    instances=manifest['instances']
    source=Path(__file__).with_name('pool_probe_node.py').read_text()
    pids={item['container']:sim_pid(item['container']) for item in instances}
    clk=os.sysconf('SC_CLK_TCK')
    samples=[]
    with ThreadPoolExecutor(max_workers=len(instances)) as ex:
        jobs={name:ex.submit(sample_node,name,a.seconds,source) for name in pids}
        start=time.monotonic()
        while time.monotonic()-start < a.seconds:
            samples.append({'wall_monotonic':time.monotonic(),
                            'processes':{name:stat(pid) for name,pid in pids.items()}})
            time.sleep(1)
        observations={name:job.result() for name,job in jobs.items()}
    summary={}
    for name,result in observations.items():
        if 'samples' in result:
            summary[name]={k:summarize(v,result['wall_duration_s']) for k,v in result['samples'].items()}
        else: summary[name]=result
        live=[s for s in samples if 'ticks' in s['processes'][name]]
        summary[name]['process_missing_samples']=len(samples)-len(live)
        if len(live)>=2:
            first,last=live[0],live[-1]
            d=last['wall_monotonic']-first['wall_monotonic']
            summary[name]['cpu_percent_one_core']=100*(last['processes'][name]['ticks']-first['processes'][name]['ticks'])/clk/d
            summary[name]['rss_peak_mib']=max(s['processes'][name]['rss_bytes'] for s in live)/1024**2
    data={'manifest':manifest,'pids':pids,'clk_tck':clk,'host_samples':samples,
          'observations':observations,'summary':summary,
          'limitations':['CPU and RSS are simulator process only, not ROS or observers',
                         'ROS source stamp and wall clock may not share a calibrated epoch; no latency claim',
                         'single stationary episode per seed; no claim about stage 4-6']}
    a.output.write_text(json.dumps(data,indent=2)+'\n')
    print(json.dumps(summary,indent=2))

if __name__=='__main__': main()
