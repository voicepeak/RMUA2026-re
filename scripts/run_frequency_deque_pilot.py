#!/usr/bin/env python3
"""Run one instrumented NullRHI process and record ROS transport timestamps."""
import argparse,hashlib,json,os,secrets,socket,subprocess,time
from pathlib import Path

ROOT=Path(__file__).resolve().parent.parent
LAUNCH=r'''set -e
source /opt/ros/noetic/setup.bash
export ROS_HOME=/tmp/ros
mkdir -p "$ROS_HOME"
roscore -p "$RMUA_PORT" > /out/roscore.log 2>&1 &
ros_pid=$!
trap 'kill "$ros_pid" 2>/dev/null || true' EXIT
for i in {1..120}; do rosparam list >/dev/null 2>&1 && break; sleep .25; done
rosparam list >/dev/null 2>&1 || exit 2
cd /sim/Build/LinuxNoEditor
LD_PRELOAD=/probe/probe.so RMUA_FREQ_GUARD_LOG=/out/frequency_guard.jsonl ./RMUA/Binaries/Linux/RMUA-Linux-Shipping RMUA seed "$RMUA_SEED" -nullrhi -settings=/configs/settings.json
'''
def sha(path):
 h=hashlib.sha256()
 with path.open('rb') as f:
  for b in iter(lambda:f.read(1048576),b''): h.update(b)
 return h.hexdigest()
def docker(*args,**kw): return subprocess.run(['docker',*args],text=True,**kw)
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--simulator',type=Path,required=True);p.add_argument('--settings',type=Path,required=True)
p.add_argument('--messages',type=Path,required=True);p.add_argument('--preload',type=Path,required=True)
p.add_argument('--output',type=Path,required=True);p.add_argument('--seed',type=int,default=123)
p.add_argument('--port',type=int,default=11820);p.add_argument('--seconds',type=int,default=12)
p.add_argument('--image',default='rmua/re-probe:20261104');p.add_argument('--display',default=':1')
a=p.parse_args()
if not 6<=a.seconds<=60: p.error('seconds must be 6..60')
sim=a.simulator.resolve(strict=True);settings=a.settings.resolve(strict=True);messages=a.messages.resolve(strict=True);preload=a.preload.resolve(strict=True)
binary=sim/'Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping';library=sim/'Build/LinuxNoEditor/RMUA/Plugins/AirSimROS/Binaries/Linux/libairsimros.so'
expected='69d5ea3302bdd5f20b842ec3d3e84bafcb50ef3649ada42c4d346c9bcd389885'
if sha(library)!=expected: p.error('unexpected AirSim ROS library; probe ABI not validated')
with socket.socket() as s:s.bind(('127.0.0.1',a.port))
out=a.output.resolve();out.mkdir(parents=True,exist_ok=False)
name='rmua-pubprobe-'+secrets.token_hex(6)
manifest={'binary_sha256':sha(binary),'library_sha256':sha(library),'settings_sha256':sha(settings),'preload_sha256':sha(preload),
 'runner_sha256':sha(Path(__file__)),'source_sha256':sha(ROOT/'scripts/frequency_deque_overlap.cpp'),
 'sensor_probe_sha256':sha(ROOT/'scripts/sensor_clock_probe.py'),'command_probe_sha256':sha(ROOT/'scripts/pwm_wind_step_probe.py'),
 'guard_scope':'tracks push_back, pop_front, size PLT calls nested within the three exported frequency functions; exact deque pointer','seed':a.seed,'seconds':a.seconds,'port':a.port,'container':name,'start_wall_ns':time.time_ns(),'status':'starting',
 'limitations':['deque method wrapper overlap does not prove simultaneous memory instructions; instrumentation changes timing and misses inline/direct/iterator accesses', 'open-loop command sequence; no truth used online',
                'header ABI and source-to-subscriber pairing require verification','NullRHI single process']}
