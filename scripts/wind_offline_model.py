#!/usr/bin/env python3
"""Offline single-precision model of 0x2fb23b0, using this host's libc rand()."""
import argparse,ctypes,hashlib,json,re,struct,platform
from pathlib import Path
import numpy as np
f=np.float32
lib=ctypes.CDLL(None);lib.rand.restype=ctypes.c_int;lib.srand.argtypes=[ctypes.c_uint]
def randunit():return f(f(lib.rand() & 0xffffff)/f(16777215.0))
def generate(seed,horizon,target,maximum,minimum):
 if seed is not None: lib.srand(seed)
 remaining=f(horizon);duration=[]
 while remaining>=f(11):
  u=randunit(); d=f(f(u+u)+f(8));duration.append(d);remaining=f(remaining-f(d+f(2)))
 if len(duration)<2:return []
 n=len(duration);k=min(int(f(f(n)*randunit())),n-1)
 base=f(f(f(target)-f(duration[k]*f(maximum)))/f(f(f(horizon)-duration[k])-f(f(n-1)+f(n-1))))
 speeds=[];dirs=[]
 for i in range(n):
  if i==k:speed=f(maximum)
  else:
   speed=f(f(f(randunit()*f(2.4))+f(-1.2))+base)
   speed=f(min(speed,f(f(maximum)+f(-.01))));speed=f(max(speed,f(minimum)))
  speeds.append(speed);dirs.append(f(randunit()*f(360)))
 items=[];t=f(0)
 for i in range(n):
  items.append((t,speeds[i],dirs[i],0));t=f(t+duration[i])
  if i<n-1:items.append((t,speeds[i],dirs[i],1));t=f(t+f(2))
 return [struct.pack('<fffI',float(t),float(s),float(d),flag) for t,s,d,flag in items]
def main():
 p=argparse.ArgumentParser(description=__doc__);p.add_argument('--output',type=Path,required=True);p.add_argument('--experiments',type=Path,default=Path('experiments'));a=p.parse_args();a.output.mkdir(parents=True,exist_ok=False)
 params={4:(200,350,2,1.5),5:(220,660,4,2),6:(200,1800,15,4)};report={'libc':'host libc via ctypes; sequence is platform-dependent','numpy':np.__version__,'libc_version':platform.libc_ver(),'model_sha256':hashlib.sha256(Path(__file__).read_bytes()).hexdigest(),'scope':'fresh stage initialization after srand(seed), no preceding libc rand consumers','results':{}}
 for stage,args in params.items():
  for seed in (123,456,2026):
   model=generate(seed,*args);raw=b''.join(model);run={ 'parameters':args,'items':[x.hex() for x in model],'sha256':hashlib.sha256(raw).hexdigest(),'comparisons':{}}
   for rep in (1,2):
    log=a.experiments/f'stage_inject_{stage}_20261104'/f'seed{seed}_repeat{rep}'/'gdb.log'
    matches=re.findall(r'WIND_ITEM\s+\d+ hex=([0-9a-f]{32})',log.read_text(errors='replace'));reference=[bytes.fromhex(x) for x in matches]
    run['comparisons'][str(rep)]={'observed_items':len(reference),'exact_bytes':model==reference,'matching_items':sum(x==y for x,y in zip(model,reference))}
   report['results'][f'stage{stage}_seed{seed}']=run
 (a.output/'model_results.json').write_text(json.dumps(report,indent=2)+'\n');print(json.dumps({k:v['comparisons'] for k,v in report['results'].items()},indent=2))
if __name__=='__main__':main()
