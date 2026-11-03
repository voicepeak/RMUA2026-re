#!/usr/bin/env python3
"""Launch bounded single-seed GPS/IMU scout in an unmodified GDB-observed sim."""
import argparse, hashlib, json, os, secrets, subprocess, time
from pathlib import Path
from run_readonly_trace import LAUNCH, docker, sha

def main():
 p=argparse.ArgumentParser(description=__doc__)
 p.add_argument('--seed',type=int,required=True);p.add_argument('--output',type=Path,required=True)
 p.add_argument('--port',type=int,default=11631);p.add_argument('--seconds',type=int,default=350)
 p.add_argument('--display',default=':1');p.add_argument('--image',default='rmua/re-probe:20261104')
 a=p.parse_args();base=Path(__file__).resolve().parent.parent;workspace=Path('/home/tianbot/RUMA-by-helinjun')
 sim=workspace/'main/simulator/simulator_12.0.0.5';settings=workspace/'main/configs/simulator_settings.json'
 out=a.output.resolve();out.mkdir(parents=True,exist_ok=False);name='rmua-scout-'+secrets.token_hex(6)
 m={'seed':a.seed,'port':a.port,'binary_sha256':sha(sim/'Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping'),
    'settings_sha256':sha(settings),'scout_sha256':sha(base/'scripts/stage_sensor_scout.py'),
    'gdb_sha256':sha(base/'scripts/stage_trace.gdb'),'map':'default OL1','mode':'VEL diagnostic',
    'control_inputs':['GPS PoseStamped','IMU','LiDAR','historical stereo image gate estimates'],
    'debug_truth_control':False,'official_result':'unknown','start_wall_unix':time.time(),'status':'starting'}
 def save(): (out/'manifest.json').write_text(json.dumps(m,indent=2)+'\n')
 save();wind=None
 try:
  launch=LAUNCH.replace('runtime_trace.gdb','stage_trace.gdb')
  args=['run','-d','--init','--name',name,'--network','host','--gpus','all','--cap-add','SYS_PTRACE',
   '--security-opt','seccomp=unconfined','--user',str(os.getuid())+':'+str(os.getgid()),'-e','HOME=/tmp',
   '-e','LD_LIBRARY_PATH=','-e','NVIDIA_DRIVER_CAPABILITIES=all','-e','DISPLAY='+a.display,
   '-e','ROS_HOSTNAME=localhost','-e','ROS_MASTER_URI=http://127.0.0.1:'+str(a.port),'-e','RMUA_PORT='+str(a.port),
   '-e','RMUA_SEED='+str(a.seed),'-v',str(sim)+':/sim:ro','-v',str(settings)+':/configs/settings.json:ro',
   '-v',str(workspace)+':/workspace:ro','-v',str(base)+':/analysis:ro','-v',str(out)+':/out',
   '-v','/tmp/.X11-unix:/tmp/.X11-unix','--entrypoint','bash',a.image,'-c',launch]
  m['container_id']=docker(*args,capture_output=True,check=True).stdout.strip();save()
  prefix='source /opt/ros/noetic/setup.bash; source /workspace/rmua_ws/devel/setup.bash; source /workspace/main/.runtime_ws/devel/setup.bash; export OPENBLAS_NUM_THREADS=1; '
  def inside(cmd): return ['docker','exec','-u',str(os.getuid())+':'+str(os.getgid()),name,'bash','-c',prefix+cmd]
  deadline=time.monotonic()+90
  while time.monotonic()<deadline:
   if docker('inspect',name,'--format','{{.State.Running}}',capture_output=True).stdout.strip()!='true':raise RuntimeError('simulator exited before ready')
   if subprocess.run(inside('timeout 2 rostopic echo -n 1 /airsim_node/drone_1/gps >/dev/null'),stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL).returncode==0:break
   time.sleep(.5)
  else: raise TimeoutError('GPS topic not ready')
  m['ready_wall_unix']=time.time();save()
  with (out/'wind_stderr.log').open('w') as e:
   wind=subprocess.Popen(inside('exec python3 /analysis/scripts/stage_wind_recorder.py /out/wind.jsonl'),stdout=e,stderr=subprocess.STDOUT)
   time.sleep(1)
   if wind.poll() is not None:raise RuntimeError('wind recorder exited')
   with (out/'scout.log').open('w') as f:
    try:
     worker=subprocess.run(inside('exec python3 /analysis/scripts/stage_sensor_scout.py /out'),stdout=f,stderr=subprocess.STDOUT,timeout=a.seconds)
     m['scout_exit_code']=worker.returncode;m['status']='scout_exited'
    except subprocess.TimeoutExpired:
     m['status']='scout_timeout'
 except Exception as exc:m.update(status='failed',error=repr(exc))
 finally:
  if wind and wind.poll() is None:wind.terminate()
  with (out/'gdb.log').open('w') as f:docker('logs',name,stdout=f,stderr=subprocess.STDOUT)
  docker('rm','-f',name,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
  if wind:
   try: wind.wait(timeout=3)
   except subprocess.TimeoutExpired: wind.kill()
  m['end_wall_unix']=time.time();save();print(json.dumps(m),flush=True)

if __name__=='__main__':main()
