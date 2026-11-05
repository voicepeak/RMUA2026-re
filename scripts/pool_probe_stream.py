#!/usr/bin/env python3
"""Stream timestamp-only ROS observation rows, preserving evidence on early exit."""
import json
import sys
import threading
import time
import rospy
from sensor_msgs.msg import Imu, PointCloud2

seconds = float(sys.argv[1])
rospy.init_node('rmua_pool_stream_probe', anonymous=True, disable_signals=True)
lock = threading.Lock()
def emit(row):
    with lock:
        print(json.dumps(row, separators=(',', ':')), flush=True)
def callback(message, topic):
    emit({'event': 'sample', 'topic': topic, 'monotonic_ns': time.monotonic_ns(),
          'wall_ns': time.time_ns(), 'stamp_ns': message.header.stamp.to_nsec(),
          'sequence': message.header.seq})
subs = [rospy.Subscriber('/airsim_node/drone_1/imu/imu', Imu, callback, 'imu', queue_size=100),
        rospy.Subscriber('/airsim_node/drone_1/lidar', PointCloud2, callback, 'lidar', queue_size=10)]
emit({'event': 'started', 'monotonic_ns': time.monotonic_ns(), 'wall_ns': time.time_ns()})
start = time.monotonic()
while time.monotonic() - start < seconds and not rospy.is_shutdown():
    time.sleep(.05)
for sub in subs:
    sub.unregister()
emit({'event': 'finished', 'monotonic_ns': time.monotonic_ns(), 'wall_ns': time.time_ns()})
