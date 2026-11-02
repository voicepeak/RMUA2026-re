#!/bin/bash
MAIN=/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping
objdump -d -M intel --no-show-raw-insn "$MAIN" > /root/main.text.disasm
wc -l /root/main.text.disasm
for a in 75c4fe 75c50e b45bb7 b0380a b798b0 9648ca 9648ea; do
  echo "==== $a"
  grep -n -i "$a" /root/main.text.disasm | head -30
done
