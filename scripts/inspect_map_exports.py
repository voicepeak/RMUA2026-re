import json
from pathlib import Path

p = Path('/mnt/d/RMUA2026-re/artifacts/RMUA2024OL1.json')
d = json.loads(p.read_text())
exports = d['Exports']

def summarize(prop, depth=0):
    if not isinstance(prop, dict):
        return prop
    name = prop.get('Name')
    typ = prop.get('$type', '').split('.')[-1]
    if 'Value' not in prop:
        return f'{name}:{typ}'
    val = prop['Value']
    if isinstance(val, list):
        if len(val) <= 8 and all(isinstance(x, dict) for x in val):
            return {name: [summarize(x, depth+1) for x in val]}
        return f'{name}:list[{len(val)}]'
    if isinstance(val, dict):
        return {name: summarize(val, depth+1)}
    return f'{name}={val}'

for idx in [1957, 1958, 1959, 1960, 1961, 1962, 1963, 1964, 1965, 1966, 1967, 1968, 1969]:
    e = exports[idx]
    print('====', idx, e.get('ObjectName'), 'class', e.get('ClassIndex'), 'size', e.get('SerialSize'))
    data = e.get('Data') or []
    for prop in data:
        print('  ', summarize(prop))
    print()

# find player starts and print first
for idx, e in enumerate(exports):
    if e.get('ObjectName') == 'PlayerStart':
        print('==== PlayerStart', idx)
        for prop in e.get('Data') or []:
            print('  ', summarize(prop))
        break
