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

def unwrap_value(v):
    if isinstance(v, list):
        if not v:
            return None
        v = v[0]
    if isinstance(v, dict):
        return v.get('Value')
    return v

def vec_of(p):
    if p is None:
        return None
    v = unwrap_value(p.get('Value'))
    if isinstance(v, dict):
        return {k: num(v.get(k)) for k in ('X', 'Y', 'Z') if k in v}
    return v

def rot_of(p):
    if p is None:
        return None
    v = unwrap_value(p.get('Value'))
    if isinstance(v, dict):
        return {k: num(v.get(k)) for k in ('Pitch', 'Yaw', 'Roll') if k in v}
    return v

def fields_of(prop):
    if prop is None:
        return {}
    v = prop.get('Value')
    if isinstance(v, list) and v and isinstance(v[0], dict):
        return {p.get('Name'): p for p in v}
    return {}

def items_of(prop):
    v = pval(prop)
    return v if isinstance(v, list) else []

def transform_of_actor(actor):
    p = props(actor)
    rv = pval(p.get('RootComponent'))
    if not rv:
        return None
    e = ex[rv - 1]
    rp = props(e)
    return {
        'root': e.get('ObjectName'),
        'location': vec_of(rp.get('RelativeLocation')),
        'rotation': rot_of(rp.get('RelativeRotation')),
        'scale': vec_of(rp.get('RelativeScale3D')),
    }

def spline_points(route_spline_value):
    if not route_spline_value:
        return None
    actor = ex[route_spline_value - 1]
    sp = pval(props(actor).get('Spline'))
    if not sp:
        return None
    comp = ex[sp - 1]
    cp = props(comp)
    curves = fields_of(cp.get('SplineCurves'))
    pos = fields_of(curves.get('Position'))
    pts = []
    for item in items_of(pos.get('Points')):
        f = fields_of(item)
        pts.append({
            'in': num(pval(f.get('InVal'))),
            'pos': vec_of(f.get('OutVal')),
            'arrive': vec_of(f.get('ArriveTangent')),
            'leave': vec_of(f.get('LeaveTangent')),
            'mode': pval(f.get('InterpMode')),
        })
    return {
        'actor': actor.get('ObjectName'),
        'component': comp.get('ObjectName'),
        'location': vec_of(cp.get('RelativeLocation')),
        'rotation': rot_of(cp.get('RelativeRotation')),
        'points': pts,
    }

def find_config():
    for e in ex:
        if 'GameConfig' in e.get('ObjectName', ''):
            return e
    return None

def summarize_config(e):
    out = {}
    p = props(e)
    for key in ['RouteConfigs', 'RouteGateConfigs', 'FactoryConfigs', 'CheckPortConfigs', 'SideTrackConfigs', 'ArrowConfigs']:
        items = []
        for item in items_of(p.get(key)):
            f = fields_of(item)
            row = {}
            for k, prop in f.items():
                v = pval(prop)
                if 'ObjectPropertyData' in prop.get('$type', ''):
                    row[k] = {'index': v, 'name': name_of(v)}
                else:
                    row[k] = num(v)
            items.append(row)
        out[key] = items
    for row in out.get('RouteConfigs', []):
        idx = row.get('RouteSpline', {})
        if isinstance(idx, dict):
            row['RouteSpline'] = idx
            row['spline'] = spline_points(idx.get('index'))
    for row in out.get('CheckPortConfigs', []):
        idx = row.get('CheckPort', {})
        if isinstance(idx, dict) and idx.get('index'):
            row['transform'] = transform_of_actor(ex[idx['index'] - 1])
    return out

config = find_config()
summary = {
    'map': path.stem,
    'config_object': config.get('ObjectName') if config else None,
    'config': summarize_config(config) if config else None,
    'gates': {},
    'factories': {},
    'ports': {},
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
for port in ex:
    n = port.get('ObjectName', '')
    if n.endswith('_Port') or n == 'Port':
        summary['ports'][n] = transform_of_actor(port)
for e in ex:
    if e.get('ObjectName', '').startswith('PlayerStart'):
        summary['player_starts'].append({'name': e.get('ObjectName'), 'transform': transform_of_actor(e)})
out = Path(sys.argv[2])
out.write_text(json.dumps(summary, indent=2), encoding='utf-8')
print('wrote', out)
