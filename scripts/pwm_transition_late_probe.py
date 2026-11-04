#!/usr/bin/env python3
"""Publish a fixed, open-loop symmetric PWM sequence in a disposable diagnostic scene."""
import json,sys,time
import rospy
from airsim_ros.msg import RotorPWM
rospy.init_node('rmua_fixed_pwm_wind_probe',anonymous=True,disable_signals=True)
pub=rospy.Publisher('/airsim_node/drone_1/rotor_pwm_cmd',RotorPWM,queue_size=10)
out=open(sys.argv[1],'w',buffering=1)
# Open loop: no sensor or truth subscription. Finish with zero commands.
phases=[(1.0,0.0),(0.75,0.2),(0.75,0.3),(0.75,0.4),(0.75,0.2),(1.0,0.0)]
time.sleep(6.5)  # fixed delay, independent of all observations
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
