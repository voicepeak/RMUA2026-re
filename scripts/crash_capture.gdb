# Parent debugger for a fresh, unmodified RMUA process. Only acts on SIGSEGV.
set pagination off
set confirm off
set print thread-events off
set disable-randomization off
handle SIGSEGV stop print nopass
run
printf "\nRMUA_GDB_STOP: signal or process exit; inspect reason above\n"
info program
info registers
printf "\nSTACK_POINTER_DUMP:\n"
x/16gx $rsp
if $pc != 0
  x/8i $pc-12
  bt 40
else
  printf "\nPC_IS_NULL: indirect call through address 0; backtrace unavailable\n"
end
info threads
thread apply all bt 12
printf "\nRMUA_26B6223_EFFECTIVE_ADDRESS_IF_THIS_IS_THE_FAULT_PC: "
p/x $r12+$r13+8
