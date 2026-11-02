#!/bin/bash
mkdir -p /mnt/d/RMUA2026-re/artifacts/maps_summary
for i in $(seq 1 12); do
  src="/mnt/d/RMUA2026-re/artifacts/maps_json/RMUA2024OL${i}.json"
  out="/mnt/d/RMUA2026-re/artifacts/maps_summary/RMUA2024OL${i}.json"
  echo "map $i"
  python3 /mnt/d/RMUA2026-re/scripts/extract_map_summary2.py "$src" "$out" || echo "failed $i"
done
echo done
ls -lh /mnt/d/RMUA2026-re/artifacts/maps_summary | tail -15
