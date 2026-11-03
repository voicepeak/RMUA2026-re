#!/usr/bin/env python3
"""Bounded idle-scene batch probe with retries; NOT a training episode runner.

Each failed batch is retried from fresh processes, not resumed. Stop/cleanup is
restricted by headless_pool.py to containers matching its manifest and label.
"""
import argparse
import json
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
POOL = ROOT/'scripts'/'headless_pool.py'

def cmd_run(command, logfile):
    with logfile.open('w') as stream:
        return subprocess.run(command, stdout=stream, stderr=subprocess.STDOUT).returncode

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--simulator',type=Path,required=True)
    p.add_argument('--settings',type=Path,required=True)
    p.add_argument('--seeds',required=True)
    p.add_argument('--output',type=Path,required=True,help='new output directory')
    p.add_argument('--port',type=int,default=11800,help='first ROS master port; new range per attempt')
    p.add_argument('--parallelism',type=int,default=4,choices=range(1,9))
    p.add_argument('--retries',type=int,default=1,help='retries for each entire batch')
    p.add_argument('--seconds',type=float,default=15,help='wall-clock liveness window after readiness')
    p.add_argument('--startup-timeout',type=float,default=75)
    p.add_argument('--display',default=':1')
    p.add_argument('--image',default='rmua/noetic-xal:driver-recovery-20261012')
    a=p.parse_args()
    try: seeds=[int(x) for x in a.seeds.split(',')]
    except ValueError: p.error('invalid seeds')
    if not seeds or any(x<0 for x in seeds) or a.seconds<=0 or a.retries<0: p.error('invalid arguments')
    groups=[seeds[i:i+a.parallelism] for i in range(0,len(seeds),a.parallelism)]
    if a.port<1 or a.port+len(groups)*(a.retries+1)*a.parallelism-1>65535: p.error('invalid port range')
    out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
    results={'kind':'idle_diagnostic_only','seconds_wall':a.seconds,'batches':[]}
    def save(): (out/'results.json').write_text(json.dumps(results,indent=2)+'\n')
    save()
    serial=0
    for number,group in enumerate(groups):
        result={'batch':number,'seeds':group,'attempts':[],'success':False};results['batches'].append(result);save()
        for attempt in range(a.retries+1):
            port=a.port+serial*a.parallelism; serial+=1
            prefix='batch%02d_attempt%02d'%(number,attempt)
            pool_dir=out/(prefix+'_pool'); manifest=pool_dir/'pool.json'
            trial={'attempt':attempt,'port':port,'success':False};result['attempts'].append(trial);save()
            start=[sys.executable,str(POOL),'start','--simulator',str(a.simulator),'--settings',str(a.settings),
                   '--seeds',','.join(map(str,group)),'--port',str(port),'--output',str(pool_dir),
                   '--display',a.display,'--image',a.image,'--timeout',str(a.startup_timeout)]
            try:
                trial['start_exit_code']=cmd_run(start,out/(prefix+'_start.log'))
                if trial['start_exit_code']!=0:
                    trial['reason']='startup failed';continue
                data=json.loads(manifest.read_text());deadline=time.monotonic()+a.seconds
                trial['ready_wall_unix_s']=time.time()
                with (out/(prefix+'_liveness.jsonl')).open('w') as stream:
                    while time.monotonic()<deadline:
                        row={'wall_unix_s':time.time(),'monotonic_s':time.monotonic(),'containers':{}}
                        ok=True
                        for item in data['instances']:
                            proc=subprocess.run(['docker','inspect','--format','{{json .State}}',item['id']],
                                                text=True,capture_output=True)
                            try: state=json.loads(proc.stdout) if proc.returncode==0 else {'error':proc.stderr.strip()}
                            except json.JSONDecodeError: state={'error':'invalid inspect JSON'}
                            row['containers'][item['container']]=state
                            if not state.get('Running'):ok=False
                        stream.write(json.dumps(row)+'\n');stream.flush()
                        if not ok:trial['reason']='instance exited during observation';break
                        time.sleep(min(1,max(0,deadline-time.monotonic())))
                    else:
                        trial['success']=True;result['success']=True
            finally:
                if manifest.exists():
                    trial['cleanup_exit_code']=cmd_run([sys.executable,str(POOL),'stop',str(manifest)],out/(prefix+'_stop.log'))
                    if trial['cleanup_exit_code']!=0:
                        trial['success']=False;result['success']=False;trial['reason']='cleanup failed'
                save()
                if trial.get('cleanup_exit_code',0)!=0:
                    raise RuntimeError('cleanup failed; refusing to start another batch')
            if result['success']:break
        if not result['success']:
            print('Batch %d failed; evidence: %s'%(number,out),file=sys.stderr)
            return 1
    print('Idle batches observed: '+str(out))
    return 0

if __name__=='__main__':
    try:sys.exit(main())
    except (OSError,ValueError,RuntimeError,subprocess.SubprocessError) as exc:
        print('pool-batches:',exc,file=sys.stderr);sys.exit(1)
