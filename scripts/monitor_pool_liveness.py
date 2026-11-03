#!/usr/bin/env python3
"""Read-only liveness and host memory evidence for an existing pool.

Wall timestamps are Unix seconds; duration and cadence use CLOCK_MONOTONIC.
The script does not start, reset, control, or stop the simulator.
"""
import argparse
import json
import os
import subprocess
import time
from pathlib import Path
from measure_headless_pool import sim_pid, stat

p = argparse.ArgumentParser(description=__doc__)
p.add_argument('manifest', type=Path)
p.add_argument('--seconds', type=float, default=90)
p.add_argument('--interval', type=float, default=1)
p.add_argument('--output', type=Path, required=True)
p.add_argument('--processes',action='store_true',help='sample simulator host PID, CPU ticks, RSS and host TCP listeners')
a = p.parse_args()
if a.seconds <= 0 or a.interval <= 0: p.error('duration and interval must be positive')
pool = json.loads(a.manifest.read_text())
start = time.monotonic()
with a.output.open('w') as stream:
    while time.monotonic()-start < a.seconds:
        mem = {}
        for line in Path('/proc/meminfo').read_text().splitlines():
            if line.startswith(('MemAvailable:', 'MemFree:', 'SwapFree:')):
                key, value = line.split(':',1); mem[key] = value.strip()
        row = {'wall_unix_s':time.time(), 'monotonic_s':time.monotonic(), 'meminfo':mem, 'containers':{}}
        if a.processes:
            row['clock_ticks_per_second']=os.sysconf('SC_CLK_TCK')
            listeners=subprocess.run(['ss','-lntp'],text=True,capture_output=True)
            row['host_tcp_listeners']=listeners.stdout
            row['host_tcp_listeners_error']=listeners.stderr
        for item in pool['instances']:
            proc = subprocess.run(['docker','inspect','--format','{{json .State}}',item['id']],
                                  text=True, capture_output=True)
            try: state=json.loads(proc.stdout) if proc.returncode==0 else {'error':proc.stderr.strip()}
            except json.JSONDecodeError: state={'error':'invalid docker inspect JSON'}
            data={'docker_state':state,'ros_master_port':item['port']}
            if a.processes and state.get('Running'):
                try:
                    pid=sim_pid(item['container'])
                    data['simulator_host_pid']=pid
                    data['simulator_proc_stat']=stat(pid)
                except (OSError,RuntimeError,subprocess.SubprocessError) as exc:
                    data['simulator_error']=str(exc)
            row['containers'][item['container']] = data
        stream.write(json.dumps(row,sort_keys=True)+'\n'); stream.flush()
        time.sleep(a.interval)
