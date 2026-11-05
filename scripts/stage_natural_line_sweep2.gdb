set pagination off
set confirm off
set print thread-events off
set breakpoint pending on
python
import gdb, os, struct, time
stage=int(os.environ.get('RMUA_STAGE','4'))
args={4:(8,12),5:(12,10),6:(10,7)}[stage]
wanted_selector=int(os.environ.get('RMUA_LINE_SELECTOR','1'))
wanted_ordinal=int(os.environ.get('RMUA_FIXTURE_ORDINAL','0'))
target=None; target_component=0; selected=0
drone_component=0; drone_actor=0; state_ptr=0; state_forced=False
tick=0; done=False
HOLD=int(os.environ.get('RMUA_PREHOLD_TICKS','10'))
SWEEP=int(os.environ.get('RMUA_SWEEP_TICKS','20'))
POST=int(os.environ.get('RMUA_POSTHOLD_TICKS','100'))
class Inject(gdb.Breakpoint):
 def stop(self):
  if int(gdb.parse_and_eval('$ecx')) != 1: return False
  state=int(gdb.parse_and_eval('$rdi'))
  print('INJECT state=%#x old=(%d,%d,%d) new=(%d,%d,%d)' % (state,int(gdb.parse_and_eval('$esi')),int(gdb.parse_and_eval('$edx')),int(gdb.parse_and_eval('$ecx')),args[0],args[1],stage),flush=True)
  gdb.execute('set $esi=%d' % args[0]); gdb.execute('set $edx=%d' % args[1]); gdb.execute('set $ecx=%d' % stage); self.enabled=False; return False
class LineInstance(gdb.Breakpoint):
 seen_components=set()
 def stop(self):
  global target,target_component,selected
  selector=int(gdb.parse_and_eval('$bpl'))
  component=int(gdb.parse_and_eval('$rdi'))
  if (selector,component) not in self.seen_components:
   self.seen_components.add((selector,component));print('NATURAL_LINE_COMPONENT selector=%d component=%#x'%(selector,component),flush=True)
  if selector!=wanted_selector:return False
  selected+=1; p=int(gdb.parse_and_eval('$rsi')); vals=struct.unpack('<12f',bytes(gdb.selected_inferior().read_memory(p,48)))
  print('NATURAL_LINE_INSTANCE component=%#x selector=%d ordinal=%d xyz_cm=%s'%(int(gdb.parse_and_eval('$rdi')),selector,selected-1,','.join('%.9g'%x for x in vals[4:7])),flush=True)
  if selected-1==wanted_ordinal and target is None:
   target=vals[4:7];target_component=int(gdb.parse_and_eval('$rdi'))
   print('NATURAL_LINE_TARGET component=%#x ordinal=%d xyz_cm=%s'%(target_component,selected-1,','.join('%.9g'%x for x in target)),flush=True)
  return False
class StateRecord(gdb.Breakpoint):
 def stop(self):
  global state_ptr
  state_ptr=int(gdb.parse_and_eval('$rdi'));return False
class EventSource(gdb.Breakpoint):
 def stop(self):
  global drone_component,drone_actor
  if not drone_component:
   drone_actor=int(gdb.parse_and_eval('$rdi'));drone_component=int(gdb.parse_and_eval('$rsi'))
   print('NATURAL_DRONE_COMPONENT actor=%#x component=%#x'%(drone_actor,drone_component),flush=True)
  src_actor=int(gdb.parse_and_eval('$rdx'));src_component=int(gdb.parse_and_eval('$rcx'))
  print('NATURAL_HIT_SOURCE mono_ns=%d tid=%d drone=%#x drone_component=%#x source_actor=%#x source_component=%#x target=%#x'%(time.monotonic_ns(),gdb.selected_thread().ptid[1],int(gdb.parse_and_eval('$rdi')),int(gdb.parse_and_eval('$rsi')),src_actor,src_component,target_component),flush=True)
  ptr=int(gdb.parse_and_eval('$r9'))
  try:
   raw=bytes(gdb.selected_inferior().read_memory(ptr,136));print('NATURAL_HIT_RESULT component=%#x index_candidate=%d bytes=%s'%(src_component,struct.unpack('<i',raw[0x58:0x5c])[0],raw.hex()),flush=True)
  except gdb.MemoryError: print('NATURAL_HIT_RESULT_UNREADABLE',flush=True)
  return False
