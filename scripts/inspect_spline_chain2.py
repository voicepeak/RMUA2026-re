import json
from pathlib import Path
d = json.loads(Path('/mnt/d/RMUA2026-re/artifacts/RMUA2024OL1.json').read_text())
ex = d['Exports']

def props(e):
    return {p.get('Name'): p for p in (e.get('Data') or [])}

def val(p):
    return p.get('Value') if p else None

a = ex[2693]
rv = val(props(a).get('RootComponent'))
r = ex[rv - 1]
rp = props(r)
print('actor', a.get('ObjectName'), 'root idx', rv, 'root', r.get('ObjectName'))
for k in ('RelativeLocation', 'RelativeRotation', 'RelativeScale3D'):
    print(' root', k, rp.get(k).get('Value') if rp.get(k) else None)
sp = val(props(a).get('Spline'))
c = ex[sp - 1]
cp = props(c)
print('spline idx', sp, 'comp', c.get('ObjectName'))
for k in ('RelativeLocation', 'RelativeRotation', 'RelativeScale3D'):
    print(' comp', k, cp.get(k).get('Value') if cp.get(k) else None)
