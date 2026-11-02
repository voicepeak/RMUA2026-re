#!/bin/bash
MAIN=/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping
for spec in "2fb7000 2fb7300" "2fb9100 2fb9600" "2fbfef0 2fc0200" "2fb8500 2fb8a00" "2fb7f80 2fb8350"; do
  set -- $spec
  echo "==== $1-$2"
  objdump -d -M intel --start-address=0x$1 --stop-address=0x$2 "$MAIN"
done > /root/flag_uses.disasm 2>&1
wc -l /root/flag_uses.disasm
cp /root/flag_uses.disasm /mnt/d/RMUA2026-re/artifacts/flag_uses.disasm