class Callback(gdb.Breakpoint):
 def stop(self):
  print('NATURAL_EVENT_CALLBACK mono_ns=%d tid=%d event=%#x'%(time.monotonic_ns(),gdb.selected_thread().ptid[1],int(gdb.parse_and_eval('$rsi'))),flush=True);return False
class StateChange(gdb.Breakpoint):
 def stop(self):
  print('NATURAL_STATE_REQUEST mono_ns=%d tid=%d state=%#x requested=%d'%(time.monotonic_ns(),gdb.selected_thread().ptid[1],int(gdb.parse_and_eval('$rdi')),int(gdb.parse_and_eval('$esi'))),flush=True);return False
class StateWritten(gdb.Breakpoint):
 def stop(self):
  state=int(gdb.parse_and_eval('$rbx'));cur,prev=struct.unpack('<II',bytes(gdb.selected_inferior().read_memory(state+0xd0,8)))
  print('NATURAL_STATE_WRITTEN mono_ns=%d tid=%d state=%#x current=%d previous=%d'%(time.monotonic_ns(),gdb.selected_thread().ptid[1],state,cur,prev),flush=True);return False
class Physics(gdb.Breakpoint):
 def stop(self):
  global tick,done,state_forced
  if target is None or not drone_component or done:return False
  if state_ptr and tick>=HOLD+2:
   try:
    mem=gdb.selected_inferior();old=struct.unpack('<I',bytes(mem.read_memory(state_ptr+0xd0,4)))[0]
    mem.write_memory(state_ptr+0xd0,struct.pack('<II',7,0));mem.write_memory(state_ptr+0x180,b'\x01');mem.write_memory(state_ptr+0x1a8,struct.pack('<f',0.0));mem.write_memory(state_ptr+0x1ac,struct.pack('<f',0.0));print('NATURAL_STATE_FORCE state=%#x old=%d new=7 tick=%d'%(state_ptr,old,tick),flush=True);state_forced=True
   except gdb.MemoryError: pass
  if tick>=HOLD+SWEEP+POST:
   done=True;print('NATURAL_SWEEP_DONE tick=%d'%tick,flush=True);return False
  if tick<HOLD:
   x=target[0]-25.0
  elif tick<HOLD+SWEEP:
   frac=(tick-HOLD)/max(1.0,SWEEP-1.0);x=target[0]-25.0+50.0*frac
  else:
   x=target[0]+25.0
  y=target[1];z=target[2]
  mem=gdb.selected_inferior()
  try:
   before=struct.unpack('<3f',bytes(mem.read_memory(drone_component+0x1d0,12)))
   mem.write_memory(drone_component+0x1d0,struct.pack('<fff',x,y,z))
   mem.write_memory(drone_component+0x1c0,struct.pack('<ffff',0.0,0.0,0.0,1.0))
  except gdb.MemoryError:
   done=True;print('NATURAL_SWEEP_UNREADABLE',flush=True);return False
  if tick<=2 or tick%50==0:
   print('NATURAL_SWEEP mono_ns=%d comp=%#x tick=%d before=%s write_cm=%.9g,%.9g,%.9g'%(time.monotonic_ns(),drone_component,tick,','.join('%.9g'%x for x in before),x,y,z),flush=True)
  tick+=1;return False
Inject('*0x2fbac30')
LineInstance('*0x2fc3f52')
StateRecord('*0x2fb8f40')
EventSource('*0x2630780')
Callback('*0x2fb7110')
StateChange('*0x2fb9960')
StateWritten('*0x2fb9be0')
Physics('*0x268f19b')
end
run
