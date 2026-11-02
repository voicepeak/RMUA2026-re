#!/bin/bash
MAIN=/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping
objdump -d -M intel --start-address=0x328b530 --stop-address=0x33b5b20 "$MAIN" > /root/game_region.disasm
wc -l /root/game_region.disasm
for a in 75c4fe b45bb7 b0380a b798b0 9648ca 9648ea; do
  echo "==== $a"
  grep -n -i "$a" /root/game_region.disasm | head -20
done
cp /root/game_region.disasm /mnt/d/RMUA2026-re/artifacts/game_region.disasm
