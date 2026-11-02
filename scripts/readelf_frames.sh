#!/bin/bash
MAIN=/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping
readelf --debug-dump=frames-interp "$MAIN" > /root/frames.txt 2>/dev/null
wc -l /root/frames.txt
grep -E 'pc=0000000002fa1|pc=0000000002fa2|pc=0000000002fa3|pc=0000000002fa8|pc=0000000002faa|pc=0000000002fb2|pc=0000000002fbf|pc=0000000002fb3|pc=0000000002fcc|pc=0000000002fd2' /root/frames.txt | head -80
