set pagination off
set confirm off
set print thread-events off
set breakpoint pending on
python
import gdb, os, struct, time
wind_ready=False
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
 seen_collision=0
 def __init__(self,addr,kind): super().__init__('*'+hex(addr), internal=False); self.kind=kind
 def stop(self):
  regs=['rdi','rsi','rdx','rcx','r8','r9']
  vals=' '.join('%s=%#x'%(r,int(gdb.parse_and_eval('$'+r)) & ((1<<64)-1)) for r in regs)
  floats=' '.join('xmm%d=%g'%(i,float(gdb.parse_and_eval('$xmm%d.v4_float[0]'%i))) for i in range(4))
  print('TRACE %s %s %s'%(self.kind,vals,floats),flush=True)
  if self.kind=='collision_callback' and Trace.seen_collision<5:
   print('COLLISION_STACK_START',flush=True); gdb.execute('bt 12'); print('COLLISION_STACK_END',flush=True); Trace.seen_collision+=1
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
Inject('*0x2fbac30')
ForceWindTick('*0x2fb8f40')
Trace(0x2fb23b0,'wind')
Trace(0x2fc77b0,'line')
Trace(0x2fc3fc0,'thick_place')
Trace(0x2fc4980,'thin_place')
Trace(0x2fc8900,'vehicle')
Trace(0x2fb7110,'collision_callback')
Trace(0x2fb9960,'state_change')
end
run
