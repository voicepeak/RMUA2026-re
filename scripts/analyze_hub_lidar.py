#!/usr/bin/env python3
"""Offline approximate corridor returns from recorded body-frame LiDAR snapshots."""
import argparse,glob,json,math,statistics
from pathlib import Path
import numpy as np
p=argparse.ArgumentParser(description=__doc__);p.add_argument('folder',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();results=[]
for path in sorted(a.folder.glob('scan_*.npz')):
 with np.load(path,allow_pickle=False) as scan:
  v=scan['xyz'].astype(float);m=json.loads(str(scan['metadata']))
 x,y,z,w=m['orientation_xyzw'];R=np.array([[1-2*(y*y+z*z),2*(x*y-z*w),2*(x*z+y*w)],[2*(x*y+z*w),1-2*(x*x+z*z),2*(y*z-x*w)],[2*(x*z-y*w),2*(y*z+x*w),1-2*(x*x+y*y)]])
 rel=(v+np.array([0.,0.,-.05]))@R.T;target=np.array([706.3,1.]);direction=target-np.array(m['position'][:2]);direction/=np.linalg.norm(direction);forward=rel[:,:2]@direction;side=-rel[:,0]*direction[1]+rel[:,1]*direction[0];bands=[]
 for lo in range(-8,8,2):
  mask=(forward>.3)&(forward<8)&(abs(side)<2)&(rel[:,2]>=lo)&(rel[:,2]<lo+2)
  bands.append({'relative_ned_z_m':[lo,lo+2],'points':int(mask.sum()),'closest_forward_m':float(forward[mask].min()) if mask.any() else None})
 results.append({'file':path.name,'metadata':m,'cloud_minus_gps_stamp_s':m['cloud_stamp_ros']-m['gps_stamp_ros'],'cloud_minus_imu_stamp_s':m['cloud_stamp_ros']-m['imu_stamp_ros'],'bands':bands})
telemetry=[]
for line in (a.folder/'telemetry.jsonl').open():
 row=json.loads(line)
 if row.get('topic')=='controller_telemetry':
  x=json.loads(row['data'])
  if 685<=x.get('s',0)<=695:telemetry.append(x)
from collections import Counter
summary={'method':'approximate raw lidar transformed by latest GPS/IMU, -0.05m sensor offset, cylinder toward static spline 3 endpoint; no pose interpolation','limits':['GPS/IMU not synchronized exactly to scan; stamp offsets recorded','zero points do not prove free space','point return is not collision mesh or cause proof','no debug truth used'],'scans':results,'near_hub_telemetry_samples':len(telemetry),'near_hub_reasons':dict(Counter(x.get('speed_limits',{}).get('reason') for x in telemetry)),'near_hub_clearance_reasons':dict(Counter(x.get('clearance',{}).get('command_reason') for x in telemetry))}
a.output.write_text(json.dumps(summary,indent=2)+'\n');print('scans',len(results),'near-hub telemetry',len(telemetry),'reasons',summary['near_hub_reasons']);print('last_scan',json.dumps(results[-1] if results else None))
