#!/bin/bash
log=/tmp/rea-ghidra-mGouHI/ghidra.log
ls -l "$log" 2>/dev/null
wc -l "$log" 2>/dev/null
grep -n -E 'ANALYZING|Analysis|ERROR|Exception|succeeded|completed|Timeout|OutOfMemory' "$log" 2>/dev/null | tail -40
