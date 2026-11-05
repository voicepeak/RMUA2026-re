#!/usr/bin/env python3
"""Force one outdoor stage at initial route setup using GDB register injection."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import secrets
import socket
import subprocess
import sys
import time

LAUNCH = r'''set -e
source /opt/ros/noetic/setup.bash
export ROS_HOME=/tmp/ros
mkdir -p "$ROS_HOME"
roscore -p "$RMUA_PORT" > /out/roscore.log 2>&1 &
ros_pid=$!
trap 'kill "$ros_pid" 2>/dev/null || true' EXIT
for i in {1..120}; do rosparam list >/dev/null 2>&1 && break; sleep .25; done
rosparam list >/dev/null 2>&1 || exit 2
cd /sim/Build/LinuxNoEditor
gdb -nx -batch -x /analysis/scripts/stage_reset_cache_trace.gdb --args ./RMUA/Binaries/Linux/RMUA-Linux-Shipping RMUA seed "$RMUA_SEED" -nullrhi -settings=/configs/settings.json
'''

def sha(path):
    h=hashlib.sha256()
    with path.open('rb') as f:
        for b in iter(lambda:f.read(1024*1024),b''): h.update(b)
    return h.hexdigest()

def docker(*args,**kw):
    return subprocess.run(['docker',*args],text=True,**kw)

def inside(name, command, timeout):
    return docker('exec','-u',str(os.getuid())+':'+str(os.getgid()),'-e','ROS_HOME=/tmp/probe',
                  name,'bash','-c','source /opt/ros/noetic/setup.bash; source /workspace/rmua_ws/devel/setup.bash; '+command,
                  capture_output=True,timeout=timeout)

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--stage',type=int,choices=(4,5,6),required=True)
    p.add_argument('--simulator',type=Path,required=True)
    p.add_argument('--settings',type=Path,required=True)
    p.add_argument('--messages',type=Path,required=True,help='ROS catkin workspace with airsim_ros services')
    p.add_argument('--output',type=Path,required=True)
    p.add_argument('--seeds',default='123,456,2026')
    p.add_argument('--repeats',type=int,default=2)
    p.add_argument('--port',type=int,default=11501)
    p.add_argument('--sample-seconds',type=int,default=5)
    p.add_argument('--image',default='rmua/re-probe:20261104')
    p.add_argument('--display',default=':1')
    p.add_argument('--reset',action='store_true')
    a=p.parse_args()
    if not 1<=a.repeats<=10 or not 2<=a.sample_seconds<=30: p.error('invalid repeats or duration')
    seeds=[int(x) for x in a.seeds.split(',')]
    sim=a.simulator.resolve(strict=True);settings=a.settings.resolve(strict=True);messages=a.messages.resolve(strict=True)
    binary=sim/'Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping'
    if not binary.is_file(): p.error('simulator binary missing')
    output=a.output.resolve();output.mkdir(parents=True,exist_ok=False)
    base=Path(__file__).resolve().parent.parent
    image=docker('image','inspect',a.image,'--format','{{.Id}}',capture_output=True,check=True).stdout.strip()
    manifest={'binary_sha256':sha(binary),'settings_sha256':sha(settings),'image_id':image,
              'gdb_script_sha256':sha(base/'scripts/stage_reset_cache_trace.gdb'), 'runner_sha256':sha(Path(__file__)),
              'mode':'nullrhi','map':'default Lobby -> OL1','seeds':seeds,'repeats':a.repeats,
              'stage_coverage':'GDB mutates initial stage call arguments; diagnostic, not official progression','injected_stage':a.stage,'runs':[]}
    manifest_path=output/'manifest.json'
    def save(): manifest_path.write_text(json.dumps(manifest,indent=2)+'\n')
    save()
    run_index=0
    for seed in seeds:
      for rep in range(1,a.repeats+1):
        port=a.port+run_index;run_index+=1
        run=output/('seed%d_repeat%d'%(seed,rep));run.mkdir()
        name='rmua-trace-'+secrets.token_hex(6)
        entry={'seed':seed,'repeat':rep,'port':port,'container':name,'start_wall':time.time(),'status':'starting'}
        manifest['runs'].append(entry);save()
        try:
            with socket.socket() as s:
                s.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
                s.bind(('127.0.0.1',port))
            cmd=['run','-d','--init','--name',name,'--network','host','--gpus','all',
                 '--cap-add','SYS_PTRACE','--security-opt','seccomp=unconfined',
                 '--user',str(os.getuid())+':'+str(os.getgid()),'-e','HOME=/tmp','-e','LD_LIBRARY_PATH=',
                 '-e','NVIDIA_DRIVER_CAPABILITIES=all','-e','DISPLAY='+a.display,
                 '-e','ROS_HOSTNAME=localhost','-e','ROS_MASTER_URI=http://127.0.0.1:'+str(port),
                 '-e','RMUA_PORT='+str(port),'-e','RMUA_SEED='+str(seed),'-e','RMUA_STAGE='+str(a.stage),
                 '-v',str(sim)+':/sim:ro','-v',str(settings)+':/configs/settings.json:ro',
                 '-v',str(messages)+':/workspace/rmua_ws:ro',
                 '-v',str(base)+':/analysis:ro','-v',str(run)+':/out','-v','/tmp/.X11-unix:/tmp/.X11-unix',
                 '--entrypoint','bash',a.image,'-c',LAUNCH]
            x=docker(*cmd,capture_output=True,check=True);entry['id']=x.stdout.strip()
            deadline=time.monotonic()+90
            while time.monotonic()<deadline:
                state=docker('inspect',name,'--format','{{.State.Running}}',capture_output=True)
                if state.stdout.strip()!='true': raise RuntimeError('simulator container exited before readiness')
                check=inside(name,'timeout 3 rostopic echo -n 1 /airsim_node/drone_1/imu/imu >/dev/null',8)
                if check.returncode==0: break
                time.sleep(.5)
            else: raise TimeoutError('IMU topic not ready')
            entry['ready_wall']=time.time();save()
            for phase in (['before_reset','after_reset'] if a.reset else ['before_reset']):
                if phase=='after_reset':
                    result=inside(name,'timeout 8 rosservice call /airsim_node/reset',12)
                    (run/'reset.log').write_text(result.stdout+result.stderr)
                    entry['reset_exit_code']=result.returncode
                    entry['reset_response_success']=(result.returncode==0 and 'success: True' in result.stdout)
                    time.sleep(1)
                result=inside(name,'timeout %d rostopic echo -p /airsim_node/drone_1/debug/wind'%a.sample_seconds,a.sample_seconds+8)
                (run/(phase+'_wind.csv')).write_text(result.stdout)
                (run/(phase+'_wind.stderr')).write_text(result.stderr)
                entry[phase+'_exit_code']=result.returncode
            entry['status']='observed' if not a.reset or entry.get('reset_response_success') else 'reset_failed'
        except (OSError,RuntimeError,subprocess.SubprocessError) as exc:
            entry['status']='failed';entry['error']=str(exc)
        finally:
            with (run/'gdb.log').open('w') as f: docker('logs',name,stdout=f,stderr=subprocess.STDOUT)
            docker('rm','-f',name,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
            log=(run/'gdb.log').read_text(errors='replace')
            entry['injection_observed']='INJECT state=' in log
            entry['wind_generation_observed']='TRACE wind ' in log
            entry['line_generation_observed']='TRACE line ' in log
            if not all(entry[k] for k in ('injection_observed','wind_generation_observed','line_generation_observed')): entry['status']='incomplete'
            entry['end_wall']=time.time();save()
        print(json.dumps(entry),flush=True)
    print('manifest:',manifest_path,flush=True)

if __name__=='__main__':
    try:main()
    except (ValueError,OSError,subprocess.SubprocessError) as e: print(str(e),file=sys.stderr);sys.exit(1)
