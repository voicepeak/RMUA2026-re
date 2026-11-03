"""GDB Python observer for libairsimros.so SHA-256 69d5ea33... only.

Installed on callback entry after the shared library loads. Breakpoints inspect
registers/memory and continue; they never write the inferior. Offsets correspond
to objdump of RESET_Callback at library vaddr 0x1851b0.
"""
import gdb
import time
import hashlib

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

class LibcSrandProbe(gdb.Breakpoint):
    def __init__(self, address):
        super().__init__('*0x%x' % address, internal=True)
    def stop(self):
        try:
            seed=int(gdb.parse_and_eval('$edi')) & 0xffffffff
            caller=int(gdb.parse_and_eval('*(unsigned long*)$rsp'))
            log('libc_srand seed=%u caller=0x%x' % (seed, caller))
        except Exception as exc:
            log('libc_srand_error=%s' % exc)
        return False

def install_libc_probe():
    # Address and digest of libc shipped in rmua/re-probe:20261104. Install
    # only after verifying the mapped file; do not guess on a different libc.
    pid=gdb.selected_inferior().pid
    for line in open('/proc/%d/maps' % pid):
        cols=line.split()
        if len(cols)>=6 and cols[2]=='00000000' and cols[-1].endswith('/libc-2.31.so'):
            path=cols[-1]
            with open(path,'rb') as f:
                digest=hashlib.sha256(f.read()).hexdigest()
            if digest != '53b909551f8e581120ddc2d24cfb5df76c52e3c2e2a5dac0d202b25638b3e902':
                raise RuntimeError('unexpected libc sha256='+digest)
            address=int(cols[0].split('-')[0],16)+0x475c0
            LibcSrandProbe(address)
            log('libc_srand_installed addr=0x%x sha256=%s' % (address,digest))
            return
    raise RuntimeError('matching libc mapping not found')

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
            try:
                install_libc_probe()
            except Exception as exc:
                log('libc_srand_install_error=%s' % exc)
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
