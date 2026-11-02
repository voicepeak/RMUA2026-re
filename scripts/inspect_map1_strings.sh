#!/bin/bash
cd /mnt/d/RMUA2026-re/extracted/Game
strings -a -n 4 RMUA2024OL1.umap > /root/map1.umap.strings
strings -a -n 4 -el RMUA2024OL1.umap > /root/map1.umap.utf16
strings -a -n 4 RMUA2024OL1.uexp > /root/map1.uexp.strings
strings -a -n 4 -el RMUA2024OL1.uexp > /root/map1.uexp.utf16
wc -l /root/map1.umap.strings /root/map1.umap.utf16 /root/map1.uexp.strings /root/map1.uexp.utf16
out=/root/map1_patterns.txt
: > "$out"
for pat in RouteGate RouteSpline RoutePort RouteJunction RouteSegment GameState GameConfig Gate Factory Start End Goal Seed Timeout PlayerStart; do
  echo "==== $pat" >> "$out"
  grep -a -n -i -F -- "$pat" /root/map1.umap.strings | head -20 >> "$out"
  grep -a -n -i -F -- "$pat" /root/map1.umap.utf16 | head -20 >> "$out"
  grep -a -n -i -F -- "$pat" /root/map1.uexp.strings | head -20 >> "$out"
  grep -a -n -i -F -- "$pat" /root/map1.uexp.utf16 | head -20 >> "$out"
done
wc -l "$out"
cp "$out" /mnt/d/RMUA2026-re/artifacts/map1_patterns.txt
