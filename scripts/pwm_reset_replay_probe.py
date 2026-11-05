#!/usr/bin/env python3
"""Publish two identical fixed PWM sequences around a controlled ROS reset."""
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
rospy.init_node('rmua_pwm_reset_replay_probe',anonymous=True,disable_signals=True)
rospy.wait_for_service('/airsim_node/reset',timeout=10)
service=rospy.ServiceProxy('/airsim_node/reset',Reset)
pub=rospy.Publisher('/airsim_node/drone_1/rotor_pwm_cmd',RotorPWM,queue_size=10)
out=open(sys.argv[1],'w',buffering=1)
def emit(row):out.write(json.dumps(row,separators=(',',':'))+'\n')
def publish(value,phase,elapsed):
 msg=RotorPWM();msg.header.stamp=rospy.Time.now();msg.rotorPWM0=msg.rotorPWM1=msg.rotorPWM2=msg.rotorPWM3=value;pub.publish(msg)
 emit({'event':'command','phase':phase,'elapsed_s':elapsed,'mono_ns':time.monotonic_ns(),'header_ns':msg.header.stamp.to_nsec(),'command':[value]*4})
def phase(name,duration,sequence):
 start=time.monotonic();emit({'event':'phase_start','phase':name,'mono_ns':time.monotonic_ns()})
 while not rospy.is_shutdown():
  elapsed=time.monotonic()-start
  if elapsed>=duration:break
  value=0.0 if not sequence or elapsed<1.0 else 0.3
  publish(value,name,elapsed);time.sleep(.02)
 emit({'event':'phase_end','phase':name,'mono_ns':time.monotonic_ns()})
phase('before',3.5,True)
emit({'event':'reset_call','mono_ns':time.monotonic_ns()})
try:
 response=service();emit({'event':'reset_response','mono_ns':time.monotonic_ns(),'success':bool(response.success)});print('success: '+str(response.success),flush=True)
 phase('settle',3.0,False)
 phase('after',3.5,True)
finally:
 publish(0.0,'final',0);out.close()
