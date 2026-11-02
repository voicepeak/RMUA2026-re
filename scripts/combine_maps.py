import json
from pathlib import Path

base = Path('/mnt/d/RMUA2026-re/artifacts/maps_summary')
all_maps = {}
for i in range(1, 13):
    p = base / f'RMUA2024OL{i}.json'
    all_maps[f'RMUA2024OL{i}'] = json.loads(p.read_text())

out = Path('/mnt/d/RMUA2026-re/artifacts/maps_summary_all.json')
out.write_text(json.dumps(all_maps, indent=2), encoding='utf-8')

for name, m in all_maps.items():
    cfg = m.get('config') or {}
    rc = cfg.get('RouteConfigs') or []
    gc = cfg.get('RouteGateConfigs') or []
    fc = cfg.get('FactoryConfigs') or []
    print(name, 'routes', len(rc), 'gates', len(m.get('gates') or {}), 'factories', len(m.get('factories') or {}),
          'route_splines', [r.get('RouteSpline', {}).get('name') for r in rc])
