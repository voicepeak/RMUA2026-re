#!/bin/bash
source /mnt/d/RMUA2026-re/scripts/env.sh
rm -rf /root/ghidra_proj
mkdir -p /root/ghidra_proj
"$GHIDRA_INSTALL_DIR/support/analyzeHeadless" /root/ghidra_proj RMUA \
  -import "$MAIN" -noanalysis -deleteProject \
  -scriptPath /mnt/d/RMUA2026-re/scripts \
  -postScript DumpAnchors.java \
  -log /root/dump_anchors.log
echo exit=$?
ls -l /root/anchors_pseudo.c 2>/dev/null
tail -20 /root/dump_anchors.log
