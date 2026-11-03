set pagination off
set confirm off
set breakpoint pending on
set print thread-events off
break *0x2fc89a7
commands
silent
printf "RMUA_HOVER_ROLL spline=%p initial_seed=%u current_seed=%u\n", $r15, *(unsigned int*)($rax+0x19c), *(unsigned int*)($rax+0x1a0)
continue
end
break *0x2fbff60
commands
silent
printf "RMUA_HOVER_START actor=%p speed=%.6f point_count=%d\n", $rdi, $xmm0.v4_float[0], *(int*)($rsi+8)
continue
end
run
