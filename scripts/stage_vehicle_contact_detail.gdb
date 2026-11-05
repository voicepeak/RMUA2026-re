set pagination off
set confirm off
set print thread-events off
set breakpoint pending on
python
import gdb, os, struct, time
wind_ready=False
pending_hits={}
stage=int(os.environ['RMUA_STAGE'])
args={4:(8,12),5:(12,10),6:(10,7)}[stage]
class Inject(gdb.Breakpoint):
 def stop(self):
  if int(gdb.parse_and_eval('$ecx')) != 1: return False
  state=int(gdb.parse_and_eval('$rdi'))
  print('INJECT state=%#x old=(%d,%d,%d) new=(%d,%d,%d)' % (state,int(gdb.parse_and_eval('$esi')),int(gdb.parse_and_eval('$edx')),int(gdb.parse_and_eval('$ecx')),args[0],args[1],stage),flush=True)
  gdb.execute('set $esi=%d' % args[0]); gdb.execute('set $edx=%d' % args[1]); gdb.execute('set $ecx=%d' % stage)
  self.enabled=False
  return False
class Trace(gdb.Breakpoint):
 def __init__(self,addr,kind): super().__init__('*'+hex(addr), internal=False); self.kind=kind
 def stop(self):
  regs=['rdi','rsi','rdx','rcx','r8','r9']
  vals=' '.join('%s=%#x'%(r,int(gdb.parse_and_eval('$'+r)) & ((1<<64)-1)) for r in regs)
  floats=' '.join('xmm%d=%g'%(i,float(gdb.parse_and_eval('$xmm%d.v4_float[0]'%i))) for i in range(4))
  print('TRACE %s %s %s'%(self.kind,vals,floats),flush=True)
  if self.kind=='event_source':
   pending_hits[gdb.selected_thread().ptid[1]]=(int(gdb.parse_and_eval('$rdx')),int(gdb.parse_and_eval('$rcx')))
   drone_component=int(gdb.parse_and_eval('$rsi')); world=struct.unpack('<fff',bytes(gdb.selected_inferior().read_memory(drone_component+0x1d0,12))); print('HIT_SOURCE mono=%.9f drone=%#x drone_component=%#x drone_world_cm=%s source_actor=%#x source_component=%#x'%(time.monotonic(),int(gdb.parse_and_eval('$rdi')),drone_component,','.join('%.9g'%v for v in world),int(gdb.parse_and_eval('$rdx')),int(gdb.parse_and_eval('$rcx'))),flush=True)
  if self.kind=='event_source' and int(gdb.parse_and_eval('$rdx')) in components:
   source=int(gdb.parse_and_eval('$rdx')); other=int(gdb.parse_and_eval('$rcx')); hitptr=int(gdb.parse_and_eval('$r9'))
   floats=[]
   for register in range(6):
    floats.append([float(gdb.parse_and_eval('$xmm%d.v4_float[%d]'%(register,j))) for j in range(2)])
   try: raw=bytes(gdb.selected_inferior().read_memory(hitptr,0x88)).hex()
   except gdb.MemoryError: raw='unreadable'
   print('VEHICLE_CONTACT_RAW mono=%.9f source_actor=%#x source_component=%#x hitptr=%#x self_moved=%d xmm=%s hit_bytes=%s'%(time.monotonic(),source,other,hitptr,int(gdb.parse_and_eval('$r8')),repr(floats),raw),flush=True)
  if self.kind=='collision_callback':
   print('VEHICLE_EVENT_CALLBACK mono=%.9f tid=%d event=%#x'%(time.monotonic(),gdb.selected_thread().ptid[1],int(gdb.parse_and_eval('$rsi'))),flush=True)
  if self.kind=='wind': self.context=int(gdb.parse_and_eval('$rdi')); WindDone(self.context)
  return False
