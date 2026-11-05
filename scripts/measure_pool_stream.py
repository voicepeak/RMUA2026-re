#!/usr/bin/env python3
"""Observe a running pool with incremental evidence; stop on missing process."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import subprocess
import sys
import time
from measure_headless_pool import sim_pid, stat, summarize

p = argparse.ArgumentParser(description=__doc__)
p.add_argument('manifest', type=Path)
p.add_argument('--seconds', type=float, required=True)
p.add_argument('--output', type=Path, required=True)
a = p.parse_args()
if not 2 <= a.seconds <= 600: p.error('seconds must be 2..600')
manifest = json.loads(a.manifest.read_text())
if not manifest.get('ready'): p.error('pool is not ready')
out = a.output.resolve(); out.mkdir(parents=True, exist_ok=False)
source_path = Path(__file__).with_name('pool_probe_stream.py')
source = source_path.read_bytes()
meta = {'duration_requested_s': a.seconds, 'started_wall_ns': time.time_ns(),
        'started_monotonic_ns': time.monotonic_ns(), 'manifest': manifest,
        'probe_sha256': hashlib.sha256(source).hexdigest(), 'observer_sha256': hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),
        'limitations': ['timestamps are ROS arrival and source stamps, not calibrated latency',
                        'no commands, camera, stage progression, or training workload',
                        'observer death can result from pool cleanup following one failure']}
(out/'meta.json').write_text(json.dumps(meta,indent=2)+'\n')
procs = {}; streams = {}; pids = {}; samples = []
try:
    for item in manifest['instances']:
        name=item['container']; pids[name]=sim_pid(name)
        stdout=(out/(name+'.jsonl')).open('wb'); stderr=(out/(name+'.stderr')).open('wb')
        streams[name]=(stdout,stderr)
        cmd=['docker','exec','-i','-u',str(os.getuid())+':'+str(os.getgid()),'-e','ROS_HOME=/tmp/probe-ros',name,
             'bash','-c','source /opt/ros/noetic/setup.bash; exec python3 - "$1"','pool-probe',str(a.seconds)]
        proc=subprocess.Popen(cmd,stdin=subprocess.PIPE,stdout=stdout,stderr=stderr)
        procs[name]=proc
        proc.stdin.write(source); proc.stdin.close()
    start=time.monotonic(); reason='duration_reached'
    with (out/'host_samples.jsonl').open('w',buffering=1) as f:
        while time.monotonic()-start < a.seconds:
            row={'monotonic_s':time.monotonic(),'wall_ns':time.time_ns(),
                 'processes':{name:stat(pid) for name,pid in pids.items()},
                 'observers':{name:proc.poll() for name,proc in procs.items()}}
            samples.append(row); f.write(json.dumps(row,separators=(',',':'))+'\n')
            if any('error' in x for x in row['processes'].values()):
                reason='simulator_process_missing'; break
            if any(code is not None for code in row['observers'].values()):
                reason='observer_exited_early'; break
            time.sleep(1)
    if reason=='duration_reached':
        for proc in procs.values():
            try: proc.wait(timeout=15)
            except subprocess.TimeoutExpired: reason='observer_finish_timeout'
finally:
    for proc in procs.values():
        if proc.poll() is None: proc.terminate()
    for proc in procs.values():
        try: proc.wait(timeout=5)
        except subprocess.TimeoutExpired:
            proc.kill();proc.wait()
    for pair in streams.values():
        for f in pair: f.close()
summary={}
for name,proc in procs.items():
    rows=[json.loads(x) for x in (out/(name+'.jsonl')).read_text().splitlines() if x.strip()]
    topics={}
    for topic in ('imu','lidar'):
        selected=[r for r in rows if r.get('event')=='sample' and r.get('topic')==topic]
        converted=[[r['monotonic_ns']/1e9,r['wall_ns']/1e9,r['stamp_ns']/1e9,r['sequence']] for r in selected]
        topics[topic]=summarize(converted,a.seconds)
    summary[name]={'pid':pids[name],'observer_exit_code':proc.returncode,'events':{e:sum(r.get('event')==e for r in rows) for e in ('started','finished')},'topics':topics}
data={'reason':reason,'elapsed_s':time.monotonic()-start,'host_sample_count':len(samples),'summary':summary}
(out/'summary.json').write_text(json.dumps(data,indent=2)+'\n')
print(json.dumps(data,indent=2))
sys.exit(0 if reason=='duration_reached' and all(p.returncode==0 for p in procs.values()) else 1)
