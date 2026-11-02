import sys
from pathlib import Path

path = Path('/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping')
data = path.read_bytes()
keys = [
    'IgnoreAllHitCollision',
    'IgnoreOverTime',
    'GameConfig.json',
    'Configs/GameConfig.json',
    'RouteGateConfigs',
    'RouteGateIndex',
    'RouteGateConfig',
    'RMUA2024OL',
    'settings_Game2024_1',
    'seed',
]
for key in keys:
    for enc in ('utf-16-le', 'utf-8'):
        needle = key.encode(enc)
        start = 0
        hits = []
        while True:
            i = data.find(needle, start)
            if i < 0:
                break
            hits.append(i)
            start = i + 1
            if len(hits) >= 10:
                break
        print(key, enc, [hex(h) for h in hits])