class WindDone(gdb.Breakpoint):
 def __init__(self,context): super().__init__('*0x2fbbf1f',temporary=True,internal=True); self.context=context
 def stop(self):
  mem=gdb.selected_inferior(); ctx=bytes(mem.read_memory(self.context,24)); ptr,count,cap,other=struct.unpack('<QiiQ',ctx)
  print('WIND_ARRAY context=%#x ptr=%#x count=%d capacity=%d tail=%#x'%(self.context,ptr,count,cap,other),flush=True)
  if 0<=count<=1000:
   for i in range(count):
    raw=bytes(mem.read_memory(ptr+i*16,16)); print('WIND_ITEM %d hex=%s floats=%s'%(i,raw.hex(),','.join('%.9g'%v for v in struct.unpack('<4f',raw))),flush=True)
  global wind_ready
  wind_ready=True
  return False
class Redirect(gdb.Breakpoint):
 def __init__(self): super().__init__('*0x2fbb5a9',internal=True)
 def stop(self):
  mem=gdb.selected_inferior(); self.old=bytes(mem.read_memory(0x2fbb5aa,4))
  mem.write_memory(0x2fbb5aa,struct.pack('<i',0x2fc8900-0x2fbb5ae))
  gdb.execute('set $xmm0.v4_float[0]=1000.0')
  print('VEHICLE_REDIRECT route=%s distance_cm=1000' % gdb.parse_and_eval('$rdi'),flush=True)
  RedirectReturn(self); self.enabled=False
  return False
class RedirectReturn(gdb.Breakpoint):
 def __init__(self,owner): super().__init__('*0x2fbb5ae',internal=True); self.owner=owner
 def stop(self):
  gdb.selected_inferior().write_memory(0x2fbb5aa,self.owner.old)
  print('VEHICLE_REDIRECT_RETURNED',flush=True); self.enabled=False
  return False
class VehicleInit(gdb.Breakpoint):
 def stop(self):
  mem=gdb.selected_inferior(); actor=int(gdb.parse_and_eval('$rdi')); arr=int(gdb.parse_and_eval('$rsi')); speed=float(gdb.parse_and_eval('$xmm0.v4_float[0]'))
  ptr,count,cap=struct.unpack('<Qii',bytes(mem.read_memory(arr,16)))
  print('VEHICLE_INIT actor=%#x speed=%g count=%d cap=%d'%(actor,speed,count,cap),flush=True)
  if os.environ['RMUA_VEHICLE_FIXTURE']=='1' and count==5:
   before=bytes(mem.read_memory(ptr,60)); after=b''.join(struct.pack('<fff',float(x),0.0,float(os.environ['RMUA_PATH_Z'])) for x in (-5000,-3750,-2500,-1250,0));mem.write_memory(ptr,after)
   print('VEHICLE_FIXTURE coordinate_source=fixed_world_path old=%s new=%s'%(before.hex(),after.hex()),flush=True)
  if 0<=count<=10000:
   for i in range(count):
    raw=bytes(mem.read_memory(ptr+i*12,12)); print('VEHICLE_PATH actor=%#x index=%d hex=%s xyz=%s'%(actor,i,raw.hex(),','.join('%.9g'%x for x in struct.unpack('<fff',raw))),flush=True)
  return False
class VehicleTick(gdb.Breakpoint):
 last={}
 def stop(self):
  actor=int(gdb.parse_and_eval('$rdi')); now=time.monotonic()
  if now-self.last.get(actor,0)<0.1: return False
  mem=gdb.selected_inferior(); active=bytes(mem.read_memory(actor+0x230,1))[0]
  if active:
   position,speed=struct.unpack('<ff',bytes(mem.read_memory(actor+0x234,8)))
   print('VEHICLE_TICK mono=%.9f actor=%#x progress=%.9g speed=%.9g dt=%.9g'%(now,actor,position,speed,float(gdb.parse_and_eval('$xmm0.v4_float[0]'))),flush=True)
   self.last[actor]=now
  return False
