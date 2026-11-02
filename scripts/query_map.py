import json
from pathlib import Path

p = Path('/mnt/d/RMUA2026-re/artifacts/RMUA2024OL1.json')
d = json.loads(p.read_text())
print('keys', list(d.keys()))
print('exports', len(d.get('Exports', [])), 'names', len(d.get('NameMap', [])))
for i, e in enumerate(d.get('Exports', [])):
    name = e.get('ObjectName', '')
    if any(k in name for k in ['Gate', 'Factory', 'GameConfig', 'Route', 'Spline', 'Start', 'Goal']):
        print(i, name, e.get('ClassIndex'), e.get('SerialSize'), e.get('SerialOffset'))
