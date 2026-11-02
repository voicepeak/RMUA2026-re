import json
from pathlib import Path

d = json.loads(Path('/mnt/d/RMUA2026-re/artifacts/RMUA2024OL1.json').read_text())
ex = d['Exports']; im = d.get('Imports', [])

def resolve(v):
    if v == 0:
        return None
    if v > 0:
        return ex[v-1].get('ObjectName') if v-1 < len(ex) else f'export?{v-1}'
    return im[-v-1].get('ObjectName') if -v-1 < len(im) else f'import?{-v-1}'

def get_props(e):
    return {p.get('Name'): p for p in (e.get('Data') or [])}

def val(p):
    if p is None:
        return None
    v = p.get('Value')
    return v

def dump_prop(p, indent=0):
    pre = '  ' * indent
    print(pre + str(p.get('Name')) + ' ' + str(p.get('$type','').split('.')[-1]))
    v = p.get('Value')
    if isinstance(v, list):
        if v and isinstance(v[0], dict):
            for x in v:
                dump_prop(x, indent+1)
        else:
            print(pre + '  ' + repr(v))
    elif isinstance(v, dict):
        dump_prop(v, indent+1)
    else:
        print(pre + '  ' + repr(v))

# gate 1_1 export
gate = next(e for e in ex if e.get('ObjectName') == '1_1_Gate')
print('gate data')
for p in gate.get('Data') or []:
    dump_prop(p)
root = val(get_props(gate).get('RootComponent'))
print('root value', root, resolve(root))
comp = ex[root-1]
print('root export', comp.get('ObjectName'), comp.get('OuterIndex'), resolve(comp.get('OuterIndex')))
for p in comp.get('Data') or []:
    dump_prop(p)
