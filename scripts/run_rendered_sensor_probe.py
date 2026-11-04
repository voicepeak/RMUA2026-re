#!/usr/bin/env python3
"""Run one owned rendered official scene with passive ROS sensor clock observation."""
import argparse,hashlib,json,os,secrets,socket,subprocess,time
from pathlib import Path
LAUNCH=r'''set -e
source /opt/ros/noetic/setup.bash
export ROS_HOME=/tmp/ros
mkdir -p "$ROS_HOME"
roscore -p "$RMUA_PORT" > /out/roscore.log 2>&1 &
r=$!; s=
cleanup(){ if [ -n "$s" ]; then kill "$s" 2>/dev/null || true; fi; kill "$r" 2>/dev/null || true; }
trap cleanup EXIT TERM INT
for i in {1..120}; do rosparam list >/dev/null 2>&1 && break; sleep .25; done
rosparam list >/dev/null 2>&1
cd /sim/Build/LinuxNoEditor
./RMUA/Binaries/Linux/RMUA-Linux-Shipping RMUA seed "$RMUA_SEED" -RenderOffscreen -settings=/configs/settings.json > /out/simulator.log 2>&1 &
s=$!
for i in {1..100}; do timeout 2 rostopic echo -n 1 /airsim_node/drone_1/imu/imu >/dev/null 2>&1 && break; kill -0 "$s"; sleep .2; done
python3 /analysis/scripts/sensor_clock_probe.py /out/samples.jsonl "$RMUA_SECONDS"
'''
def sha(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for b in iter(lambda:f.read(1048576),b''):h.update(b)
 return h.hexdigest()
def run(*a,**k):return subprocess.run(['docker',*a],text=True,**k)
p=argparse.ArgumentParser(description=__doc__)
p.add_argument('--simulator',type=Path,required=True);p.add_argument('--settings',type=Path,required=True);p.add_argument('--output',type=Path,required=True)
p.add_argument('--seed',type=int,default=123);p.add_argument('--port',type=int,default=11681);p.add_argument('--seconds',type=int,default=15);p.add_argument('--display',default=':1');p.add_argument('--image',default='rmua/noetic-xal:driver-recovery-20261012');a=p.parse_args()
if a.seed<0 or not 1<=a.port<=65535 or not 1<=a.seconds<=120:p.error('invalid seed, port, or duration')
sim=a.simulator.resolve(strict=True);settings=a.settings.resolve(strict=True);binary=sim/'Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping'
with socket.socket() as s:s.bind(('127.0.0.1',a.port))
out=a.output.resolve();out.mkdir(parents=True,exist_ok=False);repo=Path(__file__).resolve().parent.parent;name='rmua-sensor-'+secrets.token_hex(6)
image=run('image','inspect',a.image,'--format','{{.Id}}',check=True,capture_output=True).stdout.strip()
meta={'seed':a.seed,'port':a.port,'mode':'RenderOffscreen','binary_sha256':sha(binary),'settings_sha256':sha(settings),'observer_sha256':sha(repo/'scripts/sensor_clock_probe.py'),'image_id':image,'start_wall_unix_s':time.time(),'start_monotonic_s':time.monotonic(),'status':'starting','container':name}
def save():(out/'manifest.json').write_text(json.dumps(meta,indent=2)+'\n')
save()
try:
 c=['run','-d','--init','--name',name,'--network','host','--gpus','all','--user',f'{os.getuid()}:{os.getgid()}',
 '-e','HOME=/tmp','-e','LD_LIBRARY_PATH=','-e','NVIDIA_DRIVER_CAPABILITIES=all','-e','DISPLAY='+a.display,'-e','ROS_HOSTNAME=localhost',
 '-e','ROS_MASTER_URI=http://127.0.0.1:'+str(a.port),'-e','RMUA_PORT='+str(a.port),'-e','RMUA_SEED='+str(a.seed),'-e','RMUA_SECONDS='+str(a.seconds),
 '-v',str(sim)+':/sim:ro','-v',str(settings)+':/configs/settings.json:ro','-v',str(repo)+':/analysis:ro','-v',str(out)+':/out','-v','/tmp/.X11-unix:/tmp/.X11-unix',
 '--entrypoint','bash',a.image,'-c',LAUNCH]
 x=run(*c,check=True,capture_output=True);meta['container_id']=x.stdout.strip();save()
 w=run('wait',name,check=True,capture_output=True,timeout=120+a.seconds);meta['container_exit_code']=int(w.stdout.strip());meta['status']='observed' if meta['container_exit_code']==0 else 'failed'
finally:
 logs=run('logs',name,capture_output=True)
 (out/'docker.log').write_text(logs.stdout+logs.stderr)
 x=run('inspect',name,'--format','{{.Id}}',capture_output=True)
 if x.returncode==0 and x.stdout.strip()==meta.get('container_id'):run('rm','-f',name,capture_output=True)
 meta['end_wall_unix_s']=time.time();meta['end_monotonic_s']=time.monotonic();save()
print(json.dumps(meta));raise SystemExit(0 if meta['status']=='observed' else 1)
