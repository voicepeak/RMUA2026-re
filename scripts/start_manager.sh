#!/bin/bash
source /mnt/d/RMUA2026-re/scripts/env.sh
cd /root
if pgrep -f rea_manager.mjs > /dev/null; then
  echo "manager already running"
  exit 0
fi
nohup node /mnt/d/RMUA2026-re/scripts/rea_manager.mjs > /root/rea_manager.log 2>&1 &
disown
sleep 3
pgrep -af rea_manager.mjs || true
tail -20 /root/rea_manager.log
