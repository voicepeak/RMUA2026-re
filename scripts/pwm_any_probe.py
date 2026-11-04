#!/usr/bin/env python3
"""Record the published RotorPWM wire format without requiring generated ROS messages."""
import json,struct,sys,time
import rospy
rospy.init_node('rmua_pwm_wire_probe',anonymous=True,disable_signals=True)
out=open(sys.argv[1],'w',buffering=1)
def callback(msg):
 raw=bytes(msg._buff); wall=time.time_ns(); mono=time.monotonic_ns()
 try:
  seq,sec,nsec,n=struct.unpack_from('<IIII',raw); pos=16+n
  if len(raw)!=pos+32: raise ValueError('unexpected wire length')
  values=struct.unpack_from('<4d',raw,pos)
  row={'arrival_wall_ns':wall,'arrival_monotonic_ns':mono,'seq':seq,'header_stamp_ns':sec*1000000000+nsec,'frame_id':raw[16:pos].decode('utf-8','replace'),'pwm':values,'raw_hex':raw.hex(),'type':msg._connection_header.get('type'),'md5sum':msg._connection_header.get('md5sum')}
  if row['type']!='airsim_ros/RotorPWM' or row['md5sum']!='4967432b608c06d70b17be20124fa258': raise ValueError('unexpected ROS type or MD5')
 except Exception as exc: row={'arrival_wall_ns':wall,'arrival_monotonic_ns':mono,'error':str(exc),'raw_hex':raw.hex()}
 out.write(json.dumps(row,separators=(',',':'))+'\n')
sub=rospy.Subscriber('/airsim_node/drone_1/debug/rotor_pwm',rospy.AnyMsg,callback,queue_size=100)
start=time.monotonic()
while time.monotonic()-start<float(sys.argv[2]) and not rospy.is_shutdown():time.sleep(.05)
sub.unregister();out.close()
