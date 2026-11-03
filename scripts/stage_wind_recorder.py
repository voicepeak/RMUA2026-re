#!/usr/bin/env python3
"""Read-only ROS wind subscriber; preserve separate arrival and ROS clock domains."""
import json, sys, time
import rospy
from geometry_msgs.msg import Vector3Stamped
rospy.init_node('stage_wind_recorder', anonymous=True, disable_signals=True)
out = open(sys.argv[1], 'w', buffering=1)
def receive(msg):
    v=msg.vector
    out.write(json.dumps({'wall_unix':time.time(), 'monotonic':time.monotonic(),
        'ros_header_stamp':msg.header.stamp.to_sec(), 'seq':msg.header.seq,
        'wind':[v.x,v.y,v.z]})+'\n')
rospy.Subscriber('/airsim_node/drone_1/debug/wind',Vector3Stamped,receive,queue_size=1000)
try: rospy.spin()
finally: out.close()
