#!/usr/bin/env python3
"""Publish a fixed, open-loop symmetric PWM sequence in a disposable diagnostic scene."""
import json,sys,time
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
rospy.init_node('rmua_fixed_pwm_wind_probe',anonymous=True,disable_signals=True)
pub=rospy.Publisher('/airsim_node/drone_1/rotor_pwm_cmd',RotorPWM,queue_size=10)
out=open(sys.argv[1],'w',buffering=1)
# Open loop: no sensor or truth subscription. Finish with zero commands.
phases=[(1.0,0.0),(0.75,0.2),(0.75,0.3),(0.75,0.4),(0.75,0.2),(1.0,0.0)]
start=time.monotonic();index=0; boundaries=[];total=0
for duration,value in phases:total+=duration;boundaries.append((total,value))
while not rospy.is_shutdown():
 now=time.monotonic();t=now-start
 if t>=total:break
 while t>=boundaries[index][0]:index+=1
 value=boundaries[index][1];msg=RotorPWM();msg.header.stamp=rospy.Time.now()
 msg.rotorPWM0=msg.rotorPWM1=msg.rotorPWM2=msg.rotorPWM3=value;pub.publish(msg)
 out.write(json.dumps({'elapsed_s':t,'wall_ns':time.time_ns(),'mono_ns':time.monotonic_ns(),'header_ns':msg.header.stamp.to_nsec(),'command':[value]*4},separators=(',',':'))+'\n');time.sleep(.02)
msg=RotorPWM();msg.header.stamp=rospy.Time.now();pub.publish(msg);out.close()
