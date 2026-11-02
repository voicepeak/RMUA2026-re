#!/bin/bash
MAIN=/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping
for spec in "2fa1000 2fa1600" "2fcc700 2fccb00" "2fa8300 2fa8800" "2faa900 2faad00" "2fb2300 2fb2600" "2fbf100 2fbf500" "2fd2c00 2fd3600"; do
  set -- $spec
  echo "==== $1-$2"
  objdump -d -M intel --start-address=0x$1 --stop-address=0x$2 "$MAIN"
done > /root/seed_refs.disasm 2>&1
wc -l /root/seed_refs.disasm
cp /root/seed_refs.disasm /mnt/d/RMUA2026-re/artifacts/seed_refs.disasm
