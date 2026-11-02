import json
import sys
from pathlib import Path

path = Path(sys.argv[1])
d = json.loads(path.read_text())
ex = d['Exports']
im = d.get('Imports', [])

def name_of(v):
    if v is None or v == 0:
        return None
    if v > 0:
        return ex[v - 1].get('ObjectName') if v - 1 < len(ex) else f'export?{v-1}'
    return im[-v - 1].get('ObjectName') if -v - 1 < len(im) else f'import?{-v-1}'

def props(e):
    return {p.get('Name'): p for p in (e.get('Data') or [])}

def pval(p):
    return None if p is None else p.get('Value')

def num(v):
    if isinstance(v, str):
        try:
            return float(v)
        except ValueError:
            return v
    return v

def vec_of(p):
    if p is None:
        return None
    v = p.get('Value')
    if isinstance(v, list) and v:
        v = v[0].get('Value') if isinstance(v[0], dict) else v[0]
    if isinstance(v, dict):
        return {k: num(v.get(k)) for k in ('X', 'Y', 'Z') if k in v}
    return v

def rot_of(p):
    if p is None:
        return None
    v = p.get('Value')
    if isinstance(v, list) and v:
        v = v[0].get('Value') if isinstance(v[0], dict) else v[0]
    if isinstance(v, dict):
        return {k: num(v.get(k)) for k in ('Pitch', 'Yaw', 'Roll') if k in v}
    return v

def transform_of_actor(actor):
    p = props(actor)
    root = name_of(pval(p.get('RootComponent')))
    if root is None:
        return None
    e = ex[pval(p.get('RootComponent')) - 1]
    rp = props(e)
    return {
        'root': root,
        'location': vec_of(rp.get('RelativeLocation')),
        'rotation': rot_of(rp.get('RelativeRotation')),
        'scale': vec_of(rp.get('RelativeScale3D')),
    }

def actor_named(name):
    for e in ex:
        if e.get('ObjectName') == name:
            return e
    return None

def find_config():
    for e in ex:
        n = e.get('ObjectName', '')
        if 'GameConfig' in n:
            return e
    return None

def array_items(p):
    if p is None:
        return []
    v = p.get('Value')
    return v if isinstance(v, list) else []

def struct_fields(item):
    return {p.get('Name'): p for p in (item.get('Value') or [])}

def summarize_config(e):
    out = {}
    p = props(e)
    for key in ['RouteConfigs', 'RouteGateConfigs', 'FactoryConfigs', 'CheckPortConfigs', 'SideTrackConfigs', 'ArrowConfigs']:
        items = []
        for item in array_items(p.get(key)):
            f = struct_fields(item)
            row = {}
            for k, prop in f.items():
                v = pval(prop)
                if isinstance(v, int) and prop.get('$type', '').endswith('ObjectPropertyData'):
                    row[k] = {'index': v, 'name': name_of(v)}
                else:
                    row[k] = num(v)
            items.append(row)
        out[key] = items
    return out

config = find_config()
summary = {
    'map': path.stem,
    'config_object': config.get('ObjectName') if config else None,
    'config': summarize_config(config) if config else None,
    'gates': {},
    'factories': {},
    'player_starts': [],
}
for gate in ex:
    n = gate.get('ObjectName', '')
    if n.endswith('_Gate') or n == 'Gate':
        summary['gates'][n] = transform_of_actor(gate)
for factory in ex:
    n = factory.get('ObjectName', '')
    if n.endswith('_Factory') or n == 'Factory':
        summary['factories'][n] = transform_of_actor(factory)
for e in ex:
    if e.get('ObjectName', '').startswith('PlayerStart'):
        summary['player_starts'].append({'name': e.get('ObjectName'), 'transform': transform_of_actor(e)})

print(json.dumps(summary, indent=2))
