import json
from pathlib import Path
d = json.loads(Path('/mnt/d/RMUA2026-re/artifacts/RMUA2024OL1.json').read_text())
ex = d['Exports']

def props(e):
    return {p.get('Name'): p for p in (e.get('Data') or [])}

def val(p):
    return p.get('Value') if p else None

a = ex[2693]
print('actor', a.get('ObjectName'))
print('actor props', [(p.get('Name'), p.get('Value')) for p in a.get('Data') or []])
rv = val(props(a).get('RootComponent'))
print('root idx', rv)
r = ex[rv - 1]
print('root', r.get('ObjectName'))
print([(p.get('Name'), p.get('Value')) for p in r.get('Data') or []])
sp = val(props(a).get('Spline'))
print('spline idx', sp)
c = ex[sp - 1]
print('comp', c.get('ObjectName'))
print([(p.get('Name'), p.get('Value')) for p in c.get('Data') or []])
