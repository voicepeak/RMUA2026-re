#!/usr/bin/env python3
"""Fixed PWM stress input, no sensor or debug-truth subscription."""
import json,sys,time
import rospy,genpy.dynamic
msgdef=('std_msgs/Header header\nfloat64 rotorPWM0\nfloat64 rotorPWM1\nfloat64 rotorPWM2\nfloat64 rotorPWM3\n'+'='*80+'\nMSG: std_msgs/Header\nuint32 seq\ntime stamp\nstring frame_id\n')
RotorPWM=genpy.dynamic.generate_dynamic('airsim_ros/RotorPWM',msgdef)['airsim_ros/RotorPWM']
if RotorPWM._md5sum!='4967432b608c06d70b17be20124fa258':raise RuntimeError('unexpected RotorPWM MD5')
seconds=float(sys.argv[1]);rospy.init_node('rmua_pool_pwm_workload',anonymous=True,disable_signals=True)
pub=rospy.Publisher('/airsim_node/drone_1/rotor_pwm_cmd',RotorPWM,queue_size=10)
def emit(data):print(json.dumps(data,separators=(',',':')),flush=True)
ready_deadline=time.monotonic()+10
while pub.get_num_connections()==0 and time.monotonic()<ready_deadline and not rospy.is_shutdown():time.sleep(.05)
connections=pub.get_num_connections();emit({'event':'publisher_ready','connections':connections,'mono_ns':time.monotonic_ns()})
if not connections:raise SystemExit('no PWM subscriber before deadline')
start=time.monotonic();emit({'event':'started','mono_ns':time.monotonic_ns()})
while not rospy.is_shutdown():
 elapsed=time.monotonic()-start
 if elapsed>=seconds:break
 # A fixed six-second schedule, with no online sensor or truth input.
 phase=elapsed%6;value=0.0 if phase<2 or phase>=5 else (0.2 if phase<3 else (0.3 if phase<4 else 0.4))
 msg=RotorPWM();msg.header.stamp=rospy.Time.now();msg.rotorPWM0=msg.rotorPWM1=msg.rotorPWM2=msg.rotorPWM3=value;pub.publish(msg)
 emit({'event':'command','mono_ns':time.monotonic_ns(),'elapsed_s':elapsed,'header_ns':msg.header.stamp.to_nsec(),'value':value})
 time.sleep(.02)
msg=RotorPWM();msg.header.stamp=rospy.Time.now();pub.publish(msg);emit({'event':'finished','mono_ns':time.monotonic_ns(),'elapsed_s':time.monotonic()-start})
