#!/usr/bin/env python3
"""Bounded diagnostic GPS position + IMU orientation bridge; no truth/control input.

Publishes only /rmua/diagnostic/pose; downstream control is not started here.
Reject stale or temporally separated samples. Logs all decisions and source clocks.
"""
import json,math,sys,threading,time
import rospy
from geometry_msgs.msg import PoseStamped
from sensor_msgs.msg import Imu
rospy.init_node('rmua_gps_imu_pose_bridge',anonymous=True,disable_signals=True)
out=open(sys.argv[1],'w',buffering=1);seconds=float(sys.argv[2]);lock=threading.Lock();latest={'imu':None,'imu_mono':None,'last_gps_stamp':None,'departed':False}
pub=rospy.Publisher('/rmua/diagnostic/pose',PoseStamped,queue_size=1)
def imu_cb(msg):
 with lock:latest.update(imu=msg,imu_mono=time.monotonic())
def gps_cb(msg):
 now=time.monotonic();wall=time.time()
 with lock:imu=latest['imu'];imu_mono=latest['imu_mono'];last_gps_stamp=latest['last_gps_stamp'];departed=latest['departed']
 gps=msg.header.stamp.to_sec();row={'arrival_monotonic_s':now,'arrival_wall_unix_s':wall,'gps_stamp_ros_s':gps,'accepted':False}
 if imu is None:row['reason']='no_imu'
 else:
  stamp=imu.header.stamp.to_sec();q=imu.orientation;p=msg.pose.position;norm=q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w
  row.update(imu_stamp_ros_s=stamp,imu_age_monotonic_s=now-imu_mono,header_skew_s=gps-stamp,quaternion_norm2=norm,position=[p.x,p.y,p.z],orientation_xyzw=[q.x,q.y,q.z,q.w])
  if not all(math.isfinite(x) for x in (gps,stamp,p.x,p.y,p.z,q.x,q.y,q.z,q.w,norm)):row['reason']='nonfinite'
  elif gps<=0 or stamp<=0:row['reason']='invalid_stamp'
  elif last_gps_stamp is not None and gps<=last_gps_stamp:row['reason']='gps_stamp_not_increasing'
  elif departed and p.x==0. and p.y==0. and p.z==0.:row['reason']='gps_zero_after_departure'
  elif now-imu_mono>.3:row['reason']='stale_imu'
  elif abs(gps-stamp)>.2:row['reason']='header_skew'
  elif abs(norm-1.)>.01:row['reason']='invalid_quaternion'
  else:
   pose=PoseStamped();pose.header=msg.header;pose.pose.position=msg.pose.position;pose.pose.orientation=imu.orientation;pub.publish(pose);row.update(accepted=True,reason='accepted')
   with lock:latest.update(last_gps_stamp=gps,departed=departed or math.hypot(p.x,p.y)>2.)
 with lock:out.write(json.dumps(row,separators=(',',':'))+'\n')
subs=[rospy.Subscriber('/airsim_node/drone_1/imu/imu',Imu,imu_cb,queue_size=20),rospy.Subscriber('/airsim_node/drone_1/gps',PoseStamped,gps_cb,queue_size=5)]
end=time.monotonic()+seconds
while time.monotonic()<end and not rospy.is_shutdown():time.sleep(.05)
for s in subs:s.unregister()
with lock:out.close()
