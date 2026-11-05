#!/usr/bin/env python3
"""Fixed open-loop PWM followed immediately by a ROS reset service call."""
import json,sys,time
import rospy
from airsim_ros.srv import Reset
try:
 from airsim_ros.msg import RotorPWM
except ImportError:
 import genpy.dynamic
 definition=('std_msgs/Header header\nfloat64 rotorPWM0\nfloat64 rotorPWM1\nfloat64 rotorPWM2\nfloat64 rotorPWM3\n'+'='*80+'\nMSG: std_msgs/Header\nuint32 seq\ntime stamp\nstring frame_id\n')
 RotorPWM=genpy.dynamic.generate_dynamic('airsim_ros/RotorPWM',definition)['airsim_ros/RotorPWM']
 if RotorPWM._md5sum!='4967432b608c06d70b17be20124fa258':raise RuntimeError('message MD5 mismatch')
rospy.init_node('rmua_open_loop_reset_probe',anonymous=True,disable_signals=True)
rospy.wait_for_service('/airsim_node/reset',timeout=10)
service=rospy.ServiceProxy('/airsim_node/reset',Reset)
pub=rospy.Publisher('/airsim_node/drone_1/rotor_pwm_cmd',RotorPWM,queue_size=10)
out=open(sys.argv[1],'w',buffering=1);start=time.monotonic()
while not rospy.is_shutdown() and time.monotonic()-start<3.5:
 elapsed=time.monotonic()-start; value=0.0 if elapsed<1.0 else 0.3
 msg=RotorPWM();msg.header.stamp=rospy.Time.now();msg.rotorPWM0=msg.rotorPWM1=msg.rotorPWM2=msg.rotorPWM3=value
 pub.publish(msg);out.write(json.dumps({'event':'command','elapsed_s':elapsed,'mono_ns':time.monotonic_ns(),'header_ns':msg.header.stamp.to_nsec(),'command':[value]*4})+'\n');time.sleep(.02)
out.write(json.dumps({'event':'reset_call','mono_ns':time.monotonic_ns()})+'\n')
try:
 response=service();out.write(json.dumps({'event':'reset_response','mono_ns':time.monotonic_ns(),'success':bool(response.success)})+'\n');print('success: '+str(response.success),flush=True)
finally:
 msg=RotorPWM();msg.header.stamp=rospy.Time.now();pub.publish(msg)
 out.write(json.dumps({'event':'post_reset_zero_command','mono_ns':time.monotonic_ns()})+'\n');out.close()
