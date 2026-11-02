#!/bin/bash
PID=$(pgrep -f AnalyzeHeadless | head -1)
echo "pid=$PID"
jstack "$PID" > /root/jstack.txt 2>&1
grep -n -A12 '"main"' /root/jstack.txt | head -40
grep -n -E 'at ghidra|at java' /root/jstack.txt | head -40
