set pagination off
set confirm off
set breakpoint pending on
set print thread-events off
set $gate_count = 0
set logging overwrite on
break *0x2fcc89a
commands
silent
printf "RMUA_RANDOM_INIT singleton=%p configured=%u initial=%u current=%u\n", $r12, *(unsigned int*)($r12+0x198), *(unsigned int*)($r12+0x19c), *(unsigned int*)($r12+0x1a0)
continue
end
break *0x2fba970
commands
silent
set $singleton = *(unsigned long*)0x71a1c18
if $singleton != 0
printf "RMUA_GAME_RESET state=%p singleton=%p initial=%u current=%u\n", $rdi, $singleton, *(unsigned int*)($singleton+0x19c), *(unsigned int*)($singleton+0x1a0)
else
printf "RMUA_GAME_RESET state=%p singleton=NULL\n", $rdi
end
continue
end
break *0x2fc1360
commands
silent
set $gate_count = $gate_count + 1
set $actor = $rdi
set $root = *(unsigned long*)($actor+0x130)
if $root != 0
printf "RMUA_GATE actor=%p fname_index=%u fname_number=%u root=%p xyz_cm=%.6f,%.6f,%.6f quat=%.8f,%.8f,%.8f,%.8f scale=%.6f,%.6f,%.6f\n", $actor, *(unsigned int*)($actor+0x18), *(unsigned int*)($actor+0x1c), $root, *(float*)($root+0x1d0), *(float*)($root+0x1d4), *(float*)($root+0x1d8), *(float*)($root+0x1c0), *(float*)($root+0x1c4), *(float*)($root+0x1c8), *(float*)($root+0x1cc), *(float*)($root+0x1e0), *(float*)($root+0x1e4), *(float*)($root+0x1e8)
else
printf "RMUA_GATE actor=%p root=NULL\n", $actor
end
continue
end
break *0x2fb2040
commands
silent
printf "RMUA_SRAND context=%p seed=%u\n", $rdi, $esi
continue
end
break *0x2fb23b0
commands
silent
printf "RMUA_WIND_GENERATE context=%p xmm0=%.8f xmm1=%.8f xmm2=%.8f xmm3=%.8f\n", $rdi, $xmm0.v4_float[0], $xmm1.v4_float[0], $xmm2.v4_float[0], $xmm3.v4_float[0]
continue
end
break *0x2fc77b0
commands
silent
printf "RMUA_LINE object=%p rsi=%p rdx=%p rcx=%p r8=%p r9=%p xmm0=%.8f xmm1=%.8f xmm2=%.8f xmm3=%.8f\n", $rdi, $rsi, $rdx, $rcx, $r8, $r9, $xmm0.v4_float[0], $xmm1.v4_float[0], $xmm2.v4_float[0], $xmm3.v4_float[0]
continue
end
break *0x2fc8900
commands
silent
printf "RMUA_VEHICLE_BRANCH object=%p xmm0=%.8f\n", $rdi, $xmm0.v4_float[0]
continue
end
break *0x6f8bd80
commands
silent
if $gate_count >= 12
printf "RMUA_POST_GATE_THROW exception=%p typeinfo=%p name=", $rdi, $rsi
x/s *(char**)($rsi+8)
bt 18
disable $bpnum
end
continue
end
run
