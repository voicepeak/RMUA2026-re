#!/usr/bin/env python3
"""Read-only ROS message header and arrival clocks; no debug truth/control input."""
import json, sys, time, threading
import rospy
from sensor_msgs.msg import Image, Imu, PointCloud2

rospy.init_node('rmua_sensor_clock_probe', anonymous=True, disable_signals=True)
stream=open(sys.argv[1], 'w', buffering=1)
lock=threading.Lock()
def callback(msg, topic):
    wall=time.time_ns(); mono=time.monotonic_ns(); ros=rospy.Time.now().to_nsec()
    h=msg.header
    row={'topic':topic,'arrival_wall_ns':wall,'arrival_monotonic_ns':mono,'arrival_ros_ns':ros,
         'header_stamp_ns':h.stamp.to_nsec(),'seq':h.seq,'frame_id':h.frame_id}
    if isinstance(msg,Image):
        row.update(width=msg.width,height=msg.height,encoding=msg.encoding,data_bytes=len(msg.data))
    elif isinstance(msg,PointCloud2):
        row.update(width=msg.width,height=msg.height,data_bytes=len(msg.data))
    with lock: stream.write(json.dumps(row,separators=(',',':'))+'\n')
topics=[('imu','/airsim_node/drone_1/imu/imu',Imu,200),('lidar','/airsim_node/drone_1/lidar',PointCloud2,20),
        ('camera_left','/airsim_node/drone_1/front_left/Scene',Image,10),('camera_right','/airsim_node/drone_1/front_right/Scene',Image,10)]
subs=[rospy.Subscriber(name,typ,callback,key,queue_size=q,buff_size=2**24) for key,name,typ,q in topics]
start=time.monotonic();duration=float(sys.argv[2])
while time.monotonic()-start<duration and not rospy.is_shutdown():time.sleep(.05)
for s in subs:s.unregister()
with lock:stream.close()
