#!/bin/bash
PID=$(pgrep -f AnalyzeHeadless | head -1)
jstack "$PID" > /root/jstack2.txt 2>&1
grep -E '^"' /root/jstack2.txt | head -60
echo ==== RUNNABLE
grep -B1 -A8 'java.lang.Thread.State: RUNNABLE' /root/jstack2.txt | head -120
