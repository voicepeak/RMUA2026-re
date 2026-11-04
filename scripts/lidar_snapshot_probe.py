#!/usr/bin/env python3
"""Capture one unmodified ROS PointCloud2 payload and its layout; no control input."""
import hashlib,json,sys,time
from pathlib import Path
import rospy
from sensor_msgs.msg import PointCloud2
rospy.init_node('rmua_lidar_snapshot_probe',anonymous=True,disable_signals=True)
out=Path(sys.argv[1]); captured=False
def callback(msg):
 global captured
 if captured:return
 captured=True; payload=bytes(msg.data); (out/'lidar_payload.bin').write_bytes(payload)
 data={'arrival_wall_ns':time.time_ns(),'arrival_monotonic_ns':time.monotonic_ns(),'header_stamp_ns':msg.header.stamp.to_nsec(),'frame_id':msg.header.frame_id,'seq':msg.header.seq,'height':msg.height,'width':msg.width,'is_bigendian':msg.is_bigendian,'point_step':msg.point_step,'row_step':msg.row_step,'is_dense':msg.is_dense,'fields':[{'name':f.name,'offset':f.offset,'datatype':f.datatype,'count':f.count} for f in msg.fields],'payload_bytes':len(payload),'payload_sha256':hashlib.sha256(payload).hexdigest()}
 (out/'lidar_metadata.json').write_text(json.dumps(data,indent=2)+'\n')
sub=rospy.Subscriber('/airsim_node/drone_1/lidar',PointCloud2,callback,queue_size=1,buff_size=2**24)
start=time.monotonic()
while not captured and time.monotonic()-start<float(sys.argv[2]) and not rospy.is_shutdown():time.sleep(.05)
sub.unregister()
if not captured:raise SystemExit('no lidar frame observed')
