set pagination off
set confirm off
set print thread-events off
set breakpoint pending on
python
import gdb, os, struct, time
stage=int(os.environ.get('RMUA_STAGE','4'))
args={4:(8,12),5:(12,10),6:(10,7)}[stage]
drone_component=0; drone_actor=0; vehicle_actor=0; vehicle_child=0; state_ptr=0
tick=0; done=False; state_forced=False
START=int(os.environ.get('RMUA_VEHICLE_START_TICK','30'))
DURATION=int(os.environ.get('RMUA_VEHICLE_TELEPORT_TICKS','120'))
class Inject(gdb.Breakpoint):
 def stop(self):
  if int(gdb.parse_and_eval('$ecx')) != 1: return False
  state=int(gdb.parse_and_eval('$rdi'))
  print('INJECT state=%#x old=(%d,%d,%d) new=(%d,%d,%d)'%(state,int(gdb.parse_and_eval('$esi')),int(gdb.parse_and_eval('$edx')),int(gdb.parse_and_eval('$ecx')),args[0],args[1],stage),flush=True)
  gdb.execute('set $esi=%d'%args[0]);gdb.execute('set $edx=%d'%args[1]);gdb.execute('set $ecx=%d'%stage);self.enabled=False;return False
class Redirect(gdb.Breakpoint):
 def __init__(self): super().__init__('*0x2fbb5a9',internal=True)
 def stop(self):
  mem=gdb.selected_inferior();self.old=bytes(mem.read_memory(0x2fbb5aa,4));mem.write_memory(0x2fbb5aa,struct.pack('<i',0x2fc8900-0x2fbb5ae));gdb.execute('set $xmm0.v4_float[0]=1000.0')
  print('VEHICLE_REDIRECT route=%s distance_cm=1000'%gdb.parse_and_eval('$rdi'),flush=True);RedirectReturn(self);self.enabled=False;return False
class RedirectReturn(gdb.Breakpoint):
 def __init__(self,owner): super().__init__('*0x2fbb5ae',internal=True);self.owner=owner
 def stop(self):
  gdb.selected_inferior().write_memory(0x2fbb5aa,self.owner.old);print('VEHICLE_REDIRECT_RETURNED',flush=True);self.enabled=False;return False
class ForceGate(gdb.Breakpoint):
 def stop(self):
  route=int(gdb.parse_and_eval('$r14'));index=int(gdb.parse_and_eval('(int)$ebx'));speed=float(gdb.parse_and_eval('*(float*)($rsp+0x1c)'))
  print('VEHICLE_FORCE_GATE route=%#x state_index=%d speed=%g'%(route,index,speed),flush=True)
  if index==-1: gdb.execute('set $ebx=0');print('VEHICLE_FORCE_GATE_INDEX_ZERO',flush=True)
  return False
class VehicleInit(gdb.Breakpoint):
 def stop(self):
  global vehicle_actor
  mem=gdb.selected_inferior();vehicle_actor=int(gdb.parse_and_eval('$rdi'));arr=int(gdb.parse_and_eval('$rsi'));speed=float(gdb.parse_and_eval('$xmm0.v4_float[0]'))
  ptr,count,cap=struct.unpack('<Qii',bytes(mem.read_memory(arr,16)))
  print('VEHICLE_INIT actor=%#x speed=%g count=%d'%(vehicle_actor,speed,count),flush=True)
  if 0<=count<=10000:
   for i in range(count):
    raw=bytes(mem.read_memory(ptr+i*12,12));print('VEHICLE_PATH actor=%#x index=%d hex=%s xyz=%s'%(vehicle_actor,i,raw.hex(),','.join('%.9g'%x for x in struct.unpack('<fff',raw))),flush=True)
  return False
class VehicleTick(gdb.Breakpoint):
 last=0
 def stop(self):
  global vehicle_child
  actor=int(gdb.parse_and_eval('$rdi'));now=time.monotonic()
  if now-self.last<0.2:return False
  mem=gdb.selected_inferior();active=bytes(mem.read_memory(actor+0x230,1))[0]
  if active:
   position,speed=struct.unpack('<ff',bytes(mem.read_memory(actor+0x234,8)))
   candidate=struct.unpack('<Q',bytes(mem.read_memory(actor+0x240,8)))[0]
   if candidate:
    vehicle_child=candidate
   print('VEHICLE_TICK mono=%.9f actor=%#x progress=%.9g speed=%.9g child=%#x'%(now,actor,position,speed,candidate),flush=True)
   self.last=now
  return False
