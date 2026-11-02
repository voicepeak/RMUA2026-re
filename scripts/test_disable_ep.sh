#!/bin/bash
source /mnt/d/RMUA2026-re/scripts/env.sh
export REA_GHIDRA_SEED_FILE=/root/empty_seeds.tsv
cd /root
time rea analyze "$LIB" --provider ghidra --format json > /root/test_disable.json 2>/root/test_disable.err
echo exit=$?
wc -c /root/test_disable.json /root/test_disable.err
head -c 600 /root/test_disable.json
echo
grep -i -E 'REA test|seed|error|timeout' /root/test_disable.err | head -20
