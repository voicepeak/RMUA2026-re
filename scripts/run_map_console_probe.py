#!/usr/bin/env python3
"""Try the engine's interactive console for map selection; retain proof of actual LoadMap."""
import ctypes,ctypes.util,json,os,re,secrets,subprocess,time
from pathlib import Path
from run_readonly_trace import LAUNCH,docker,sha
base=Path(__file__).resolve().parent.parent
out=base/'experiments/map_console_20261104';out.mkdir(exist_ok=False)
sim=Path('/home/tianbot/RUMA-by-helinjun/main/simulator/simulator_12.0.0.5'); settings=Path('/home/tianbot/RUMA-by-helinjun/main/configs/simulator_settings.json'); name='rmua-console-'+secrets.token_hex(6);port=11731
meta={'requested_map':'/Game/Maps/RMUA2024OL/RMUA2024OL2','seed':123,'start_wall_unix_s':time.time(),'status':'starting','binary_sha256':sha(sim/'Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping')}
def save():(out/'manifest.json').write_text(json.dumps(meta,indent=2)+'\n')
launch=LAUNCH.replace('-nullrhi','-windowed -ResX=800 -ResY=600')
try:
 args=['run','-d','--init','--name',name,'--network','host','--gpus','all','--cap-add','SYS_PTRACE','--security-opt','seccomp=unconfined','--user',f'{os.getuid()}:{os.getgid()}','-e','HOME=/tmp','-e','LD_LIBRARY_PATH=','-e','NVIDIA_DRIVER_CAPABILITIES=all','-e','DISPLAY=:1','-e','ROS_HOSTNAME=localhost','-e',f'ROS_MASTER_URI=http://127.0.0.1:{port}','-e',f'RMUA_PORT={port}','-e','RMUA_SEED=123','-v',str(sim)+':/sim:ro','-v',str(settings)+':/configs/settings.json:ro','-v',str(base)+':/analysis:ro','-v',str(out)+':/out','-v','/tmp/.X11-unix:/tmp/.X11-unix','--entrypoint','bash','rmua/re-probe:20261104','-c',launch]
 meta['container_id']=docker(*args,capture_output=True,check=True).stdout.strip();save()
 deadline=time.monotonic()+45;window=None
 while time.monotonic()<deadline:
  state=docker('inspect',name,'--format','{{.State.Running}}',capture_output=True)
  if state.stdout.strip()!='true':raise RuntimeError('simulator exited')
  w=subprocess.run(['xwininfo','-display',':1','-root','-tree'],capture_output=True,text=True).stdout
  (out/'window_tree.txt').write_text(w)
  m=re.search(r'^\s*(0x[0-9a-f]+)\s+"([^"]*(?:RMUA|Unreal)[^"]*)"',w,re.I|re.M)
  log=docker('logs',name,capture_output=True).stdout
  if m and 'LogLoad: LoadMap: /Game/Maps/RMUA2024OL/RMUA2024OL1' in log:window=int(m.group(1),16);meta['window_title']=m.group(2);break
  time.sleep(.5)
 if window is None:raise RuntimeError('simulator window or initial map not observed')
 x=ctypes.CDLL(ctypes.util.find_library('X11'));xt=ctypes.CDLL(ctypes.util.find_library('Xtst'))
 x.XOpenDisplay.argtypes=[ctypes.c_char_p];x.XOpenDisplay.restype=ctypes.c_void_p
 x.XSetInputFocus.argtypes=[ctypes.c_void_p,ctypes.c_ulong,ctypes.c_int,ctypes.c_ulong];x.XKeysymToKeycode.argtypes=[ctypes.c_void_p,ctypes.c_ulong];x.XKeysymToKeycode.restype=ctypes.c_ubyte;x.XFlush.argtypes=[ctypes.c_void_p]
 xt.XTestFakeKeyEvent.argtypes=[ctypes.c_void_p,ctypes.c_uint,ctypes.c_int,ctypes.c_ulong]
 d=x.XOpenDisplay(b':1')
 if not d:raise RuntimeError('XOpenDisplay failed')
 x.XSetInputFocus(d,window,1,0);x.XFlush(d);time.sleep(.5)
 def key(sym):
  code=x.XKeysymToKeycode(d,sym)
  if not code:raise RuntimeError('missing keycode '+str(sym))
  xt.XTestFakeKeyEvent(d,code,1,0);xt.XTestFakeKeyEvent(d,code,0,0);x.XFlush(d);time.sleep(.025)
 key(0x60);time.sleep(.5)
 # "open /Game/Maps/RMUA2024OL/RMUA2024OL2" uses explicit Shift for uppercase.
 for char in 'open /Game/Maps/RMUA2024OL/RMUA2024OL2':
  if char.isupper():
   shift=x.XKeysymToKeycode(d,0xffe1);xt.XTestFakeKeyEvent(d,shift,1,0);key(ord(char.lower()));xt.XTestFakeKeyEvent(d,shift,0,0);x.XFlush(d)
  else:key(ord(char))
 key(0xff0d);meta['console_command_wall_unix_s']=time.time();time.sleep(8)
 log=docker('logs',name,capture_output=True).stdout
 maps=re.findall(r'LogLoad: LoadMap: ([^\s]+)',log);meta['observed_maps']=maps
 meta['status']='target_map_observed' if meta['requested_map'] in maps else 'target_map_not_observed'
except Exception as exc:meta.update(status='failed',error=repr(exc))
finally:
 x=docker('logs',name,capture_output=True);(out/'gdb.log').write_text(x.stdout+x.stderr)
 x=docker('inspect',name,'--format','{{.Id}}',capture_output=True)
 if x.returncode==0 and x.stdout.strip()==meta.get('container_id'):docker('rm','-f',name,capture_output=True)
 meta['end_wall_unix_s']=time.time();save()
print(json.dumps(meta,indent=2))
