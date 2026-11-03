#!/usr/bin/env python3
"""Capture 8 isolated NullRHI processes under GDB; this is idle diagnostics only."""
import argparse
import json
import subprocess
import sys
import time
from pathlib import Path

ROOT=Path(__file__).resolve().parent

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--simulator',type=Path,required=True)
    p.add_argument('--settings',type=Path,required=True)
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--seeds',default='123,456,2026,789,42,1005,1007,2024')
    p.add_argument('--port',type=int,default=11900)
    p.add_argument('--display',default=':1')
    p.add_argument('--seconds',type=int,default=90,help='wall-clock monitoring duration, starting after containers are created')
    p.add_argument('--image',default='rmua/re-probe:20261104')
    a=p.parse_args()
    if a.seconds<1:p.error('seconds must be positive')
    seeds=a.seeds.split(',')
    if len(seeds)!=8:p.error('provide exactly 8 seeds')
    out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    pool=out/'pool';manifest=pool/'pool.json'
    meta={'start_wall_unix_s':time.time(),'start_monotonic_s':time.monotonic(),'seeds':seeds,
          'note':'idle diagnostic only; GDB can perturb timings, no rule or binary changes'}
    def save(): (out/'run.json').write_text(json.dumps(meta,indent=2)+'\n')
    save();start_proc=None;monitor=None
    try:
        command=[sys.executable,str(ROOT/'headless_pool.py'),'start','--simulator',str(a.simulator),
                 '--settings',str(a.settings),'--output',str(pool),'--seeds',a.seeds,'--port',str(a.port),
                 '--image',a.image,'--display',a.display,'--timeout','120','--gdb-crash']
        with (out/'start.log').open('w') as stream:
            start_proc=subprocess.Popen(command,stdout=stream,stderr=subprocess.STDOUT)
            deadline=time.monotonic()+30
            while time.monotonic()<deadline:
                try: data=json.loads(manifest.read_text())
                except (OSError,json.JSONDecodeError): data={}
                if len(data.get('instances',[]))==8:break
                if start_proc.poll() is not None:break
                time.sleep(.1)
            if len(data.get('instances',[]))==8:
                monitor_cmd=[sys.executable,str(ROOT/'monitor_pool_liveness.py'),str(manifest),
                             '--seconds',str(a.seconds),'--interval','1','--processes',
                             '--output',str(out/'liveness.jsonl')]
                with (out/'monitor.log').open('w') as log:
                    monitor=subprocess.Popen(monitor_cmd,stdout=log,stderr=subprocess.STDOUT)
                    meta['monitor_start_wall_unix_s']=time.time();save()
                    while monitor.poll() is None:
                        if start_proc.poll() not in (None,0):
                            meta['start_exit_code']=start_proc.returncode
                            monitor.terminate();break
                        time.sleep(.25)
                    try:meta['monitor_exit_code']=monitor.wait(timeout=15)
                    except subprocess.TimeoutExpired: monitor.kill();meta['monitor_exit_code']=monitor.wait()
            if start_proc.poll() is None:
                start_proc.terminate()
                try:start_proc.wait(timeout=15)
                except subprocess.TimeoutExpired:start_proc.kill();start_proc.wait()
            meta['start_exit_code']=start_proc.returncode
    finally:
        if monitor and monitor.poll() is None:
            monitor.terminate();monitor.wait(timeout=15)
        if start_proc and start_proc.poll() is None:
            start_proc.terminate();start_proc.wait(timeout=15)
        if manifest.exists():
            with (out/'cleanup.log').open('w') as log:
                stop=subprocess.run([sys.executable,str(ROOT/'headless_pool.py'),'stop',str(manifest)],stdout=log,stderr=subprocess.STDOUT)
            meta['cleanup_exit_code']=stop.returncode
            meta['sigsegv_logs']=[str(p.relative_to(out)) for p in sorted(pool.glob('*.log'))
                                  if 'received signal SIGSEGV' in p.read_text(errors='replace')]
        samples=out/'liveness.jsonl'
        if samples.exists():
            meta['observed_exited_containers']=sorted({name for line in samples.read_text().splitlines()
                for name,info in json.loads(line)['containers'].items()
                if info['docker_state'].get('Status') in ('exited','dead')})
        meta['end_wall_unix_s']=time.time();meta['end_monotonic_s']=time.monotonic();save()
    return 0 if (meta.get('start_exit_code')==0 and meta.get('monitor_exit_code')==0 and
                 meta.get('cleanup_exit_code')==0 and not meta.get('sigsegv_logs') and
                 not meta.get('observed_exited_containers')) else 1

if __name__=='__main__':sys.exit(main())
