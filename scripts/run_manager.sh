#!/bin/bash
source /mnt/d/RMUA2026-re/scripts/env.sh
cd /root
exec node /mnt/d/RMUA2026-re/scripts/rea_manager.mjs >> /root/rea_manager.log 2>&1
