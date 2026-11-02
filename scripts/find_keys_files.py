import sys
from pathlib import Path

paths = [
    Path('/mnt/d/RMUA2026-re/extracted/Game/BP_GameConfig2024_1.uasset'),
    Path('/mnt/d/RMUA2026-re/extracted/Game/BP_GameConfig2024_1.uexp'),
    Path('/mnt/d/RMUA2026-re/extracted/Game/BP_Gate.uasset'),
    Path('/mnt/d/RMUA2026-re/extracted/Game/BP_Gate.uexp'),
    Path('/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Plugins/AirSimROS/Binaries/Linux/libairsimros.so'),
    Path('/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Content/Paks/RMUA-LinuxNoEditor.pak'),
]
keys = ['IgnoreAllHitCollision', 'IgnoreOverTime', 'GameConfig.json', 'RouteGateConfigs', 'RouteGateIndex', 'RouteGateConfig', 'RMUA2024OL']
for path in paths:
    print('FILE', path, path.stat().st_size)
    data = path.read_bytes()
    for key in keys:
        for enc in ('utf-16-le', 'utf-8'):
            needle = key.encode(enc)
            hits = []
            start = 0
            while True:
                i = data.find(needle, start)
                if i < 0:
                    break
                hits.append(i)
                start = i + 1
                if len(hits) >= 5:
                    break
            if hits:
                print(' ', key, enc, [hex(h) for h in hits])
