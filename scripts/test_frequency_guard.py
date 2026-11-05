#!/usr/bin/env python3
"""Verify interposer forwarding, serialization, return values and exception unlock."""
import json,os,subprocess,tempfile
from pathlib import Path
root=Path(__file__).resolve().parent
record='_ZN10airsim_ros21record_frequency_dataERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE'
getter='_ZN10airsim_ros19get_topic_frequencyERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE'
one='_ZN10airsim_ros27get_topic_frequency_one_secERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE'
with tempfile.TemporaryDirectory(prefix='rmua-freq-test-') as temp:
 p=Path(temp)
 fake='''#include <atomic>\n#include <stdexcept>\n#include <unistd.h>\nstd::atomic<int> active{0},overlap{0},count{0};\nextern "C" int get_overlap(){return overlap.load();}\nextern "C" int get_count(){return count.load();}\n'''
 fake+=f'''extern "C" void rec(const void*) __asm__("{record}");\nextern "C" void rec(const void *p) {{if(!p) throw std::runtime_error("synthetic"); if(active.fetch_add(1)>0)overlap++;usleep(300);count++;active--;}}\n'''
 fake+=f'''extern "C" float freq(const void*) __asm__("{getter}");\nextern "C" float freq(const void*) {{return 4.25f;}}\n'''
 fake+=f'''extern "C" float freqone(const void*) __asm__("{one}");\nextern "C" float freqone(const void*) {{return 9.5f;}}\n'''
 main='''#include <thread>\n#include <vector>\n#include <stdexcept>\n#include <cstdio>\nextern "C" int get_overlap();\nextern "C" int get_count();\n'''
 main+=f'''extern "C" void rec(const void*) __asm__("{record}");\nextern "C" float freq(const void*) __asm__("{getter}");\nextern "C" float freqone(const void*) __asm__("{one}");\n'''
 main+='''int main(){try {rec(nullptr);return 8;}catch(const std::runtime_error&){} int key=1;if(freq(&key)!=4.25f || freqone(&key)!=9.5f)return 9;std::vector<std::thread> threads;for(int i=0;i<4;i++)threads.emplace_back([&](){for(int j=0;j<30;j++)rec(&key);});for(auto &t:threads)t.join();std::printf("%d %d\\n",get_count(),get_overlap());return 0;}\n'''
 (p/'fake.cpp').write_text(fake);(p/'main.cpp').write_text(main)
 def run(*cmd,**kw):return subprocess.run(cmd,check=True,**kw)
 run('g++','-shared','-fPIC','-pthread',str(p/'fake.cpp'),'-o',str(p/'libfake.so'))
 run('g++','-pthread',str(p/'main.cpp'),'-L'+str(p),'-Wl,-rpath,'+str(p),'-lfake','-o',str(p/'fixture'))
 run('g++','-shared','-fPIC','-O2','-Wall','-Wextra','-Werror',str(root/'frequency_guard.cpp'),'-ldl','-pthread','-o',str(p/'guard.so'))
 control=run(str(p/'fixture'),text=True,capture_output=True).stdout.strip().split();assert int(control[0])==120 and int(control[1])>0
 env=dict(os.environ,LD_PRELOAD=str(p/'guard.so'),RMUA_FREQ_GUARD_LOG=str(p/'guard.jsonl'))
 guarded=run(str(p/'fixture'),text=True,capture_output=True,env=env).stdout.strip().split();assert list(map(int,guarded))==[120,0]
 rows=[json.loads(x) for x in (p/'guard.jsonl').read_text().splitlines()];assert rows and rows[0]['calls']==1
 print('forwarding, float return values, exception unlock and serialized multithread fixture: pass')
