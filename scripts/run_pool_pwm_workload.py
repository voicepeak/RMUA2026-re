#!/usr/bin/env python3
"""Run bounded independent NullRHI instances with fixed PWM and sensor observers."""
import argparse,hashlib,json,os,subprocess,sys,time
from pathlib import Path
ROOT=Path(__file__).resolve().parent.parent
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--simulator',type=Path,required=True);p.add_argument('--settings',type=Path,required=True)
p.add_argument('--seeds',required=True);p.add_argument('--output',type=Path,required=True)
p.add_argument('--port',type=int,default=12700);p.add_argument('--seconds',type=float,default=300)
p.add_argument('--image',default='rmua/re-probe:20261104');p.add_argument('--display',default=':1')
a=p.parse_args()
if not 6<=a.seconds<=590:p.error('duration must be 6..590 seconds')
seeds=[int(x) for x in a.seeds.split(',')]
if not 1<=len(seeds)<=8 or any(x<0 for x in seeds):p.error('provide one to eight nonnegative seeds')
out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
manifest=out/'pool'/'pool.json';state={'status':'starting','seeds':seeds,'requested_workload_s':a.seconds,
 'limitations':['fixed open-loop PWM is a synthetic workload, not a complete flight or training task',
                'observer and publisher processes add load; topic reception and applied PWM require independent validation',
                'nullrhi camera and episode reset are not validated']}
def save():(out/'workload_manifest.json').write_text(json.dumps(state,indent=2)+'\n')
save();actions=[];streams=[];observer=None;pool_started=False
try:
 start=[sys.executable,str(ROOT/'scripts/headless_pool.py'),'start','--simulator',str(a.simulator),'--settings',str(a.settings),
  '--seeds',a.seeds,'--port',str(a.port),'--output',str(out/'pool'),'--display',a.display,'--image',a.image,'--timeout','90']
 with (out/'start.log').open('w') as f: result=subprocess.run(start,stdout=f,stderr=subprocess.STDOUT)
 state['start_exit_code']=result.returncode
 if result.returncode:raise RuntimeError('pool startup failed; inspect start.log and manifest')
 pool_started=True;pool=json.loads(manifest.read_text());state['pool_id']=pool['pool_id'];state['status']='running';save()
 oo=(out/'observer.stdout').open('w');oe=(out/'observer.stderr').open('w');streams.extend([oo,oe])
 observer=subprocess.Popen([sys.executable,str(ROOT/'scripts/measure_pool_stream.py'),str(manifest),'--seconds',str(a.seconds+10),'--workload','fixed_pwm','--output',str(out/'stream')],stdout=oo,stderr=oe)
 source=(ROOT/'scripts/pool_pwm_workload.py').read_bytes();state['publisher_sha256']=hashlib.sha256(source).hexdigest();save()
 for item in pool['instances']:
  name=item['container'];stdout=(out/(name+'.commands.jsonl')).open('wb');stderr=(out/(name+'.commands.stderr')).open('wb');streams.extend([stdout,stderr])
  command=['docker','exec','-i','-u',str(os.getuid())+':'+str(os.getgid()),'-e','ROS_HOME=/tmp/pwm-workload',name,
           'bash','-c','source /opt/ros/noetic/setup.bash; exec python3 - "$1"','pool-pwm',str(a.seconds)]
  proc=subprocess.Popen(command,stdin=subprocess.PIPE,stdout=stdout,stderr=stderr);actions.append((item,proc));proc.stdin.write(source);proc.stdin.close()
 start_mono=time.monotonic();state['observer_pid']=observer.pid;state['publisher_pids']={item['container']:proc.pid for item,proc in actions};save()
 with (out/'liveness.jsonl').open('w',buffering=1) as f:
  while time.monotonic()-start_mono<a.seconds+35:
   row={'monotonic_ns':time.monotonic_ns(),'wall_ns':time.time_ns(),'containers':{},'observer_exit':observer.poll(),
        'publisher_exits':{item['container']:proc.poll() for item,proc in actions}}
   for item,_ in actions:
    q=subprocess.run(['docker','inspect','--format','{{json .State}}',item['id']],text=True,capture_output=True)
    try:status=json.loads(q.stdout) if q.returncode==0 else {'error':q.stderr.strip()}
    except json.JSONDecodeError:status={'error':'invalid docker inspect JSON'}
    row['containers'][item['container']]=status
   f.write(json.dumps(row,separators=(',',':'))+'\n')
   if any(not x.get('Running') for x in row['containers'].values()):state['reason']='container exited';break
   if any(x not in (None,0) for x in row['publisher_exits'].values()):state['reason']='publisher failed';break
   if row['observer_exit'] not in (None,0):state['reason']='observer failed';break
   if all(x==0 for x in row['publisher_exits'].values()) and row['observer_exit']==0:
    state['status']='observed';state['reason']='all observers and publishers completed';break
   time.sleep(1)
  else:state['reason']='workload deadline expired'
except Exception as exc:
 state['error']=repr(exc);state['status']='failed'
finally:
 for _,proc in actions:
  if proc.poll() is None:proc.terminate()
 for _,proc in actions:
  try:proc.wait(timeout=5)
  except subprocess.TimeoutExpired:proc.kill();proc.wait()
 state['publisher_exit_codes']={item['container']:proc.returncode for item,proc in actions}
 if observer:
  if observer.poll() is None:observer.terminate()
  try:observer.wait(timeout=5)
  except subprocess.TimeoutExpired:observer.kill();observer.wait()
  state['observer_exit_code']=observer.returncode
 for f in streams:f.close()
 if state['status']=='observed':
  quality={}
  for item,_ in actions:
   name=item['container'];path=out/(name+'.commands.jsonl')
   try:
    rows=[json.loads(x) for x in path.read_text().splitlines() if x.strip()]
    ready=next((x for x in rows if x.get('event')=='publisher_ready'),None)
    finish=next((x for x in rows if x.get('event')=='finished'),None)
    quality[name]=bool(ready and ready.get('connections',0)>0 and finish and finish.get('elapsed_s',0)>=a.seconds)
   except (OSError,ValueError):quality[name]=False
  state['publisher_duration_checks']=quality
  if not all(quality.values()):state['status']='incomplete';state['reason']='publisher connection or duration check failed'
 if pool_started or manifest.exists():
  with (out/'stop.log').open('w') as f:cleanup=subprocess.run([sys.executable,str(ROOT/'scripts/headless_pool.py'),'stop',str(manifest)],stdout=f,stderr=subprocess.STDOUT)
  state['cleanup_exit_code']=cleanup.returncode
  if cleanup.returncode:state['status']='failed';state['reason']='cleanup failed'
 if state['status']=='running':state['status']='failed'
 state['end_wall_ns']=time.time_ns();save()
print(json.dumps(state,indent=2));sys.exit(0 if state['status']=='observed' else 1)
