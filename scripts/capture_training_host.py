#!/usr/bin/env python3
"""Capture reproducibility-relevant host and image facts without credentials or hostnames."""
import argparse,datetime,hashlib,json,os,platform,subprocess
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--simulator',type=Path,required=True);p.add_argument('--settings',type=Path,required=True);p.add_argument('--output',type=Path,required=True);p.add_argument('--image',default='rmua/re-probe:20261104');a=p.parse_args()
def sha(p):
 h=hashlib.sha256()
 with p.open('rb') as f:
  for b in iter(lambda:f.read(1048576),b''):h.update(b)
 return h.hexdigest()
def command(args):
 try:
  r=subprocess.run(args,text=True,capture_output=True,timeout=20,env={**os.environ,'LC_ALL':'C'});return {'exit_code':r.returncode,'stdout':r.stdout.strip(),'stderr':r.stderr.strip()}
 except (OSError,subprocess.SubprocessError) as e:return {'error':str(e)}
binary=a.simulator/'Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping'
mem={}
for line in Path('/proc/meminfo').read_text().splitlines():
 if line.startswith(('MemTotal:','SwapTotal:')):
  k,v=line.split(':',1);mem[k]=v.strip()
data={'captured_utc':datetime.datetime.now(datetime.timezone.utc).isoformat(),'os_release':Path('/etc/os-release').read_text(),'kernel_release':platform.release(),'machine':platform.machine(),'cpu_logical_count':os.cpu_count(),'memory':mem,'cpu':command(['lscpu','-J']),'gpu':command(['nvidia-smi','--query-gpu=name,driver_version,memory.total','--format=csv,noheader']),'docker_version':command(['docker','version','--format','{{.Server.Version}}']),'image':a.image,'image_id':command(['docker','image','inspect',a.image,'--format','{{.Id}}']),'binary_sha256':sha(binary),'settings_sha256':sha(a.settings),'python':platform.python_version(),'libc':platform.libc_ver(),'limitations':['Host snapshot, not a lockstep simulator specification','GPU memory is shared with other host processes']}
a.output.parent.mkdir(parents=True,exist_ok=True);a.output.write_text(json.dumps(data,indent=2)+'\n');print(a.output)
