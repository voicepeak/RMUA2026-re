#!/bin/bash
MAIN=/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping
nm -DC --defined-only "$MAIN" > /root/main.symbols.demangled 2>/root/main.nm.err
wc -l /root/main.symbols.demangled
grep -i -E 'RMUA|GameConfig|RouteGate|RouteSpline|RoutePort|RouteJunction|RouteSegment|GameState|AirSimROS|Windmill|WindChange|HitWindmill' /root/main.symbols.demangled > /root/main.game_symbols.txt
wc -l /root/main.game_symbols.txt
head -250 /root/main.game_symbols.txt
