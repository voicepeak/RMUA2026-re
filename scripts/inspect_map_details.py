import json
from pathlib import Path

p = Path('/mnt/d/RMUA2026-re/artifacts/RMUA2024OL1.json')
d = json.loads(p.read_text())
exports = d['Exports']

def walk(prop, indent=0, max_items=40):
    pre = '  ' * indent
    if not isinstance(prop, dict):
        print(pre + repr(prop))
        return
    name = prop.get('Name')
    typ = prop.get('$type', '').split('.')[-1]
    print(f'{pre}{name} [{typ}]')
    val = prop.get('Value')
    if isinstance(val, list):
        if len(val) > max_items:
            print(pre + f'  ({len(val)} items)')
        for i, item in enumerate(val[:max_items]):
            print(pre + f'  -- item {i}')
            walk(item, indent + 2, max_items)
    elif isinstance(val, dict):
        walk(val, indent + 1, max_items)
    else:
        print(pre + f'  = {val}')

print('==== gate root component 3438')
walk(exports[3438])
print('==== gate static mesh 5704')
walk(exports[5704])

for idx in [1957]:
    e = exports[idx]
    print('==== config', e.get('ObjectName'))
    for prop in e.get('Data') or []:
        if prop.get('Name') in ['RouteConfigs', 'RouteGateConfigs', 'FactoryConfigs', 'CheckPortConfigs', 'SideTrackConfigs', 'ArrowConfigs']:
            print('####', prop.get('Name'), 'len', len(prop.get('Value') or []))
            vals = prop.get('Value') or []
            for i, item in enumerate(vals[:3]):
                print(f'  -- {prop.get("Name")}[{i}]')
                walk(item, 2)