class EventSource(gdb.Breakpoint):
 def stop(self):
  global drone_component,drone_actor
  if not drone_component:
   drone_actor=int(gdb.parse_and_eval('$rdi'));drone_component=int(gdb.parse_and_eval('$rsi'));print('NATURAL_VEHICLE_DRONE_COMPONENT actor=%#x component=%#x'%(drone_actor,drone_component),flush=True)
  print('NATURAL_VEHICLE_HIT_SOURCE mono_ns=%d drone=%#x drone_component=%#x source_actor=%#x source_component=%#x'%(time.monotonic_ns(),int(gdb.parse_and_eval('$rdi')),int(gdb.parse_and_eval('$rsi')),int(gdb.parse_and_eval('$rdx')),int(gdb.parse_and_eval('$rcx'))),flush=True)
  return False
class StateRecord(gdb.Breakpoint):
 def stop(self):
  global state_ptr
  state_ptr=int(gdb.parse_and_eval('$rdi'));return False
class StateChange(gdb.Breakpoint):
 def stop(self):
  print('NATURAL_VEHICLE_STATE_REQUEST mono_ns=%d tid=%d state=%#x requested=%d'%(time.monotonic_ns(),gdb.selected_thread().ptid[1],int(gdb.parse_and_eval('$rdi')),int(gdb.parse_and_eval('$esi'))),flush=True);return False
class StateWritten(gdb.Breakpoint):
 def stop(self):
  state=int(gdb.parse_and_eval('$rbx'));cur,prev=struct.unpack('<II',bytes(gdb.selected_inferior().read_memory(state+0xd0,8)))
  print('NATURAL_VEHICLE_STATE_WRITTEN mono_ns=%d tid=%d current=%d previous=%d'%(time.monotonic_ns(),gdb.selected_thread().ptid[1],cur,prev),flush=True);return False
class Physics(gdb.Breakpoint):
 def stop(self):
  global tick,done,state_forced
  if done or not drone_component or not vehicle_child: return False
  tick+=1
  if tick<START: return False
  if tick>START+DURATION:
   done=True;print('NATURAL_VEHICLE_TELEPORT_DONE tick=%d'%tick,flush=True);return False
  mem=gdb.selected_inferior()
  try:
   vehicle_xyz=struct.unpack('<fff',bytes(mem.read_memory(vehicle_child+0x1d0,12)))
   before=struct.unpack('<fff',bytes(mem.read_memory(drone_component+0x1d0,12)))
   # place the drone collision component at the vehicle child, offset 50 cm
   target=(vehicle_xyz[0],vehicle_xyz[1],vehicle_xyz[2]+100.0)
   mem.write_memory(drone_component+0x1d0,struct.pack('<fff',*target))
   if state_ptr:
    mem.write_memory(state_ptr+0xd0,struct.pack('<II',7,0));mem.write_memory(state_ptr+0x180,b'\x01');mem.write_memory(state_ptr+0x1a8,struct.pack('<f',0.0));mem.write_memory(state_ptr+0x1ac,struct.pack('<f',0.0))
    if not state_forced:
     state_forced=True;print('NATURAL_VEHICLE_STATE_FORCE state=%#x tick=%d'%(state_ptr,tick),flush=True)
  except gdb.MemoryError:
   done=True;print('NATURAL_VEHICLE_UNREADABLE',flush=True);return False
  if tick<=START+2 or tick%50==0:
   print('NATURAL_VEHICLE_TELEPORT tick=%d vehicle=%s before=%s target=%s'%(tick,','.join('%.9g'%x for x in vehicle_xyz),','.join('%.9g'%x for x in before),','.join('%.9g'%x for x in target)),flush=True)
  return False
Inject('*0x2fbac30')
ForceGate('*0x2fc9469')
VehicleInit('*0x2fbff60')
VehicleTick('*0x2fc0180')
StateRecord('*0x2fb8f40')
Redirect()
EventSource('*0x2630780')
StateChange('*0x2fb9960')
StateWritten('*0x2fb9be0')
Physics('*0x268f19b')
end
run
