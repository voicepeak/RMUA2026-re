#!/usr/bin/env python3
"""Posthoc point-in-local-hull checks for a candidate vehicle child component."""
import bisect
import hashlib
import json
from pathlib import Path
import re
import sys
import numpy as np
from scipy.spatial import ConvexHull
from scipy.spatial.transform import Rotation

root = Path(sys.argv[1])
manifest = json.loads((root/'manifest.json').read_text())
mesh_path = Path('artifacts/maps/obstacle_collision_meshes.json')
mesh = json.loads(mesh_path.read_text())['HoverTruck']['convex_elements'][0]
# This experiment only supports the explicitly recorded identity element transform.
parts = mesh['element_transform_raw']['Value']
parsed = {item['Name']: item['Value'][0]['Value'] for item in parts}
if ([float(parsed['Rotation'][k]) for k in 'XYZW'] != [0,0,0,1] or
        [float(parsed['Translation'][k]) for k in 'XYZ'] != [0,0,0] or
        [float(parsed['Scale3D'][k]) for k in 'XYZ'] != [1,1,1]):
    raise ValueError('unsupported convex element transform')
hull = ConvexHull(np.array(mesh['vertices_local'],dtype=float))
drone_re = re.compile(r'DRONE_WORLD mono=([\d.]+) actor=(0x[0-9a-f]+) component=(0x[0-9a-f]+) xyz_cm=([^\s]+) quat=([^\s]+)')
child_re = re.compile(r'VEHICLE_CHILD mono=([\d.]+) actor=(0x[0-9a-f]+) offset=0x240 component=(0x[0-9a-f]+) outer=(0x[0-9a-f]+) fname=([^\s]+) xyz_cm=([^\s]+) quat=([^\s]+) scale=([^\s]+)')
hit_re = re.compile(r'HIT_SOURCE mono=([\d.]+) drone=(0x[0-9a-f]+) drone_component=(0x[0-9a-f]+) drone_world_cm=([^\s]+) source_actor=(0x[0-9a-f]+) source_component=(0x[0-9a-f]+)')
vec = lambda v: [float(x) for x in v.split(',')]
result = {'mesh_sha256': hashlib.sha256(mesh_path.read_bytes()).hexdigest(),
          'mesh': 'HoverTruck convex element 0; element transform identity', 'runs': {},
          'limitations': ['candidate child is inferred from actor offset, ownership and matching transform, not independently typed',
                          'point-in-static-hull is not a runtime contact test; collision settings, actual shape and other transforms remain unverified',
                          'samples are nearest in debugger monotonic time with 0.03 s maximum gap; contacts between samples may be missed',
                          'all truth is posthoc; fixture path and forced state are diagnostic interventions']}
for run in manifest['runs']:
    name = 'seed%d_repeat%d' % (run['seed'],run['repeat'])
    log = (root/name/'gdb.log').read_text(errors='replace')
    ds = [{'t':float(m[1]),'actor':m[2],'component':m[3],'xyz':vec(m[4])} for m in drone_re.finditer(log)]
    cs = [{'t':float(m[1]),'actor':m[2],'component':m[3],'outer':m[4],
           'xyz':vec(m[6]),'quat':vec(m[7]),'scale':vec(m[8])} for m in child_re.finditer(log)]
    cs.sort(key=lambda x:x['t']); times=[c['t'] for c in cs]
    points=[]
    for d in ds:
        i=bisect.bisect_left(times,d['t']); choices=[cs[j] for j in (i-1,i) if 0<=j<len(cs)]
        if not choices: continue
        c=min(choices,key=lambda x:abs(x['t']-d['t'])); gap=abs(c['t']-d['t'])
        if gap>0.03 or any(abs(x)<1e-8 for x in c['scale']): continue
        local=Rotation.from_quat(c['quat']).inv().apply(np.array(d['xyz'])-np.array(c['xyz']))/np.array(c['scale'])
        signed=float(np.max(hull.equations[:,:3]@local+hull.equations[:,3]))
        points.append({'mono_s':d['t'],'time_gap_s':gap,'drone_actor':d['actor'],
                       'vehicle_actor':c['actor'],'vehicle_child':c['component'],
                       'local_point_cm':local.tolist(),'max_hull_plane_distance_cm':signed})
    hits=list(hit_re.finditer(log)); child_ids={c['component'] for c in cs}; actors={c['actor'] for c in cs}
    inside=[x for x in points if x['max_hull_plane_distance_cm']<=0]
    result['runs'][name]={'status':run.get('status'),'drone_samples':len(ds),'child_samples':len(cs),
                          'matched_within_0_03s':len(points),'inside_samples':len(inside),
                          'inside_examples':inside[:5], 'most_inside_sample':min(points,key=lambda x:x['max_hull_plane_distance_cm']) if points else None,
                          'hit_count':len(hits),'hits_matching_actor_or_child':sum(m[5] in actors or m[6] in child_ids for m in hits)}
(root/'mesh_proximity_analysis.json').write_text(json.dumps(result,indent=2)+'\n')
print(json.dumps(result,indent=2))
