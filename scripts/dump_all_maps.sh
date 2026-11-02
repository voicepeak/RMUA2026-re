#!/bin/bash
mkdir -p /mnt/d/RMUA2026-re/artifacts/maps_json
cd /root/uasset_test
dotnet build -c Release > /tmp/uasset_build.log 2>&1
for i in $(seq 1 12); do
  src="/mnt/d/RMUA2026-re/extracted/Maps/RMUA/Content/Maps/RMUA2024OL/RMUA2024OL${i}.umap"
  out="/mnt/d/RMUA2026-re/artifacts/maps_json/RMUA2024OL${i}.json"
  if [ -f "$out" ]; then
    echo "skip $i"
    continue
  fi
  echo "map $i"
  dotnet bin/Release/net8.0/uasset_test.dll "$src" "$out" >> /tmp/dump_maps.log 2>&1 || echo "failed $i"
done
echo done
ls -lh /mnt/d/RMUA2026-re/artifacts/maps_json | tail -20
