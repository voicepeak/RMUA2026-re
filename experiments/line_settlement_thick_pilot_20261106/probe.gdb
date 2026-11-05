set pagination off
set confirm off
set print thread-events off
set breakpoint pending on
python
import gdb, os, struct, time
wind_ready=False
force_released=False
pending_hits={}
fixture_component=0
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
  if self.kind=='event_source':
   pending_hits[gdb.selected_thread().ptid[1]]=(int(gdb.parse_and_eval('$rdx')),int(gdb.parse_and_eval('$rcx')),struct.unpack('<i',bytes(gdb.selected_inferior().read_memory(int(gdb.parse_and_eval('$r9'))+0x58,4)))[0])
   print('LINE_HIT_SOURCE mono_ns=%d tid=%d actor=%#x component=%#x hitptr=%#x'%(time.monotonic_ns(),gdb.selected_thread().ptid[1],int(gdb.parse_and_eval('$rdx')),int(gdb.parse_and_eval('$rcx')),int(gdb.parse_and_eval('$r9'))),flush=True)
   ptr=int(gdb.parse_and_eval('$r9')); raw=bytes(gdb.selected_inferior().read_memory(ptr,136)); print('HIT_RESULT component=%#x bytes=%s'%(int(gdb.parse_and_eval('$rcx')),raw.hex()),flush=True)
  if self.kind=='event_source' and Trace.seen_collision<5:
   print('COLLISION_STACK_START',flush=True); gdb.execute('bt 12'); print('COLLISION_STACK_END',flush=True)
   print('EVENT_SOURCE_RDI %s'%gdb.parse_and_eval('$rdi'),flush=True); gdb.execute('x/16gx $rsp')
   Trace.seen_collision+=1
  if self.kind=='collision_callback':
   print('LINE_EVENT_CALLBACK mono_ns=%d tid=%d event=%#x'%(time.monotonic_ns(),gdb.selected_thread().ptid[1],int(gdb.parse_and_eval('$rsi'))),flush=True)
  if self.kind=='state_change':
   if force_released and int(gdb.parse_and_eval('$esi'))==12:
    SettlementReturn(int(gdb.parse_and_eval('$rdi')),int(gdb.parse_and_eval('$edx')) & 0xff)
   print('LINE_STATE_REQUEST mono_ns=%d tid=%d state=%#x requested=%d'%(time.monotonic_ns(),gdb.selected_thread().ptid[1],int(gdb.parse_and_eval('$rdi')),int(gdb.parse_and_eval('$esi'))),flush=True)
  if self.kind=='wind': self.context=int(gdb.parse_and_eval('$rdi')); WindDone(self.context)
  return False
def settlement_fields(state):
 mem=gdb.selected_inferior()
 config=struct.unpack('<Q',bytes(mem.read_memory(0x719f540,8)))[0]
 score=struct.unpack('<i',bytes(mem.read_memory(config+0x2fc,4)))[0]
 ptr,count,capacity=struct.unpack('<Qii',bytes(mem.read_memory(config+0x308,16)))
 label=bytes(mem.read_memory(ptr,count*2)).decode('utf-16-le','replace').rstrip('\x00') if ptr and 0<count<=128 else ''
 current,previous=struct.unpack('<II',bytes(mem.read_memory(state+0xd0,8)))
 return config,score,label,current,previous
class SettlementReturn(gdb.FinishBreakpoint):
 def __init__(self,state,flag):
  super().__init__(gdb.newest_frame(),internal=True)
  self.state=state;self.flag=flag;self.tid=gdb.selected_thread().ptid[1]
  self.before=settlement_fields(state)
  print('SETTLEMENT_ENTRY mono_ns=%d tid=%d state=%#x flag=%d config=%#x score=%d current=%d previous=%d label=%s'%(time.monotonic_ns(),self.tid,state,flag,self.before[0],self.before[1],self.before[3],self.before[4],self.before[2]),flush=True)
 def stop(self):
  after=settlement_fields(self.state)
  print('SETTLEMENT_RETURN mono_ns=%d tid=%d state=%#x flag=%d config=%#x score=%d current=%d previous=%d label=%s'%(time.monotonic_ns(),gdb.selected_thread().ptid[1],self.state,self.flag,after[0],after[1],after[3],after[4],after[2]),flush=True)
  return False
 def out_of_scope(self):
  print('SETTLEMENT_RETURN_OUT_OF_SCOPE mono_ns=%d tid=%d state=%#x'%(time.monotonic_ns(),self.tid,self.state),flush=True)
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
 start_ns=0
 last=0
 def stop(self):
  global wind_ready, force_released
  if not wind_ready: return False
  mem=gdb.selected_inferior(); state=int(gdb.parse_and_eval('$rdi')); config=struct.unpack('<Q',bytes(mem.read_memory(0x719f540,8)))[0]
  oldstate=struct.unpack('<I',bytes(mem.read_memory(state+0xd0,4)))[0]; newstate={4:7,5:9,6:11}[stage]; offset={4:0x158,5:0x160,6:0x168}[stage]
  clock=time.monotonic_ns()
  if self.started and clock-self.start_ns>=10000000000:
   print('LINE_FORCE_RELEASE mono_ns=%d state=%#x value=%d'%(clock,state,oldstate),flush=True);force_released=True;self.enabled=False;return False
  if not self.started:
   mem.write_memory(state+offset,struct.pack('<Q',clock));mem.write_memory(config+0x2d4,b'\x01');self.started=True;self.start_ns=clock
   print('FORCE_WIND_CLOCK monotonic_ns=%d wall_ns=%d'%(clock,time.time_ns()),flush=True)
  mem.write_memory(state+0xd0,struct.pack('<I',newstate))
  if clock-self.last>1000000000:
   index=struct.unpack('<I',bytes(mem.read_memory(state+0x210,4)))[0]
   print('FORCE_WIND_TICK state=%#x old=%d new=%d index=%d mono_ns=%d'%(state,oldstate,newstate,index,clock),flush=True);self.last=clock
  return False