class ForceGate(gdb.Breakpoint):
 def stop(self):
  route=int(gdb.parse_and_eval('$r14')); index=int(gdb.parse_and_eval('(int)$ebx')); speed=float(gdb.parse_and_eval('*(float*)($rsp+0x1c)')); count=int(gdb.parse_and_eval('*(int*)($r14+0x248)'))
  print('VEHICLE_FORCE_GATE route=%#x state_index=%d speed=%g candidates=%d' % (route,index,speed,count),flush=True)
  if index == -1:
   gdb.execute('set $ebx=0'); print('VEHICLE_FORCE_GATE_INDEX_ZERO',flush=True)
  return False
components={}
class ComponentBefore(gdb.Breakpoint):
 def stop(self):
  components[int(gdb.parse_and_eval('$rbx'))]=int(gdb.parse_and_eval('$rax')); return False
class ComponentAfter(gdb.Breakpoint):
 last={}
 def stop(self):
  actor=int(gdb.parse_and_eval('$rbx')); now=time.monotonic()
  if now-self.last.get(actor,0)<0.1: return False
  component=components.get(actor,0)
  if component:
   xyz=struct.unpack('<fff',bytes(gdb.selected_inferior().read_memory(component+0x1d0,12)))
   quat=struct.unpack('<ffff',bytes(gdb.selected_inferior().read_memory(component+0x1c0,16))); print('VEHICLE_WORLD mono=%.9f actor=%#x component=%#x xyz_cm=%s quat=%s'%(now,actor,component,','.join('%.9g'%x for x in xyz),','.join('%.9g'%x for x in quat)),flush=True)
   mem=gdb.selected_inferior()
   try:
    candidate=struct.unpack('<Q',bytes(mem.read_memory(actor+0x240,8)))[0]
    if candidate:
     outer=struct.unpack('<Q',bytes(mem.read_memory(candidate+0x20,8)))[0]; nameidx,number=struct.unpack('<II',bytes(mem.read_memory(candidate+0x18,8)))
     if outer==actor:
      loc=struct.unpack('<fff',bytes(mem.read_memory(candidate+0x1d0,12)));q=struct.unpack('<ffff',bytes(mem.read_memory(candidate+0x1c0,16)));scale=struct.unpack('<fff',bytes(mem.read_memory(candidate+0x1e0,12)))
      print('VEHICLE_CHILD mono=%.9f actor=%#x offset=0x240 component=%#x outer=%#x fname=%d:%d xyz_cm=%s quat=%s scale=%s'%(now,actor,candidate,outer,nameidx,number,','.join('%.9g'%x for x in loc),','.join('%.9g'%x for x in q),','.join('%.9g'%x for x in scale)),flush=True)
     else: print('VEHICLE_CHILD_CANDIDATE actor=%#x component=%#x outer=%#x'%(actor,candidate,outer),flush=True)
   except gdb.MemoryError: print('VEHICLE_CHILD_UNREADABLE actor=%#x'%actor,flush=True)

   self.last[actor]=now
  return False
class ForceWindTick(gdb.Breakpoint):
 started=False
 last=0
 def stop(self):
  global wind_ready
  if not wind_ready: return False
  mem=gdb.selected_inferior(); state=int(gdb.parse_and_eval('$rdi')); config=struct.unpack('<Q',bytes(mem.read_memory(0x719f540,8)))[0]
  oldstate=struct.unpack('<I',bytes(mem.read_memory(state+0xd0,4)))[0]; newstate={4:7,5:9,6:11}[stage]; offset={4:0x158,5:0x160,6:0x168}[stage]
  clock=time.monotonic_ns()
  if not self.started:
   mem.write_memory(state+offset,struct.pack('<Q',clock));mem.write_memory(config+0x2d4,b'\x01');self.started=True
   print('FORCE_WIND_CLOCK monotonic_ns=%d wall_ns=%d'%(clock,time.time_ns()),flush=True)
  mem.write_memory(state+0xd0,struct.pack('<I',newstate))
  if clock-self.last>1000000000:
   index=struct.unpack('<I',bytes(mem.read_memory(state+0x210,4)))[0]
   print('FORCE_WIND_TICK state=%#x old=%d new=%d index=%d mono_ns=%d'%(state,oldstate,newstate,index,clock),flush=True);self.last=clock
  return False
