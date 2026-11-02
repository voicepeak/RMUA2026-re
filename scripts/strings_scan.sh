#!/bin/bash
source /mnt/d/RMUA2026-re/scripts/env.sh
cd /root
strings -a -n 4 "$MAIN" > /root/main.strings
strings -a -n 4 -el "$MAIN" > /root/main.utf16.strings
wc -l /root/main.strings /root/main.utf16.strings
for f in /root/main.strings /root/main.utf16.strings; do
  echo "=== $f ==="
  grep -i -E 'GameConfig|IgnoreAllHitCollision|IgnoreOverTime|seed|gate|collision|timeout|track|finish|score|penalty|RMUA|airsim_node|reset|wind|noise' "$f" > "$f.matches" || true
  wc -l "$f.matches"
done
readelf -S "$MAIN" > /root/main.sections.txt
readelf -d "$MAIN" > /root/main.dynamic.txt 2>&1
readelf -sW "$MAIN" > /root/main.symbols.txt 2>&1
wc -l /root/main.sections.txt /root/main.dynamic.txt /root/main.symbols.txt
