#!/bin/bash
cd /mnt/d/RMUA2026-re/extracted/Game
for f in BP_GameConfig2024_1.uasset BP_GameConfig2024_1.uexp BP_Gate.uasset BP_Gate.uexp; do
  echo "==== $f"
  ls -l "$f"
  xxd -l 64 "$f"
  echo "-- strings ascii"
  strings -a -n 3 "$f" | head -80
  echo "-- strings utf16"
  strings -a -n 3 -el "$f" | head -40
done
echo "==== map uexp head"
xxd -l 64 RMUA2024OL1.uexp
echo "==== map tokens"
strings -a -n 4 RMUA2024OL1.uexp > /tmp/map1.strings
strings -a -n 4 -el RMUA2024OL1.uexp > /tmp/map1.utf16.strings
grep -i -E 'RouteGate|RouteSpline|RoutePort|RouteJunction|GameState|GameConfig|Gate|Factory|Start|End|Goal|Seed|Timeout' /tmp/map1.strings > /tmp/map1.tokens
grep -i -E 'RouteGate|RouteSpline|RoutePort|RouteJunction|GameState|GameConfig|Gate|Factory|Start|End|Goal|Seed|Timeout' /tmp/map1.utf16.strings >> /tmp/map1.tokens
wc -l /tmp/map1.strings /tmp/map1.utf16.strings /tmp/map1.tokens
head -150 /tmp/map1.tokens
cp /tmp/map1.tokens /mnt/d/RMUA2026-re/artifacts/map1.tokens.txt
