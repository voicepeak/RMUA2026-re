#!/usr/bin/env python3
"""Summarize explicit pool PWM repeats and the observed sensor/publisher overlap."""
import json,sys
from pathlib import Path
out=Path(sys.argv[1]);paths=[Path(x) for x in sys.argv[2:]]
if not paths:raise SystemExit('usage: summarize_pool_pwm_repeats.py OUTPUT RUN...')
result={'runs':{},'limitations':['published PWM commands are not proof of actuator application',
 'fixed open-loop PWM and sensor subscriptions are a synthetic load, not a full training episode',
 'finite repeat observations do not establish a universal stable concurrency limit',
 'source-header/wall ratio is not independently calibrated physical simulation speed']}
for p in paths:
 w=json.loads((p/'workload_analysis.json').read_text());s=json.loads((p/'stream/independent_analysis.json').read_text());m=json.loads((p/'pool/pool.json').read_text());st=json.loads((p/'workload_manifest.json').read_text())
 runs=[]
 for name,a in w['instances'].items():
  q=s['instances'][name];runs.append({'seed':a['seed'],'command_count':a['command_count'],'command_values':a['command_values'],
    'imu_overlap_s':a['topics']['imu'].get('overlap_with_publisher_s'),'lidar_overlap_s':a['topics']['lidar'].get('overlap_with_publisher_s'),
    'imu_hz':a['topics']['imu'].get('arrival_hz'),'lidar_hz':a['topics']['lidar'].get('arrival_hz'),
    'imu_zero_stamps':a['topics']['imu'].get('zero_stamps'),'imu_nonzero_regressions':a['topics']['imu'].get('nonzero_stamp_regressions'),
    'imu_source_seconds_per_wall_second':q['topics']['imu'].get('source_seconds_per_wall_second'),
    'cpu_percent_one_core':q['simulator_cpu_percent_one_core'],'rss_peak_mib':q['simulator_rss_peak_mib']})
 result['runs'][str(p)]={'status':st['status'],'cleanup_exit_code':st.get('cleanup_exit_code'),'requested_workload_s':st['requested_workload_s'],
   'binary_sha256':m['binary_sha256'],'settings_sha256':m['settings_sha256'],'image_id':m['image_id'],
   'all_observers_finished':s['all_observers_finished'],'all_processes_observed':s['all_processes_observed'],'instances':runs}
out.mkdir(parents=True,exist_ok=False);(out/'summary.json').write_text(json.dumps(result,indent=2)+'\n')
for k,v in result['runs'].items():print(k,v['status'],'instances',len(v['instances']),'cleanup',v['cleanup_exit_code'])
