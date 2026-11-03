#!/usr/bin/env python3
"""Export cooked mesh convex collision data, without applying instance transforms."""
import argparse
from collections import Counter
import hashlib
import json
from pathlib import Path


def export(path):
    data = path.read_bytes()
    asset = json.loads(data)
    body = next(x for x in asset['Exports'] if x.get('ObjectName') == 'BodySetup_0')
    geom = next(x for x in body['Data'] if x['Name'] == 'AggGeom')
    out = []
    for array in geom['Value']:
        if array['Name'] != 'ConvexElems':
            raise ValueError('unhandled collision shape ' + array['Name'])
        for item in array['Value']:
            fields = {x['Name']:x for x in item['Value']}
            verts = []
            for v in fields['VertexData']['Value']:
                p = v['Value'][0]['Value']
                verts.append([float(p[k]) for k in ('X','Y','Z')])
            indices = [int(v['Value']) for v in fields['IndexData']['Value']]
            if not verts or len(indices)%3 or any(i < 0 or i >= len(verts) for i in indices):
                raise ValueError('invalid collision vertices or triangles')
            triangles = [indices[i:i+3] for i in range(0,len(indices),3)]
            if any(len(set(t)) != 3 for t in triangles):
                raise ValueError('triangle contains duplicate indices')
            edges = Counter(tuple(sorted((t[i],t[(i+1)%3]))) for t in triangles for i in range(3))
            out.append({'vertices_local':verts, 'triangle_indices':indices,
                        'bounds_min_local':[min(p[i] for p in verts) for i in range(3)],
                        'bounds_max_local':[max(p[i] for p in verts) for i in range(3)],
                        'edge_incidence_counts':{str(n):sum(v==n for v in edges.values()) for n in sorted(set(edges.values()))},
                        'element_transform_raw':fields.get('Transform'),
                        'element_box_raw':fields.get('ElemBox')})
    return {'source_json':str(path), 'source_json_sha256':hashlib.sha256(data).hexdigest(),
            'coordinate_scope':'local mesh; element/component/actor transforms not applied',
            'convex_elements':out}


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('assets', nargs='+', type=Path)
    p.add_argument('--output', type=Path, required=True)
    args=p.parse_args()
    data={path.stem.removeprefix('mesh_'):export(path) for path in args.assets}
    args.output.write_text(json.dumps(data,indent=2)+'\n')
    for name,value in data.items():
        print(name,[(len(e['vertices_local']),len(e['triangle_indices'])//3,
                     e['bounds_min_local'],e['bounds_max_local']) for e in value['convex_elements']])


if __name__ == '__main__': main()
