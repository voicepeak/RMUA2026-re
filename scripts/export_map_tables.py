import csv
import json
import math
from pathlib import Path

base = Path('/mnt/d/RMUA2026-re/artifacts')
data = json.loads((base / 'maps_summary_all.json').read_text())
outdir = base / 'tables'
outdir.mkdir(exist_ok=True)

def yaw_world(loc, rot, p):
    if not loc or not p:
        return None
    yaw = (rot or {}).get('Yaw') or 0.0
    c, s = math.cos(math.radians(yaw)), math.sin(math.radians(yaw))
    x, y, z = p.get('X', 0.0), p.get('Y', 0.0), p.get('Z', 0.0)
    return (loc.get('X', 0.0) + c*x - s*y,
            loc.get('Y', 0.0) + s*x + c*y,
            loc.get('Z', 0.0) + z)

for kind in ('gates', 'factories', 'ports'):
    with (outdir / f'{kind}.csv').open('w', newline='', encoding='utf-8') as f:
        w = csv.writer(f)
        w.writerow(['map', 'name', 'x', 'y', 'z', 'yaw', 'scale_x', 'scale_y', 'scale_z'])
        for m, v in data.items():
            for name, tr in (v.get(kind) or {}).items():
                if not tr or not tr.get('location'):
                    continue
                loc, rot, sc = tr['location'], tr.get('rotation') or {}, tr.get('scale') or {}
                w.writerow([m, name, loc.get('X'), loc.get('Y'), loc.get('Z'), rot.get('Yaw'),
                            sc.get('X'), sc.get('Y'), sc.get('Z')])

with (outdir / 'routes.csv').open('w', newline='', encoding='utf-8') as f:
    w = csv.writer(f)
    w.writerow(['map', 'route_index', 'spline', 'point', 'local_x', 'local_y', 'local_z',
                'world_x', 'world_y', 'world_z', 'seg_interval'])
    for m, v in data.items():
        for r in (v.get('config') or {}).get('RouteConfigs') or []:
            sp = r.get('spline') or {}
            loc = sp.get('location'); rot = sp.get('rotation')
            for i, p in enumerate(sp.get('points') or []):
                wp = yaw_world(loc, rot, p.get('pos'))
                w.writerow([m, r.get('RouteIndex'), sp.get('actor'), i,
                            p.get('pos', {}).get('X'), p.get('pos', {}).get('Y'), p.get('pos', {}).get('Z'),
                            *(wp or ('', '', '')), r.get('SegmentInterval')])

with (outdir / 'check_ports.csv').open('w', newline='', encoding='utf-8') as f:
    w = csv.writer(f)
    w.writerow(['map', 'index', 'name', 'x', 'y', 'z', 'yaw'])
    for m, v in data.items():
        for r in (v.get('config') or {}).get('CheckPortConfigs') or []:
            tr = r.get('transform') or {}
            loc = tr.get('location'); rot = tr.get('rotation') or {}
            w.writerow([m, r.get('PortIndex'), (r.get('CheckPort') or {}).get('name'),
                        (loc or {}).get('X'), (loc or {}).get('Y'), (loc or {}).get('Z'), rot.get('Yaw')])
print('tables written', outdir)
