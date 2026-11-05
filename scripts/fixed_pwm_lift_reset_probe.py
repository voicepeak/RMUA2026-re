#!/usr/bin/env python3
"""Fixed open-loop rotor PWM pulse followed by the official reset service.

No sensor or debug-truth subscription participates in the command schedule.
"""
import argparse
import json
import time

import rospy
try:
    from airsim_ros.msg import RotorPWM
except ImportError:
    import genpy.dynamic
    definition = ('std_msgs/Header header\nfloat64 rotorPWM0\nfloat64 rotorPWM1\n'
                  'float64 rotorPWM2\nfloat64 rotorPWM3\n' + '=' * 80 +
                  '\nMSG: std_msgs/Header\nuint32 seq\ntime stamp\nstring frame_id\n')
    RotorPWM = genpy.dynamic.generate_dynamic('airsim_ros/RotorPWM', definition)['airsim_ros/RotorPWM']
    if RotorPWM._md5sum != '4967432b608c06d70b17be20124fa258':
        raise RuntimeError('unexpected RotorPWM message definition')
from airsim_ros.srv import Reset


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('output')
    parser.add_argument('--lift-seconds', type=float, default=0.5)
    parser.add_argument('--value', type=float, default=0.2)
    parser.add_argument('--settle-seconds', type=float, default=0.1)
    args = parser.parse_args()
    rospy.init_node('rmua_fixed_pwm_lift_reset', anonymous=True, disable_signals=True)
    out = open(args.output, 'w', buffering=1)
    pub = rospy.Publisher('/airsim_node/drone_1/rotor_pwm_cmd', RotorPWM, queue_size=10)
    rospy.wait_for_service('/airsim_node/reset', timeout=15.0)
    reset = rospy.ServiceProxy('/airsim_node/reset', Reset)
    rate = rospy.Rate(50)
    start = time.monotonic()
    lift_end = start + args.lift_seconds
    settle_end = lift_end + args.settle_seconds
    try:
        while not rospy.is_shutdown() and time.monotonic() < settle_end:
            now = time.monotonic()
            msg = RotorPWM()
            msg.header.stamp = rospy.Time.now()
            value = args.value if now < lift_end else 0.0
            msg.rotorPWM0 = msg.rotorPWM1 = msg.rotorPWM2 = msg.rotorPWM3 = value
            pub.publish(msg)
            out.write(json.dumps({'mono_ns': time.monotonic_ns(), 'elapsed_s': now - start,
                                  'pwm': value}, separators=(',', ':')) + '\n')
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
        msg = RotorPWM()
        msg.header.stamp = rospy.Time.now()
        pub.publish(msg)
        out.close()


if __name__ == '__main__':
    main()
