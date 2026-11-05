set pagination off
set confirm off
set print thread-events off
set breakpoint pending on
python
import gdb, os, struct, time
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
class ForceBucket(gdb.Breakpoint):
 def stop(self):
  old=int(gdb.parse_and_eval('(int)$eax'))
  print('VEHICLE_FORCE_BUCKET original=%d forced=0'%old,flush=True)
  gdb.execute('set $eax=0')
  self.enabled=False
  return False
class VehicleInit(gdb.Breakpoint):
 def stop(self):
  mem=gdb.selected_inferior(); actor=int(gdb.parse_and_eval('$rdi')); arr=int(gdb.parse_and_eval('$rsi')); speed=float(gdb.parse_and_eval('$xmm0.v4_float[0]'))
  ptr,count,cap=struct.unpack('<Qii',bytes(mem.read_memory(arr,16)))
  print('VEHICLE_INIT actor=%#x speed=%g count=%d cap=%d'%(actor,speed,count,cap),flush=True)
  if 0<=count<=10000:
   for i in range(count):
    raw=bytes(mem.read_memory(ptr+i*12,12)); print('VEHICLE_PATH actor=%#x index=%d hex=%s xyz=%s'%(actor,i,raw.hex(),','.join('%.9g'%x for x in struct.unpack('<fff',raw))),flush=True)
  return False
class VehicleTick(gdb.Breakpoint):
 last={}
 def stop(self):
  actor=int(gdb.parse_and_eval('$rdi')); now=time.monotonic()
  if now-self.last.get(actor,0)<0.5: return False
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
  if now-self.last.get(actor,0)<0.5: return False
  component=components.get(actor,0)
  if component:
   xyz=struct.unpack('<fff',bytes(gdb.selected_inferior().read_memory(component+0x1d0,12)))
   print('VEHICLE_WORLD mono=%.9f actor=%#x component=%#x xyz_cm=%s'%(now,actor,component,','.join('%.9g'%x for x in xyz)),flush=True)
   self.last[actor]=now
  return False
Inject('*0x2fbac30')
ForceBucket('*0x2fc89de')
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
end
run
