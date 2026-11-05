#!/usr/bin/env python3
"""Exercise exact-object tracking, forwarding, and different-object controls."""
import json,os,subprocess,tempfile
from pathlib import Path
root=Path(__file__).resolve().parent
r='_ZN10airsim_ros21record_frequency_dataERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE'
g='_ZN10airsim_ros19get_topic_frequencyERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE'
o='_ZN10airsim_ros27get_topic_frequency_one_secERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE'
push='_ZNSt5dequeImSaImEE9push_backERKm'
pop='_ZNSt5dequeImSaImEE9pop_frontEv'
size='_ZNKSt5dequeImSaImEE4sizeEv'
with tempfile.TemporaryDirectory(prefix='rmua-deque-') as t:
 d=Path(t)
 fake=f'''#include <string>\n#include <cstdint>\n#include <unistd.h>\nstatic int a,b;\nextern "C" void push(void*,const uint64_t&) __asm__("{push}");\nextern "C" void push(void*,const uint64_t&){{usleep(15000);}}\nextern "C" void pop(void*) __asm__("{pop}");\nextern "C" void pop(void*){{usleep(15000);}}\nextern "C" size_t size(const void*) __asm__("{size}");\nextern "C" size_t size(const void*){{usleep(15000);return 37;}}\nextern "C" void rec(const std::string&) __asm__("{r}");\nextern "C" void rec(const std::string &k){{uint64_t x=5;push(k=="other"?&b:&a,x);}}\nextern "C" float get(const std::string&) __asm__("{g}");\nextern "C" float get(const std::string &k){{return size(k=="other"?&b:&a)==37?4.25f:0.0f;}}\nextern "C" float one(const std::string&) __asm__("{o}");\nextern "C" float one(const std::string &k){{pop(k=="other"?&b:&a);return 9.5f;}}\n'''
 main=f'''#include <atomic>\n#include <string>\n#include <thread>\nextern "C" void rec(const std::string&) __asm__("{r}");\nextern "C" float get(const std::string&) __asm__("{g}");\nextern "C" float one(const std::string&) __asm__("{o}");\nint main(int argc,char**){{std::atomic<int> ready{{0}};std::string a="same",b=argc>1?"other":"same";float result=0;std::thread t1([&]{{ready++;while(ready.load()<2){{}}rec(a);}}),t2([&]{{ready++;while(ready.load()<2){{}}result=get(b);}});t1.join();t2.join();return result==4.25f && one(a)==9.5f?0:1;}}\n'''
 (d/'fake.cpp').write_text(fake);(d/'main.cpp').write_text(main)
 def run(*cmd,**kw):return subprocess.run(cmd,check=True,**kw)
 run('g++','-shared','-fPIC',str(d/'fake.cpp'),'-o',str(d/'libfake.so'))
 run('g++','-pthread',str(d/'main.cpp'),'-L'+str(d),'-Wl,-rpath,'+str(d),'-lfake','-o',str(d/'fixture'))
 run('g++','-shared','-fPIC','-O2','-Wall','-Wextra','-Werror',str(root/'frequency_deque_overlap.cpp'),'-ldl','-pthread','-o',str(d/'probe.so'))
 for same in (True,False):
  log=d/('same.jsonl' if same else 'different.jsonl')
  run(str(d/'fixture'),*(() if same else ('different',)),env=dict(os.environ,LD_PRELOAD=str(d/'probe.so'),RMUA_FREQ_GUARD_LOG=str(log)))
  rows=[json.loads(x) for x in log.read_text().splitlines()]
  assert bool([x for x in rows if x['overlap']])==same, rows
  assert all(x['untracked']==0 for x in rows)
 print('same-object read/write, distinct-object control, forwarding and float/size returns: pass')
