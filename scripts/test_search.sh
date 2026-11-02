#!/bin/bash
set -u
source /mnt/d/RMUA2026-re/scripts/env.sh
cd /root
time rea search "$LIB" --pattern rotor_pwm --provider ghidra --snapshot /root/lib.snapshot --format json > /root/lib_search2.json 2>/root/lib_search2.err
echo exit=$?
wc -c /root/lib_search2.json /root/lib_search2.err
jq '.normalized_result' /root/lib_search2.json | head -c 2000