class DroneWorld(gdb.Breakpoint):
 last=0
 def stop(self):
  now=time.monotonic()
  if now-self.last < 0.1: return False
  root=int(gdb.parse_and_eval('$rbx'));actor=int(gdb.parse_and_eval('$r14'))
  if not root:return False
  mem=gdb.selected_inferior();xyz=struct.unpack('<fff',bytes(mem.read_memory(root+0x1d0,12)));quat=struct.unpack('<ffff',bytes(mem.read_memory(root+0x1c0,16)))
  print('DRONE_WORLD mono=%.9f actor=%#x component=%#x xyz_cm=%s quat=%s'%(now,actor,root,','.join('%.9g'%x for x in xyz),','.join('%.9g'%x for x in quat)),flush=True);self.last=now
  return False
class ParkedSample(gdb.Breakpoint):
 last={}
 def stop(self):
  actor=int(gdb.parse_and_eval('$rdi')); now=time.monotonic()
  if now-self.last.get(actor,0)<0.1:return False
  mem=gdb.selected_inferior(); progress=struct.unpack('<f',bytes(mem.read_memory(actor+0x234,4)))[0]
  if progress<4800:return False
  component=components.get(actor,0)
  if not component:return False
  xyz=struct.unpack('<fff',bytes(mem.read_memory(component+0x1d0,12)))
  quat=struct.unpack('<ffff',bytes(mem.read_memory(component+0x1c0,16)))
  print('VEHICLE_WORLD mono=%.9f actor=%#x component=%#x xyz_cm=%s quat=%s'%(now,actor,component,','.join('%.9g'%x for x in xyz),','.join('%.9g'%x for x in quat)),flush=True)
  candidate=struct.unpack('<Q',bytes(mem.read_memory(actor+0x240,8)))[0]
  if candidate:
   outer=struct.unpack('<Q',bytes(mem.read_memory(candidate+0x20,8)))[0];nameidx,number=struct.unpack('<II',bytes(mem.read_memory(candidate+0x18,8)))
   if outer==actor:
    loc=struct.unpack('<fff',bytes(mem.read_memory(candidate+0x1d0,12)));q=struct.unpack('<ffff',bytes(mem.read_memory(candidate+0x1c0,16)));scale=struct.unpack('<fff',bytes(mem.read_memory(candidate+0x1e0,12)))
    print('VEHICLE_CHILD mono=%.9f actor=%#x offset=0x240 component=%#x outer=%#x fname=%d:%d xyz_cm=%s quat=%s scale=%s'%(now,actor,candidate,outer,nameidx,number,','.join('%.9g'%x for x in loc),','.join('%.9g'%x for x in q),','.join('%.9g'%x for x in scale)),flush=True)
  self.last[actor]=now
  return False
ParkedSample('*0x2fc0180')
class HitEventAllocated(gdb.Breakpoint):
 def stop(self):
  tid=gdb.selected_thread().ptid[1]; source=pending_hits.pop(tid,None)
  if source:
   print('VEHICLE_EVENT_ALLOC mono=%.9f tid=%d source_actor=%#x source_component=%#x event=%#x'%(time.monotonic(),tid,source[0],source[1],int(gdb.parse_and_eval('$rax'))),flush=True)
  return False
HitEventAllocated('*0x2630797')
DroneWorld('*0x26302df')
Inject('*0x2fbac30')
ForceWindTick('*0x2fb8f40')
ComponentBefore('*0x2fc022e')
ComponentAfter('*0x2fc0252')
ForceGate('*0x2fc9469')
VehicleInit('*0x2fbff60')
VehicleTick('*0x2fc0180')
Redirect()
Trace(0x2fb23b0,'wind')
Trace(0x2fc77b0,'line')
Trace(0x2fc3fc0,'thick_place')
Trace(0x2fc4980,'thin_place')
Trace(0x2fc8900,'vehicle')
Trace(0x2fc93c0,'vehicle_branch')
Trace(0x2630780,'event_source')
Trace(0x2fb7110,'collision_callback')
Trace(0x2fb9960,'state_change')
end
run
