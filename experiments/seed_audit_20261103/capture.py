#!/usr/bin/env python3
"""Stationary diagnostic capture; never publishes control or changes settings."""
import json, os, sys, time
import numpy as np
import rospy
import rosgraph
from cv_bridge import CvBridge
from geometry_msgs.msg import PoseStamped, Vector3Stamped
from sensor_msgs.msg import Image, Imu, PointCloud2
from sensor_msgs import point_cloud2
import cv2

name, duration = sys.argv[1], float(sys.argv[2])
out = '/analysis/experiments/seed_audit_20261103/' + name
os.makedirs(out, exist_ok=True)
data = {'name': name, 'duration_wall_s': duration, 'topics': {}, 'wind': [], 'pose': [], 'imu_stamps': []}
bridge = CvBridge()

def count(topic, msg):
    row = data['topics'].setdefault(topic, {'count': 0, 'first_stamp': None, 'last_stamp': None})
    stamp = msg.header.stamp.to_sec()
    row['count'] += 1
    if row['first_stamp'] is None: row['first_stamp'] = stamp
    row['last_stamp'] = stamp

def cb_wind(m):
    count('wind', m)
    data['wind'].append([m.header.stamp.to_sec(), m.vector.x, m.vector.y, m.vector.z])

def cb_pose(m):
    count('pose', m)
    if len(data['pose']) < 10:
        p, q = m.pose.position, m.pose.orientation
        data['pose'].append([m.header.stamp.to_sec(), p.x,p.y,p.z,q.x,q.y,q.z,q.w])

def cb_imu(m):
    count('imu', m)
    data['imu_stamps'].append([m.header.stamp.to_sec(), time.time()])

def cb_image(m):
    count('image', m)
    if data['topics']['image']['count'] == 1:
        data['image_first'] = {'width': m.width, 'height': m.height, 'encoding': m.encoding, 'bytes': len(m.data)}
        if m.width and m.height:
            cv2.imwrite(out + '/first.png', bridge.imgmsg_to_cv2(m, 'bgr8'))

def cb_cloud(m):
    count('lidar', m)
    if data['topics']['lidar']['count'] == 1:
        xyz = np.asarray(list(point_cloud2.read_points(m, field_names=('x','y','z'), skip_nans=True)), dtype=np.float32).reshape(-1,3)
        np.save(out + '/first_lidar.npy', xyz)
        data['lidar_frame'] = m.header.frame_id

rospy.init_node('rmua_seed_audit', anonymous=True, disable_signals=True)
subs = [rospy.Subscriber('/airsim_node/drone_1/debug/wind', Vector3Stamped, cb_wind, queue_size=100),
        rospy.Subscriber('/airsim_node/drone_1/debug/pose_gt', PoseStamped, cb_pose, queue_size=20),
        rospy.Subscriber('/airsim_node/drone_1/imu/imu', Imu, cb_imu, queue_size=200),
        rospy.Subscriber('/airsim_node/drone_1/front_left/Scene', Image, cb_image, queue_size=2),
        rospy.Subscriber('/airsim_node/drone_1/lidar', PointCloud2, cb_cloud, queue_size=2)]
start = time.monotonic()
while time.monotonic() - start < duration and not rospy.is_shutdown(): time.sleep(.05)
for sub in subs: sub.unregister()
data['actual_wall_s'] = time.monotonic() - start
data['master_state'] = rosgraph.Master(rospy.get_name()).getSystemState()
with open(out + '/capture.json', 'w') as f: json.dump(data, f, indent=2)
print(json.dumps({'name': name, 'counts': {k:v['count'] for k,v in data['topics'].items()}, 'wind_samples':len(data['wind'])}))
