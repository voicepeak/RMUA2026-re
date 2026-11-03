#!/usr/bin/env python3
"""GPS/IMU VEL scout for first 17 visual estimates; never subscribes to debug truth.

Diagnostic only: historical stereo reference, not competition-equivalent.
Preserves original scout safety limits. GPS absence or IMU staleness stops flight.
"""
import sys, math, time, json, hashlib
from pathlib import Path
def scout_worker(out):
    import rosgraph
    import rospy
    import yaml
    from airsim_ros.msg import VelCmd
    from geometry_msgs.msg import PoseStamped
    from sensor_msgs.msg import Imu, PointCloud2
    import numpy as np
    from tf.transformations import quaternion_matrix
    sys.path.insert(0, '/workspace/main')
    from adapters.ros_noetic.lidar_cloud import decode_xyz
    from control.control_authority.lease import ControlLease, check_graph
    # Historical gate centers are stereo image estimates, never live debug truth.
    route_file = Path('/analysis/experiments/stage_probe_20261104/visual_gates_seed123.yaml')
    gates = yaml.safe_load(route_file.read_text())['gates']
    points = [(float(g['x']), float(g['y']), float(g['z'])) for g in gates[:17]]
    offsets = {}
    rospy.init_node('alpha_scout_vel', anonymous=True, disable_signals=True)
    latest = {'pose': None, 'pose_wall': 0., 'imu': None, 'imu_wall': 0., 'lidar': None, 'lidar_wall': 0.}
    def pose_cb(msg):
        latest['pose'], latest['pose_wall'] = msg, time.monotonic()
    def imu_cb(msg):
        latest['imu'], latest['imu_wall'] = msg, time.monotonic()
    def lidar_cb(msg):
        latest['lidar'], latest['lidar_wall'] = msg, time.monotonic()
    rospy.Subscriber('/airsim_node/drone_1/lidar', PointCloud2, lidar_cb, queue_size=1, buff_size=2**24)
    rospy.Subscriber('/airsim_node/drone_1/gps', PoseStamped, pose_cb, queue_size=1)
    rospy.Subscriber('/airsim_node/drone_1/imu/imu', Imu, imu_cb, queue_size=1)
    master = rosgraph.Master(rospy.get_name())
    def graph(require_self):
        pubs, _, _ = master.getSystemState()
        check_graph(dict(pubs), rospy.get_name(), 'VEL', require_self)
    result = {'mode':'scout-only', 'control':'VEL', 'position':'GPS PoseStamped; orientation from IMU; no debug truth',
              'route_source':str(route_file), 'route_sha256':hashlib.sha256(route_file.read_bytes()).hexdigest(),
              'coverage_scope':'17 historical stereo estimates; first segment only', 'route_count':len(points), 'waypoints_reached':0, 'fault':None, 'coverage_complete':False,
              'detour':{'target_offsets_ned_xy_m':offsets, 'speed_limit_m_s':0.6, 'later_speed_limit_m_s':1.0, 'min_lidar_clearance_m':1.15, 'horizon_m':2.0, 'forward_ray_min':10, 'note':'scout only; cone support is partial observation, not certified free space'},
              'limits':{'xy_m_s':2.0,'z_m_s':0.6,'rate_rad_s':1.0,'tilt_rad':0.35,
                        'stale_s':0.5,'max_leg_m':45.0}, 'debug':True}
    out = Path(out)
    publisher = None
    try:
        with ControlLease(rospy.get_param('/run_id'), 'VEL'):
            graph(False)
            deadline = time.monotonic()+8
            while time.monotonic() < deadline and (latest['pose'] is None or latest['imu'] is None):
                time.sleep(.05)
            if latest['pose'] is None or latest['imu'] is None:
                raise RuntimeError('POSE_OR_IMU_UNAVAILABLE')
            publisher = rospy.Publisher('/airsim_node/drone_1/vel_body_cmd', VelCmd, queue_size=1)
            deadline = time.monotonic()+3
            while time.monotonic()<deadline and publisher.get_num_connections()!=1:
                time.sleep(.05)
            if publisher.get_num_connections()!=1:
                raise RuntimeError('VEL_CONNECTION_COUNT')
            graph(True)
            pos = latest['pose'].pose.position
            targets = [(pos.x,pos.y,pos.z-1.5)] + points
            start = time.monotonic()
            last_progress = start
            best = float('inf')
            index = 0
            last_graph = 0.
            lidar_bad_since = None
            with (out/'scout_trajectory.jsonl').open('w') as trace:
                while index < len(targets):
                    now = time.monotonic()
                    if now-start > 1800:
                        raise RuntimeError('MISSION_TIMEOUT')
                    if now-latest['pose_wall'] > .5 or now-latest['imu_wall'] > .5:
                        raise RuntimeError('STALE_POSE_OR_IMU')
                    if now-last_graph > 1:
                        graph(True)
                        last_graph = now
                    p = latest['pose'].pose.position
                    q = latest['imu'].orientation
                    if index > 1 and p.x == 0. and p.y == 0. and p.z == 0.:
                        raise RuntimeError('GPS_ZERO_POSITION')
                    imu = latest['imu']
                    omega = math.sqrt(imu.angular_velocity.x**2+imu.angular_velocity.y**2+imu.angular_velocity.z**2)
                    qi = imu.orientation
                    cos_tilt = 1-2*(qi.x*qi.x+qi.y*qi.y)
                    if not math.isfinite(omega) or omega > 1.:
                        raise RuntimeError('ANGULAR_RATE_LIMIT')
                    if not math.isfinite(cos_tilt) or cos_tilt < math.cos(.35):
                        raise RuntimeError('TILT_LIMIT')
                    target = targets[index]
                    if not all(math.isfinite(v) for v in (p.x,p.y,p.z,q.x,q.y,q.z,q.w,*target)):
                        raise RuntimeError('NONFINITE_POSE_OR_TARGET')
                    if abs(q.x*q.x+q.y*q.y+q.z*q.z+q.w*q.w-1.) > .05:
                        raise RuntimeError('INVALID_POSE_QUATERNION')
                    dx,dy,dz = target[0]-p.x,target[1]-p.y,target[2]-p.z
                    distance = math.sqrt(dx*dx+dy*dy+dz*dz)
                    if distance > 45:
                        raise RuntimeError('UNVERIFIED_ROUTE_GAP_%d_%.1fm'%(index,distance))
                    clearance = None
                    clearance_required = None
                    forward_support = None
                    if index >= 59:
                        if latest['lidar'] is None or now-latest['lidar_wall'] > .5:
                            raise RuntimeError('DETOUR_LIDAR_STALE')
                        cloud = decode_xyz(latest['lidar'], 0).points
                        if len(cloud) < 100:
                            raise RuntimeError('DETOUR_LIDAR_INSUFFICIENT')
                        # A zero XYZ is an ambiguous no-return, not obstacle or free space.
                        # Reject the entire scan and hold until a fresh valid scan arrives.
                        if np.any(np.all(cloud == 0., axis=1)):
                            if lidar_bad_since is None:
                                lidar_bad_since = now
                            if now-lidar_bad_since > 1.:
                                raise RuntimeError('DETOUR_LIDAR_INVALID_TIMEOUT')
                            hold = VelCmd()
                            hold.header.stamp = rospy.Time.now()
                            hold.header.frame_id = 'drone_1'
                            hold.va, hold.stop = 4, 0
                            publisher.publish(hold)
                            trace.write(json.dumps({'wall':now, 'event':'lidar_zero_scan_hold',
                                'lidar_stamp':latest['lidar'].header.stamp.to_sec()})+'\n')
                            trace.flush()
                            time.sleep(.1)
                            continue
                        lidar_bad_since = None
                        rotation = quaternion_matrix([q.x,q.y,q.z,q.w])[:3,:3]
                        world = (cloud+np.array([0.,0.,-.05]))@rotation.T+np.array([p.x,p.y,p.z])
                        direction = np.array([dx,dy,dz])
                        horizon = min(2., distance)
                        end = np.array([p.x,p.y,p.z])+direction*horizon/max(distance,1e-9)
                        segment = end-np.array([p.x,p.y,p.z])
                        den = float(segment@segment)
                        relative = world-np.array([p.x,p.y,p.z])
                        projection = np.clip((relative@segment)/max(den,1e-12),0.,1.)
                        closest = relative-projection[:,None]*segment
                        clearance = float(np.sqrt(np.min(np.sum(closest*closest,axis=1))))
                        lidar_age = max(0., now-latest['lidar_wall'])
                        # Reserve for at most 2 m/s translation since scan arrival.
                        clearance_required = 1.15 + 2.*lidar_age
                        ranges = np.linalg.norm(relative,axis=1)
                        cosine = (relative@direction)/(np.maximum(ranges,1e-9)*max(distance,1e-9))
                        forward_support = int(np.count_nonzero((cosine > .92) & (ranges > horizon+1.15)))
                        if forward_support < 10:
                            raise RuntimeError('DETOUR_UNKNOWN_FORWARD_%d'%forward_support)
                        if clearance < clearance_required:
                            raise RuntimeError('DETOUR_CLEARANCE_%.3fm_REQUIRED_%.3fm'%(clearance,clearance_required))
                    tolerance = .35 if index == 0 else 1.5
                    if distance < tolerance:
                        if index:
                            result['waypoints_reached'] = index
                        index += 1
                        best, last_progress = float('inf'), now
                        continue
                    if distance < best-.3:
                        best, last_progress = distance, now
                    if now-last_progress > 25:
                        raise RuntimeError('NO_PROGRESS_WAYPOINT_%d'%index)
                    yaw = math.atan2(2*(q.w*q.z+q.x*q.y), 1-2*(q.y*q.y+q.z*q.z))
                    vx,vy = .35*dx,.35*dy
                    horizontal = math.hypot(vx,vy)
                    speed_limit = .6 if 59 <= index <= 80 else (1. if index > 80 else 2.)
                    if horizontal>speed_limit:
                        vx,vy = vx*speed_limit/horizontal,vy*speed_limit/horizontal
                    cmd = VelCmd()
                    cmd.header.stamp = rospy.Time.now()
                    cmd.header.frame_id = 'drone_1'
                    cmd.vx = math.cos(yaw)*vx+math.sin(yaw)*vy
                    cmd.vy = -math.sin(yaw)*vx+math.cos(yaw)*vy
                    cmd.vz = max(-.6,min(.6,-.35*dz))
                    cmd.yawRate = 0.
                    cmd.va, cmd.stop = 4,0
                    publisher.publish(cmd)
                    trace.write(json.dumps({'wall':now,'ros_gps_stamp':latest['pose'].header.stamp.to_sec(),'wall_unix':time.time(),
                        'position':[p.x,p.y,p.z], 'target':target,'waypoint':index,
                        'velocity_body':[cmd.vx,cmd.vy,cmd.vz], 'angular_rate':omega, 'lidar_clearance_m':clearance, 'lidar_clearance_required_m':clearance_required, 'lidar_forward_support':forward_support})+'\n')
                    trace.flush()
                    time.sleep(.1)
            result['coverage_complete'] = True
    except Exception as exc:
        result['fault'] = str(exc)
    finally:
        if publisher is not None:
            cmd = VelCmd()
            cmd.header.stamp = rospy.Time.now()
            cmd.header.frame_id = 'drone_1'
            cmd.va,cmd.stop = 4,0
            for _ in range(3):
                publisher.publish(cmd)
                time.sleep(.1)
            publisher.unregister()
        (out/'scout_result.json').write_text(json.dumps(result,indent=2)+'\n')
    print(json.dumps(result),flush=True)
    return 0 if result['coverage_complete'] else 1


if __name__ == '__main__':
    sys.exit(scout_worker(sys.argv[1]))
