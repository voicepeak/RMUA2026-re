#!/usr/bin/env python3
"""Verify that all map gate layouts are rigid transforms of the OL1 layout."""
import csv
import itertools
import json
import math
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
GATES = ROOT / 'artifacts/tables/gates.csv'
ROUTES = ROOT / 'artifacts/tables/routes.csv'


def load_gates():
    by_map = defaultdict(dict)
    with GATES.open() as stream:
        for row in csv.DictReader(stream):
            by_map[row['map']][row['name']] = {
                'xyz': (float(row['x']), float(row['y']), float(row['z'])),
                'yaw': float(row['yaw']),
                'scale': (float(row['scale_x']), float(row['scale_y']), float(row['scale_z'])),
            }
    return by_map


def rigid_fit(ref, target, names):
    a = [(ref[n]['xyz'][0], ref[n]['xyz'][1]) for n in names]
    b = [(target[n]['xyz'][0], target[n]['xyz'][1]) for n in names]
    ax = sum(p[0] for p in a) / len(a)
    ay = sum(p[1] for p in a) / len(a)
    bx = sum(p[0] for p in b) / len(b)
    by = sum(p[1] for p in b) / len(b)
    dot = 0.0
    cross = 0.0
    for (x, y), (u, v) in zip(a, b):
        px, py = x - ax, y - ay
        qu, qv = u - bx, v - by
        dot += px * qu + py * qv
        cross += px * qv - py * qu
    angle = math.atan2(cross, dot)
    ca, sa = math.cos(angle), math.sin(angle)
    tx = bx - (ca * ax - sa * ay)
    ty = by - (sa * ax + ca * ay)
    residuals = []
    for (x, y), (u, v) in zip(a, b):
        px = ca * x - sa * y + tx
        py = sa * x + ca * y + ty
        residuals.append(math.hypot(px - u, py - v))
    z_shift = [target[n]['xyz'][2] - ref[n]['xyz'][2] for n in names]
    return {
        'yaw_deg': math.degrees(angle),
        'translation_cm': [tx, ty],
        'residual_max_cm': max(residuals),
        'residual_mean_cm': sum(residuals) / len(residuals),
        'z_shift_mean_cm': sum(z_shift) / len(z_shift),
        'z_shift_std_cm': math.sqrt(sum((z - sum(z_shift) / len(z_shift)) ** 2 for z in z_shift) / len(z_shift)),
    }


def route_checks(by_map):
    route_points = defaultdict(list)
    with ROUTES.open() as stream:
        for row in csv.DictReader(stream):
            route_points[(row['map'], row['spline'])].append(
                (float(row['world_x']), float(row['world_y']), float(row['world_z'])))
    result = {}
    for map_name, gates in by_map.items():
        missing = []
        vertical = []
        for name, gate in gates.items():
            spline = name[:-5] if name.endswith('_Gate') else name
            points = route_points.get((map_name, spline))
            if not points:
                missing.append(name)
                continue
            gz = gate['xyz'][2]
            nearest = min(points, key=lambda p: math.hypot(p[0] - gate['xyz'][0], p[1] - gate['xyz'][1]))
            vertical.append((name, gz - nearest[2]))
        result[map_name] = {'missing_route': missing,
                            'gate_z_minus_nearest_route_z_min_cm': min(v for _, v in vertical) if vertical else None,
                            'gate_z_minus_nearest_route_z_max_cm': max(v for _, v in vertical) if vertical else None}
    return result


def main():
    by_map = load_gates()
    names = sorted(next(iter(by_map.values())))
    ref = by_map['RMUA2024OL1']
    pair_ref = [math.dist(ref[a]['xyz'], ref[b]['xyz']) for a, b in itertools.combinations(names, 2)]
    result = {
        'source': [str(GATES.relative_to(ROOT)), str(ROUTES.relative_to(ROOT))],
        'maps': len(by_map),
        'gates_per_map': {m: len(g) for m, g in by_map.items()},
        'names': names,
        'pairwise_distance_check': {},
        'rigid_fit': {},
        'scale_identical_to_ol1': {},
        'route_z_check': route_checks(by_map),
        'limitations': [
            'static gate transforms only; physical opening geometry and collision are not measured',
            'rigid fit assumes only yaw and translation; roll/pitch are zero in the static table',
            'a rigid copy does not prove identical environment assets, lighting or obstacles',
            'OL2 runtime BeginPlay comparison is imported separately and covers one map',
        ],
    }
    for map_name, gates in by_map.items():
        pairs = [math.dist(gates[a]['xyz'], gates[b]['xyz']) for a, b in itertools.combinations(names, 2)]
        result['pairwise_distance_check'][map_name] = {
            'max_abs_difference_cm': max(abs(a - b) for a, b in zip(pairs, pair_ref)),
            'identical_to_ol1_within_0p02_cm': all(abs(a - b) <= 0.02 for a, b in zip(pairs, pair_ref)),
        }
        result['rigid_fit'][map_name] = rigid_fit(ref, gates, names)
        result['scale_identical_to_ol1'][map_name] = all(
            gates[n]['scale'] == ref[n]['scale'] for n in names)
    runtime = ROOT / 'experiments/ol2_direct_gates_20261105/gate_comparison.json'
    if runtime.exists():
        data = json.loads(runtime.read_text())
        result['ol2_runtime_beginplay'] = {
            'source': str(runtime.relative_to(ROOT)),
            'matched_runs': sum(1 for run in data.get('runs', []) if run.get('matched') == 12),
            'total_runs': len(data.get('runs', [])),
            'tolerance_cm': data.get('tolerance_cm'),
        }
    out = ROOT / 'experiments/ol_gate_rigid_20261106'
    out.mkdir(exist_ok=True)
    (out / 'analysis.json').write_text(json.dumps(result, indent=2) + '\n')
    print(json.dumps(result, indent=2))


if __name__ == '__main__':
    main()
