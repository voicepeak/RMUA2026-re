set pagination off
set confirm off
set print thread-events off
set breakpoint pending on
python
import gdb, os, struct
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
class LineInstance(gdb.Breakpoint):
 def stop(self):
  mem=gdb.selected_inferior(); component=int(gdb.parse_and_eval('$rdi')); data=int(gdb.parse_and_eval('$rsi')); typ=int(gdb.parse_and_eval('$bpl')); raw=bytes(mem.read_memory(data,48))
  print('LINE_INSTANCE component=%#x selector=%d transform_hex=%s floats=%s'%(component,typ,raw.hex(),','.join('%.9g'%x for x in struct.unpack('<12f',raw))),flush=True);return False
class LineInstanceReturn(gdb.Breakpoint):
 def stop(self):
  print('LINE_INSTANCE_RESULT index=%d'%int(gdb.parse_and_eval('(int)$eax')),flush=True);return False
Inject('*0x2fbac30')
LineInstance('*0x2fc3f52')
LineInstanceReturn('*0x2fc3f57')
Trace(0x2fb23b0,'wind')
Trace(0x2fc77b0,'line')
Trace(0x2fc3fc0,'thick_place')
Trace(0x2fc4980,'thin_place')
Trace(0x2fc8900,'vehicle')
end
run
