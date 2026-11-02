import json
from pathlib import Path

d = json.loads(Path('/mnt/d/RMUA2026-re/artifacts/RMUA2024OL1.json').read_text())
ex = d['Exports']
im = d.get('Imports', [])
print('exports', len(ex), 'imports', len(im))
for idx in [3435, 3436, 3437, 3438, 3439, 3440, 2694, 2695, 2696, 2700, 2701, 5704, 5705, 5706]:
    e = ex[idx]
    print('E', idx, e.get('ObjectName'), 'class', e.get('ClassIndex'), 'outer', e.get('OuterIndex'), 'data', len(e.get('Data') or []))
for idx in [0, 1, 2, 1945, 1946, 1947, 1948, 1958, 1959, 2694, 2695]:
    if idx < len(im):
        e = im[idx]
        print('I', idx, e.get('ObjectName'), e.get('ClassPackage'), e.get('ClassName'))

# package index resolver
def resolve(v):
    if v == 0:
        return 'null'
    if v < 0:
        i = -v - 1
        return f'export[{i}]={ex[i].get("ObjectName")}' if 0 <= i < len(ex) else f'export?{i}'
    i = v - 1
    return f'import[{i}]={im[i].get("ObjectName")}' if 0 <= i < len(im) else f'import?{i}'

print('resolve samples', [(v, resolve(v)) for v in [3438, 2694, 1959, 5704, 1946]])
