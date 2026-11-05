#!/usr/bin/env python3
"""Fixed open-loop vertical pulse followed by the official reset service.

No sensor or debug-truth subscription participates in the command schedule.
"""
import argparse
import json
import time

import rospy
from airsim_ros.msg import VelCmd
from airsim_ros.srv import Reset


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('output')
    parser.add_argument('--lift-seconds', type=float, default=0.9)
    parser.add_argument('--value', type=float, default=0.5)
    parser.add_argument('--settle-seconds', type=float, default=0.15)
    args = parser.parse_args()
    rospy.init_node('rmua_fixed_lift_reset', anonymous=True, disable_signals=True)
    out = open(args.output, 'w', buffering=1)
    pub = rospy.Publisher('/airsim_node/drone_1/vel_body_cmd', VelCmd, queue_size=1)
    rospy.wait_for_service('/airsim_node/reset', timeout=15.0)
    reset = rospy.ServiceProxy('/airsim_node/reset', Reset)
    rate = rospy.Rate(50)
    start = time.monotonic()
    lift_end = start + args.lift_seconds
    settle_end = lift_end + args.settle_seconds
    try:
        while not rospy.is_shutdown() and time.monotonic() < settle_end:
            now = time.monotonic()
            cmd = VelCmd()
            cmd.header.stamp = rospy.Time.now()
            cmd.header.frame_id = 'drone_1'
            cmd.vx = cmd.vy = 0.0
            cmd.vz = args.value if now < lift_end else 0.0
            cmd.yawRate = 0.0
            cmd.va = 8
            cmd.stop = 0
            pub.publish(cmd)
            out.write(json.dumps({'mono_ns': time.monotonic_ns(), 'elapsed_s': now - start,
                                  'vz': cmd.vz}, separators=(',', ':')) + '\n')
            rate.sleep()
        reset_start = time.monotonic()
        response = reset()
        success = bool(getattr(response, 'success', False))
        out.write(json.dumps({'event': 'reset', 'mono_ns': time.monotonic_ns(),
                              'elapsed_s': reset_start - start, 'success': success},
                             separators=(',', ':')) + '\n')
    except rospy.ServiceException as exc:
        out.write(json.dumps({'event': 'reset_error', 'error': str(exc)}) + '\n')
    finally:
        cmd = VelCmd()
        cmd.header.stamp = rospy.Time.now()
        cmd.header.frame_id = 'drone_1'
        cmd.va = 8
        pub.publish(cmd)
        out.close()


if __name__ == '__main__':
    main()
