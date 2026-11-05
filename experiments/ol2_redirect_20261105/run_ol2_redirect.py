#!/usr/bin/env python3
"""Bounded GDB observation of an unmodified local simulator; optional official reset."""
import argparse
import hashlib
import json
import os
import re
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
gdb -nx -batch -x /analysis/scripts/runtime_ol2_redirect.gdb --args ./RMUA/Binaries/Linux/RMUA-Linux-Shipping RMUA seed "$RMUA_SEED" -nullrhi -settings=/configs/settings.json "-ini:Engine:[/Script/EngineSettings.GameMapsSettings]:GameDefaultMap=$RMUA_MAP"
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
    p.add_argument('--readiness-timeout',type=int,default=90)
    p.add_argument('--overlay', type=Path, required=True)
    p.add_argument('--map-url',default='/Game/Maps/RMUA2024OL/RMUA2024OL2')
    a=p.parse_args()
    if not 1<=a.repeats<=10 or not 2<=a.sample_seconds<=30: p.error('invalid repeats or duration')
    seeds=[int(x) for x in a.seeds.split(',')]
    sim=a.simulator.resolve(strict=True);settings=a.settings.resolve(strict=True);messages=a.messages.resolve(strict=True)
    overlay=a.overlay.resolve(strict=True)
    if not overlay.is_dir(): p.error('--overlay must be a directory containing both paks')
    overlay_pak=overlay/'RMUA-LinuxNoEditor_0_P.pak'
    binary=sim/'Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping'
    if not binary.is_file(): p.error('simulator binary missing')
    output=a.output.resolve();output.mkdir(parents=True,exist_ok=False)
    base=Path(__file__).resolve().parent.parent
    image=docker('image','inspect',a.image,'--format','{{.Id}}',capture_output=True,check=True).stdout.strip()
    manifest={'binary_sha256':sha(binary),'settings_sha256':sha(settings),'image_id':image,
              'gdb_script_sha256':sha(base/'scripts/runtime_ol2_redirect.gdb'), 'runner_sha256':sha(Path(__file__)), 'readiness_timeout_s':a.readiness_timeout,
              'overlay_sha256':sha(overlay_pak), 'base_pak_sha256':sha(overlay/'RMUA-LinuxNoEditor.pak'), 'mode':'nullrhi','requested_map':a.map_url,'seeds':seeds,'repeats':a.repeats,
              'stage_coverage':'direct OL2; GDB skips early frequency bookkeeping and redirects OL1 travel URL to OL2; no flight','runs':[]}
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
                 '-e','RMUA_PORT='+str(port),'-e','RMUA_SEED='+str(seed),'-e','RMUA_MAP='+a.map_url,
                 '-v',str(sim)+':/sim:ro','-v',str(settings)+':/configs/settings.json:ro',
                 '-v',str(messages)+':/workspace/rmua_ws:ro',
                 '-v',str(overlay)+':/sim/Build/LinuxNoEditor/RMUA/Content/Paks:ro',
                 '-v',str(base)+':/analysis:ro','-v',str(run)+':/out','-v','/tmp/.X11-unix:/tmp/.X11-unix',
                 '--entrypoint','bash',a.image,'-c',LAUNCH]
            x=docker(*cmd,capture_output=True,check=True);entry['id']=x.stdout.strip()
            deadline=time.monotonic()+a.readiness_timeout
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
            sensor=inside(name,'python3 /analysis/scripts/sensor_clock_probe.py /out/sensor_clock.jsonl 5',12)
            (run/'sensor_clock.stderr').write_text(sensor.stderr)
            entry['sensor_clock_exit_code']=sensor.returncode
            sensor_path=run/'sensor_clock.jsonl'
            rows=[json.loads(line) for line in sensor_path.read_text().splitlines()] if sensor_path.exists() else []
            entry['sensor_counts']={name:sum(row.get('topic')==name for row in rows) for name in ('imu','lidar','camera_left','camera_right')}
            entry['lidar_nonempty_count']=sum(row.get('topic')=='lidar' and row.get('data_bytes',0)>0 for row in rows)
            topics=inside(name,'timeout 4 rostopic list',8)
            (run/'topics_after_sample.txt').write_text(topics.stdout+topics.stderr)
            entry['status']='observed' if (not a.reset or entry.get('reset_response_success')) and sensor.returncode==0 and entry['sensor_counts']['imu']>0 and entry['lidar_nonempty_count']>0 else 'incomplete'

        except (OSError,RuntimeError,subprocess.SubprocessError) as exc:
            entry['status']='failed';entry['error']=str(exc)
        finally:
            with (run/'gdb.log').open('w') as f: docker('logs',name,stdout=f,stderr=subprocess.STDOUT)
            logs=(run/'gdb.log').read_text(errors='replace')
            loaded=re.findall(r'LogLoad: LoadMap: ([^\s]+)',logs)
            entry['observed_maps']=loaded
            entry['redirect_observed']='RMUA_MAP_REDIRECT old=OL1 new=OL2' in logs
            entry['early_frequency_skip_observed']='RMUA_FREQ_GATE skipped=' in logs
            entry['node_ready_observed']='RMUA_FREQ_GATE node_ready=' in logs
            if entry['status']=='observed' and not all(entry[k] for k in ('redirect_observed','early_frequency_skip_observed','node_ready_observed')): entry['status']='incomplete'
            if entry['status']=='observed' and (not loaded or loaded[-1].split('?',1)[0]!=a.map_url):
                entry['status']='requested_map_not_observed'
            docker('rm','-f',name,stdout=subprocess.DEVNULL,stderr=subprocess.DEVNULL)
            entry['end_wall']=time.time();save()
        print(json.dumps(entry),flush=True)
    print('manifest:',manifest_path,flush=True)

if __name__=='__main__':
    try:main()
    except (ValueError,OSError,subprocess.SubprocessError) as e: print(str(e),file=sys.stderr);sys.exit(1)
