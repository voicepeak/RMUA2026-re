#!/bin/bash
out=/root/main_patterns.txt
: > "$out"
for pat in RMUA2024OL RMUA2024 settings_Game2024 Game2024 OL1 OL2 OL12; do
  echo "==== $pat" >> "$out"
  grep -a -n -F -- "$pat" /root/main.strings | head -40 >> "$out"
  grep -a -n -F -- "$pat" /root/main.utf16.strings | head -40 >> "$out"
done
wc -l "$out"
cp "$out" /mnt/d/RMUA2026-re/artifacts/main_patterns.txt
