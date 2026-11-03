#!/usr/bin/env python3
"""Read-only ROS arrival and source-stamp probe; execute inside each container."""
import json
import sys
import threading
import time
import rospy
from sensor_msgs.msg import Imu, PointCloud2

seconds = float(sys.argv[1])
rospy.init_node('rmua_pool_probe', anonymous=True, disable_signals=True)
data = {'imu': [], 'lidar': []}
lock = threading.Lock()
def callback(message, key):
    row = [time.monotonic(), time.time(), message.header.stamp.to_sec(), message.header.seq]
    with lock: data[key].append(row)
subs = [rospy.Subscriber('/airsim_node/drone_1/imu/imu', Imu, callback, 'imu', queue_size=100),
        rospy.Subscriber('/airsim_node/drone_1/lidar', PointCloud2, callback, 'lidar', queue_size=10)]
start = time.monotonic()
while time.monotonic()-start < seconds and not rospy.is_shutdown(): time.sleep(.05)
for sub in subs: sub.unregister()
with lock:
    print(json.dumps({'wall_duration_s':time.monotonic()-start,'samples':data},separators=(',',':')),flush=True)
