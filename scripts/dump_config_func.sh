#!/bin/bash
MAIN=/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping
objdump -d -M intel --start-address=0x2fb3900 --stop-address=0x2fb5600 "$MAIN" > /root/config_func.disasm
wc -l /root/config_func.disasm
grep -n -E 'movabs|movaps.*#|mov.*# 6|mov.*# 7|lea.*# 6|lea.*# 7|call' /root/config_func.disasm | head -200
cp /root/config_func.disasm /mnt/d/RMUA2026-re/artifacts/config_func.disasm