class FixtureReturn(gdb.Breakpoint):
 def __init__(self): super().__init__('*0x2fc3f57',temporary=True,internal=True)
 def stop(self):
  print('LINE_FIXTURE_RESULT index=%d'%int(gdb.parse_and_eval('(int)$eax')),flush=True); self.enabled=False; return False
class LineFixture(gdb.Breakpoint):
 seen=0
 def stop(self):
  wanted=int(os.environ['RMUA_LINE_SELECTOR']);selector=int(gdb.parse_and_eval('$bpl'))
  if selector!=wanted:return False
  self.seen+=1
  if self.seen<=int(os.environ.get('RMUA_FIXTURE_ORDINAL','0')):return False
  mem=gdb.selected_inferior();p=int(gdb.parse_and_eval('$rsi')); old=bytes(mem.read_memory(p,48)); raw=struct.pack('<12f',0,0,0,1,0,0,350,0,1,1,1,0)
  mem.write_memory(p,raw);print('LINE_FIXTURE selector=%d component=%#x coordinate_source=fixed_world_origin old=%s new=%s'%(selector,int(gdb.parse_and_eval('$rdi')),old.hex(),raw.hex()),flush=True);FixtureReturn();self.enabled=False;return False
class LineEventAllocated(gdb.Breakpoint):
 def stop(self):
  tid=gdb.selected_thread().ptid[1]; source=pending_hits.pop(tid,None)
  if source:print('LINE_EVENT_ALLOC mono_ns=%d tid=%d actor=%#x component=%#x candidate_index=%d event=%#x'%(time.monotonic_ns(),tid,source[0],source[1],source[2],int(gdb.parse_and_eval('$rax'))),flush=True)
  return False
class PassiveState(gdb.Breakpoint):
 last=0
 def stop(self):
  global force_released
  now=time.monotonic_ns()
  if not force_released or now-self.last<200000000:return False
  state=int(gdb.parse_and_eval('$rdi'));value=struct.unpack('<I',bytes(gdb.selected_inferior().read_memory(state+0xd0,4)))[0]
  print('LINE_STATE_SAMPLE mono_ns=%d state=%#x value=%d'%(now,state,value),flush=True);self.last=now;return False
class StateTwelveWritten(gdb.Breakpoint):
 def stop(self):
  global force_released
  state=int(gdb.parse_and_eval('$rbx'));current,previous=struct.unpack('<II',bytes(gdb.selected_inferior().read_memory(state+0xd0,8)))
  print('LINE_STATE_TWELVE_WRITTEN mono_ns=%d tid=%d released=%d state=%#x current=%d previous=%d'%(time.monotonic_ns(),gdb.selected_thread().ptid[1],int(force_released),state,current,previous),flush=True);return False
LineEventAllocated('*0x2630797')
PassiveState('*0x2fb8f40')
StateTwelveWritten('*0x2fb9be0')
Inject('*0x2fbac30')
LineFixture('*0x2fc3f52')
ForceWindTick('*0x2fb8f40')
Trace(0x2fb23b0,'wind')
Trace(0x2fc77b0,'line')
Trace(0x2fc3fc0,'thick_place')
Trace(0x2fc4980,'thin_place')
Trace(0x2fc8900,'vehicle')
Trace(0x2630780,'event_source')
Trace(0x2fb7110,'collision_callback')
Trace(0x2fb9960,'state_change')
end
run
