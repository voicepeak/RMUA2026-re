#!/bin/bash
MAIN=/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping
for spec in "2fa2400 2fa3200" "2fa3100 2fa3e00" "2fa3d00 2fa5000" "2fa8300 2fa8800" "2faa900 2faad00" "2fb2000 2fb2800" "2fbf000 2fbf600" "2fd2c00 2fd3700"; do
  set -- $spec
  echo "==== $1-$2"
  objdump -d -M intel --start-address=0x$1 --stop-address=0x$2 "$MAIN"
done > /root/route_init.disasm 2>&1
wc -l /root/route_init.disasm
cp /root/route_init.disasm /mnt/d/RMUA2026-re/artifacts/route_init.disasm
