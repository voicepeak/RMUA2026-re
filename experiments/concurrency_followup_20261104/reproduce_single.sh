#!/bin/bash
# Run from repository root; output directory must not exist. Diagnostic idle scene.
set -u
OUT=${1:-/tmp/rmua-single-seed42}
python3 scripts/headless_pool.py start \
 --simulator /home/tianbot/RUMA-by-helinjun/main/simulator/simulator_12.0.0.5 \
 --settings /home/tianbot/RUMA-by-helinjun/main/configs/simulator_settings.json \
 --seeds 42 --port 11720 --output "$OUT" --display :1 --timeout 60 || exit 1
trap 'python3 scripts/headless_pool.py stop "$OUT/pool.json"' EXIT
python3 scripts/monitor_pool_liveness.py "$OUT/pool.json" --seconds 80 --interval 1 --output "$OUT/monitor.jsonl"
