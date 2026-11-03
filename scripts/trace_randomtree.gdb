set pagination off
set confirm off
set breakpoint pending on
set print thread-events off
break *0x2fa3dd0
commands
silent
printf "RMUA_RANDOM_ENTRY state=%p seed_state=%u item_count=%d\n", $rdi, *(unsigned int*)($rdi+0x10), *(int*)($rdi+0x28)
continue
end
break *0x2fa41dd
commands
silent
printf "RMUA_TREE_CANDIDATE class=%p x_cm=%.4f y_cm=%.4f z_cm=%.4f\n", $r14, *(float*)$rdx, *(float*)($rdx+4), *(float*)($rdx+8)
continue
end
run
