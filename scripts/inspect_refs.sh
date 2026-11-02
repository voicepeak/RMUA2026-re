#!/bin/bash
MAIN=/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping
for a in 0xb45bb7 0xb0380a 0xb798b0 0x75c4fe 0x9648ca; do
  echo "==== $a"
  grep -n -F -- "$a" /root/main.text.disasm | head -30
done
echo "==== context 0x2fb3b00"
objdump -d -M intel --start-address=0x2fb3a80 --stop-address=0x2fb3e80 "$MAIN" | head -200
echo "==== context 0x2fd6600"
objdump -d -M intel --start-address=0x2fd6500 --stop-address=0x2fd7300 "$MAIN" | head -260
