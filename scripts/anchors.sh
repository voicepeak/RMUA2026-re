#!/bin/bash
out=/root/anchors.txt
: > "$out"
for pat in IgnoreAllHitCollision IgnoreOverTime GameConfig seed Seed RenderOffscreen RouteGate RouteSpline RoutePort RouteJunction RouteSegment GameState2024 ARMUAGameModeBase AirSimROSGameType AirSimROSTimeChecker reset Reset Timeout Collision rotor_pwm_cmd vel_body_cmd takeoff land wind Wind; do
  echo "==== $pat" >> "$out"
  grep -a -n -i -- "$pat" /root/main.strings | head -25 >> "$out"
  grep -a -n -i -- "$pat" /root/main.utf16.strings | head -25 >> "$out"
done
wc -l "$out"
cp "$out" /mnt/d/RMUA2026-re/artifacts/anchors.txt
