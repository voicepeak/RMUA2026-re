#!/usr/bin/env python3
"""Exercise LD_PRELOAD forwarding and observed 12-byte Header layout with a toy DSO."""
import json
import os
from pathlib import Path
import subprocess
import tempfile

root=Path(__file__).resolve().parent
sym_imu='_ZNK3ros9Publisher7publishIN11sensor_msgs4Imu_ISaIvEEEEEvRKT_'
sym_lidar='_ZNK3ros9Publisher7publishIN11sensor_msgs12PointCloud2_ISaIvEEEEEvRKT_'
sym_pwm='_ZNK3ros9Publisher7publishIN10airsim_ros9RotorPWM_ISaIvEEEEEvRKT_'
sym_cb='_ZN10airsim_ros13commands_impl18ROTOR_PWM_CallbackERKN5boost10shared_ptrIKNS_9RotorPWM_ISaIvEEEEE'
with tempfile.TemporaryDirectory(prefix='rmua-publish-test-') as temp:
    p=Path(temp)
    fake='int forwarded=0;\n'
    for name,sym in [('imu',sym_imu),('lidar',sym_lidar),('pwm',sym_pwm)]:
        fake+=f'void {name}(const void*,const void*) __asm__("{sym}");\nvoid {name}(const void *a,const void *b) {{if(a && b) forwarded++;}}\n'
    fake+=f'void cb(const void*) __asm__("{sym_cb}");\nvoid cb(const void *a) {{if(a) forwarded++;}}\n'
    main='#include <stdint.h>\nextern int forwarded;\n'
    for name,sym in [('imu',sym_imu),('lidar',sym_lidar),('pwm',sym_pwm)]:
        main+=f'void {name}(const void*,const void*) __asm__("{sym}");\n'
    main+=f'void cb(const void*) __asm__("{sym_cb}");\n'
    main+='int main(void){uint32_t h[3]={7,1234,987654321};void *shared=h;imu(h,h);lidar(h,h);pwm(h,h);cb(&shared);return forwarded==4?0:3;}\n'
    (p/'fake.c').write_text(fake);(p/'main.c').write_text(main)
    def run(*cmd,**kw):return subprocess.run(cmd,check=True,**kw)
    run('gcc','-shared','-fPIC',str(p/'fake.c'),'-o',str(p/'libfake.so'))
    run('gcc',str(p/'main.c'),'-L'+str(p),'-Wl,-rpath,'+str(p),'-lfake','-o',str(p/'fixture'))
    run('gcc','-shared','-fPIC','-O2','-Wall','-Wextra','-Werror',str(root/'ros_publish_interpose.c'),'-ldl','-pthread','-o',str(p/'probe.so'))
    env=dict(os.environ,LD_PRELOAD=str(p/'probe.so'),RMUA_PUBLISH_TRACE=str(p/'events.jsonl'))
    run(str(p/'fixture'),env=env)
    rows=[json.loads(x) for x in (p/'events.jsonl').read_text().splitlines()]
    assert [x['type'] for x in rows]==['imu','lidar','rotor_pwm','rotor_pwm_command_callback']
    assert all((x['seq'],x['sec'],x['nsec'])==(7,1234,987654321) for x in rows)
    assert all(x['event']=='hook_entry' and x['mono_ns']>0 and x['wall_ns']>0 for x in rows)
    print('four hooks intercepted and forwarded; toy header fields and clocks verified')
