#!/usr/bin/env python3
"""Check real ABI forwarding and same-key classification with independent fixtures."""
import json
import os
import subprocess
import tempfile
from pathlib import Path
root = Path(__file__).resolve().parent
record = '_ZN10airsim_ros21record_frequency_dataERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE'
getter = '_ZN10airsim_ros19get_topic_frequencyERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE'
one = '_ZN10airsim_ros27get_topic_frequency_one_secERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE'
with tempfile.TemporaryDirectory(prefix='rmua-overlap-') as temp:
 p=Path(temp)
 fake=f'''#include <string>\n#include <unistd.h>\nextern "C" void rec(const std::string&) __asm__("{record}");\nextern "C" void rec(const std::string&) {{usleep(10000);}}\nextern "C" float get(const std::string&) __asm__("{getter}");\nextern "C" float get(const std::string&) {{usleep(10000);return 4.25f;}}\nextern "C" float one(const std::string&) __asm__("{one}");\nextern "C" float one(const std::string&) {{return 9.5f;}}\n'''
 main=f'''#include <string>\n#include <thread>\n#include <atomic>\nextern "C" void rec(const std::string&) __asm__("{record}");\nextern "C" float get(const std::string&) __asm__("{getter}");\nextern "C" float one(const std::string&) __asm__("{one}");\nint main(int argc,char**) {{std::atomic<int> ready{{0}};std::string a="frame", b=argc==1?"frame":"imu"; float r=0;std::thread t1([&]{{ready++;while(ready.load()<2){{}} rec(a);}});std::thread t2([&]{{ready++;while(ready.load()<2){{}} r=get(b);}});t1.join();t2.join();return r==4.25f && one(a)==9.5f?0:1;}}\n'''
 (p/'fake.cpp').write_text(fake);(p/'main.cpp').write_text(main)
 def run(*cmd,**kw):return subprocess.run(cmd,check=True,**kw)
 run('g++','-shared','-fPIC',str(p/'fake.cpp'),'-o',str(p/'libfake.so'))
 run('g++','-pthread',str(p/'main.cpp'),'-L'+str(p),'-Wl,-rpath,'+str(p),'-lfake','-o',str(p/'fixture'))
 run('g++','-shared','-fPIC','-O2','-Wall','-Wextra','-Werror',str(root/'frequency_overlap_interposer.cpp'),'-ldl','-pthread','-o',str(p/'probe.so'))
 for same in (True,False):
  log=p/('same.jsonl' if same else 'different.jsonl')
  env=dict(os.environ,LD_PRELOAD=str(p/'probe.so'),RMUA_FREQ_GUARD_LOG=str(log))
  run(str(p/'fixture'),*(() if same else ('different',)),env=env)
  rows=[json.loads(x) for x in log.read_text().splitlines()]
  assert any(row['overlap'] for row in rows)==same,rows
  assert all(row['untracked']==0 for row in rows)
 print('same-key overlap, distinct-key control, forwarding and float returns: pass')
