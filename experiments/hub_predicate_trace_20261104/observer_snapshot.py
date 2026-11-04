#!/usr/bin/env python3
"""Passive, bounded hub LiDAR snapshots and controller telemetry; no truth/control."""
import json,os,sys,threading,time
import numpy as np
import rospy
from geometry_msgs.msg import PoseStamped
from sensor_msgs.msg import Imu,PointCloud2
from std_msgs.msg import String
out=sys.argv[1];os.makedirs(out,exist_ok=True);rospy.init_node('rmua_hub_lidar_probe',anonymous=True,disable_signals=True)
lock=threading.Lock();state={'pose':None,'imu':None,'last':0.,'count':0};stream=open(os.path.join(out,'telemetry.jsonl'),'w',buffering=1)
def gps(m):
 with lock:state['pose']=m
def imu(m):
 with lock:state['imu']=m
def telemetry(m,topic):
 row={'topic':topic,'wall_unix':time.time(),'monotonic':time.monotonic(),'ros_now':rospy.Time.now().to_sec(),'data':m.data}
 with lock:stream.write(json.dumps(row,separators=(',',':'))+'\n')
def lidar(m):
 now=time.monotonic()
 with lock:
  pose=state['pose'];orientation=state['imu']
  if pose is None or orientation is None or now-state['last']<3 or state['count']>=24:return
  p=pose.pose.position
  if p.x<560 or p.x>750:return
  state['last']=now;index=state['count'];state['count']+=1
 try:
  fields={f.name:f for f in m.fields}
  if any(k not in fields or fields[k].datatype!=7 for k in ('x','y','z')):raise ValueError('unsupported point fields')
  dtype=np.dtype({'names':['x','y','z'],'formats':[('>f4' if m.is_bigendian else '<f4')]*3,'offsets':[fields[k].offset for k in ('x','y','z')],'itemsize':m.point_step})
  if m.row_step!=m.width*m.point_step:raise ValueError('padded point cloud rows unsupported')
  a=np.frombuffer(m.data,dtype=dtype,count=m.width*m.height);xyz=np.column_stack([a[k] for k in ('x','y','z')]);valid=np.isfinite(xyz).all(axis=1)&(np.linalg.norm(xyz,axis=1)>.05);xyz=xyz[valid]
  q=orientation.orientation
  meta={'cloud_stamp_ros':m.header.stamp.to_sec(),'gps_stamp_ros':pose.header.stamp.to_sec(),'imu_stamp_ros':orientation.header.stamp.to_sec(),'wall_unix':time.time(),'monotonic':now,'frame_id':m.header.frame_id,'position':[p.x,p.y,p.z],'orientation_xyzw':[q.x,q.y,q.z,q.w],'input_points':m.width*m.height,'saved_points':len(xyz),'point_frame':'message frame; not transformed'}
  np.savez_compressed(os.path.join(out,'scan_%03d.npz'%index),xyz=xyz,metadata=json.dumps(meta))
 except Exception as exc:
  with lock:stream.write(json.dumps({'topic':'probe_error','wall_unix':time.time(),'error':repr(exc)})+'\n')
subs=[rospy.Subscriber('/airsim_node/drone_1/gps',PoseStamped,gps,queue_size=1),rospy.Subscriber('/airsim_node/drone_1/imu/imu',Imu,imu,queue_size=1),rospy.Subscriber('/airsim_node/drone_1/lidar',PointCloud2,lidar,queue_size=1,buff_size=2**24),rospy.Subscriber('/rmua/controller/command_trace',String,telemetry,'command_trace',queue_size=200),rospy.Subscriber('/rmua/controller/telemetry',String,telemetry,'controller_telemetry',queue_size=200)]
try:rospy.spin()
finally:
 for sub in subs:sub.unregister()
 with lock:stream.close()
