#!/bin/bash
LIB=/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Plugins/AirSimROS/Binaries/Linux/libairsimros.so
file "$LIB"
readelf -S "$LIB" > /root/lib.sections.txt
readelf --string-dump=.gnu_debuglink "$LIB" > /root/lib.debuglink.txt 2>&1
nm -DC --defined-only "$LIB" > /root/lib.symbols.demangled 2>/root/lib.nm.err
wc -l /root/lib.symbols.demangled
grep -i -E 'AirSimROS|GameStatus|reset|takeoff|PWM|pwm|Velocity|vel_|Pose|Gate|Status|Ports|Circle|Tree' /root/lib.symbols.demangled > /root/lib.key_symbols.txt
wc -l /root/lib.key_symbols.txt
head -160 /root/lib.key_symbols.txt