def save():(out/'manifest.json').write_text(json.dumps(manifest,indent=2)+'\n')
save();procs=[];opened=[]
try:
 cmd=['run','-d','--init','--name',name,'--network','host','--gpus','all','--user',str(os.getuid())+':'+str(os.getgid()),
      '-e','HOME=/tmp','-e','LD_LIBRARY_PATH=','-e','NVIDIA_DRIVER_CAPABILITIES=all','-e','DISPLAY='+a.display,
      '-e','ROS_HOSTNAME=localhost','-e','ROS_MASTER_URI=http://127.0.0.1:'+str(a.port),'-e','RMUA_PORT='+str(a.port),'-e','RMUA_SEED='+str(a.seed),
      '-v',str(sim)+':/sim:ro','-v',str(settings)+':/configs/settings.json:ro','-v',str(messages)+':/workspace/rmua_ws:ro',
      '-v',str(ROOT)+':/analysis:ro','-v',str(preload)+':/probe/probe.so:ro','-v',str(out)+':/out','-v','/tmp/.X11-unix:/tmp/.X11-unix',
      '--entrypoint','bash',a.image,'-c',LAUNCH]
 manifest['image_id']=docker('image','inspect',a.image,'--format','{{.Id}}',capture_output=True,check=True).stdout.strip()
 manifest['id']=docker(*cmd,capture_output=True,check=True).stdout.strip();save()
 deadline=time.monotonic()+90
 while time.monotonic()<deadline:
  status=docker('inspect',name,'--format','{{.State.Running}}',capture_output=True)
  if status.stdout.strip()!='true':raise RuntimeError('container exited before ready')
  check=docker('exec',name,'bash','-c','source /opt/ros/noetic/setup.bash; timeout 3 rostopic echo -n 1 /airsim_node/drone_1/imu/imu >/dev/null',capture_output=True,timeout=6)
  if check.returncode==0:break
  time.sleep(.5)
 else:raise TimeoutError('IMU not ready')
 manifest['ready_wall_ns']=time.time_ns();save()
 jobs=[('sensor_clock','python3 /analysis/scripts/sensor_clock_probe.py /out/sensor_clock.jsonl '+str(a.seconds)),
       ('pwm_output','python3 /analysis/scripts/pwm_any_probe.py /out/pwm_output.jsonl '+str(a.seconds)),
       ('commands','python3 /analysis/scripts/pwm_wind_step_probe.py /out/commands.jsonl')]
 for label,job in jobs:
  stdout=(out/(label+'.stdout')).open('w');stderr=(out/(label+'.stderr')).open('w');opened.extend([stdout,stderr])
  command='source /opt/ros/noetic/setup.bash; source /workspace/rmua_ws/devel/setup.bash; '+job
  proc=subprocess.Popen(['docker','exec','-u',str(os.getuid())+':'+str(os.getgid()),'-e','ROS_HOME=/tmp/probe',name,'bash','-c',command],stdout=stdout,stderr=stderr)
  procs.append((label,proc))
 for label,proc in procs:manifest[label+'_exit_code']=proc.wait(timeout=a.seconds+25)
 manifest['status']='observed' if all(proc.returncode==0 for _,proc in procs) else 'incomplete'
except Exception as exc:
 manifest['status']='error';manifest['error']=repr(exc)
finally:
 for _,proc in procs:
  if proc.poll() is None:proc.terminate();proc.wait(timeout=5)
 for stream in opened:stream.close()
 log=docker('logs',name,capture_output=True);(out/'simulator.log').write_text(log.stdout+log.stderr)
 stop=docker('rm','-f',name,capture_output=True);manifest['cleanup_exit_code']=stop.returncode
 manifest['end_wall_ns']=time.time_ns();save()
print(json.dumps(manifest,indent=2))
if manifest['status']!='observed' or manifest['cleanup_exit_code']!=0:raise SystemExit(1)
