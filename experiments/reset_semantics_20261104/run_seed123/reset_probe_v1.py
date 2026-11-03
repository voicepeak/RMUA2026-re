"""GDB Python observer for libairsimros.so SHA-256 69d5ea33... only.

Installed on callback entry after the shared library loads. Breakpoints inspect
registers/memory and continue; they never write the inferior. Offsets correspond
to objdump of RESET_Callback at library vaddr 0x1851b0.
"""
import gdb
import time

def log(message):
    print('RESET_PROBE wall_unix_s=%.9f monotonic_s=%.9f %s' %
          (time.time(), time.monotonic(), message), flush=True)

class BranchProbe(gdb.Breakpoint):
    def __init__(self, address, kind):
        self.kind = kind
        super().__init__('*0x%x' % address, internal=True)
    def stop(self):
        try:
            if self.kind == 'inactive':
                flag=(int(gdb.parse_and_eval('$eax')) & 0xff) ^ 1
                log('branch=node_flag_not_1 node_flag=%d result=false' % flag)
            elif self.kind == 'pose':
                ptr=int(gdb.parse_and_eval('$r12'))
                vals=[float(gdb.parse_and_eval('*(float*)0x%x' % (ptr+i))) for i in range(0,28,4)]
                log('branch=pose_after_reset result_ptr=0x%x offsets_00_04_08_0c_10_14_18=%s' %
                    (ptr, ','.join('%.9g' % v for v in vals)))
        except Exception as exc:
            log('error=%s' % exc)
        return False

class ResetEntry(gdb.Breakpoint):
    def __init__(self):
        super().__init__('_ZN10airsim_ros13commands_impl14RESET_CallbackERNS_13ResetRequest_ISaIvEEERNS_14ResetResponse_IS2_EE', internal=True)
        self.installed=False
    def stop(self):
        if not self.installed:
            pc=int(gdb.parse_and_eval('$pc'))
            BranchProbe(pc+0x90, 'inactive')   # 0x185240
            BranchProbe(pc+0x17e, 'pose')     # 0x18532e, immediately after GETPOSE_UE
            self.installed=True
            log('callback_pc=0x%x offsets=0x90,0x17e' % pc)
        log('callback_enter')
        return False

class RngResetEntry(gdb.Breakpoint):
    def __init__(self):
        super().__init__('*0x2fced20', internal=True)
    def stop(self):
        try:
            ptr=int(gdb.parse_and_eval('$rdi'))
            arg=int(gdb.parse_and_eval('$esi')) & 0xffffffff
            values=[int(gdb.parse_and_eval('*(unsigned int*)0x%x' % (ptr+i))) for i in (0x198,0x19c,0x1a0)]
            log('rng_helper_arg=%u configured=%u initial=%u current=%u' %
                (arg,*values))
        except Exception as exc:
            log('rng_helper_error=%s' % exc)
        return False

ResetEntry()
RngResetEntry()
