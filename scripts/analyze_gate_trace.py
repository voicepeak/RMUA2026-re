#!/usr/bin/env python3
"""Associate GDB gate component transforms with an existing map table."""
import argparse
import csv
import json
import math
from pathlib import Path
import re

PATTERN=re.compile(r'RMUA_GATE actor=(\S+) fname_index=(\d+) fname_number=(\d+) root=(\S+) '
                   r'xyz_cm=([^ ]+) quat=([^ ]+) scale=([^\s]+)')

def wrap(value):return (value+180.)%360.-180.
def vector(value):return [float(x) for x in value.split(',')]
def yaw(q):
    x,y,z,w=q
    return math.degrees(math.atan2(2*(w*z+x*y),1-2*(y*y+z*z)))

def quaternion_angle(a,b):
    norm=math.sqrt(sum(x*x for x in a)*sum(x*x for x in b))
    if not norm:return None
    return math.degrees(2*math.acos(min(1.,abs(sum(x*y for x,y in zip(a,b))/norm))))

def main():
    parser=argparse.ArgumentParser(description=__doc__)
    parser.add_argument('trace_dir',type=Path)
    parser.add_argument('--table',type=Path,default=Path('artifacts/tables/gates.csv'))
    parser.add_argument('--map',default='RMUA2024OL1')
    parser.add_argument('--tolerance-cm',type=float,default=50.)
    parser.add_argument('--output',type=Path,required=True)
    args=parser.parse_args()
    static=[r for r in csv.DictReader(args.table.open()) if r['map']==args.map]
    manifest=json.loads((args.trace_dir/'manifest.json').read_text())
    output={'map':args.map,'tolerance_cm':args.tolerance_cm,'runs':[],
            'association':'nearest static position, one-to-one; unresolved FName IDs are not decoded',
            'limitations':['gate actor transforms at BeginPlay only; later motion not excluded',
                           'scale is not physical opening dimensions',
                           'nearest-position association can hide identity ambiguity beyond its search tolerance']}
    for run in manifest['runs']:
        path=args.trace_dir/('seed%d_repeat%d'%(run['seed'],run['repeat']))/'gdb.log'
        text=path.read_text(errors='replace') if path.exists() else ''
        found=[]
        for m in PATTERN.finditer(text):
            p=vector(m.group(5));q=vector(m.group(6));s=vector(m.group(7))
            found.append({'actor':m.group(1),'fname_index':int(m.group(2)),'fname_number':int(m.group(3)),
                          'position_cm':p,'quat_xyzw':q,'scale':s,'yaw_deg':yaw(q)})
        remaining={r['name']:r for r in static};matched=[]
        for item in found:
            distances=sorted((math.dist(item['position_cm'],[float(r[k]) for k in ('x','y','z')]),name)
                             for name,r in remaining.items())
            if not distances or distances[0][0] > args.tolerance_cm:
                item['match']=None;matched.append(item);continue
            dist,name=distances[0];row=remaining.pop(name)
            item.update(match=name,position_error_cm=dist,
                        yaw_error_deg=wrap(item['yaw_deg']-float(row['yaw'])),
                        scale_error=[item['scale'][i]-float(row[k]) for i,k in enumerate(('scale_x','scale_y','scale_z'))],
                        second_nearest_cm=distances[1][0] if len(distances)>1 else None)
            matched.append(item)
        output['runs'].append({'seed':run['seed'],'repeat':run['repeat'],'status':run['status'],
                               'captured':len(found),'matched':sum(x.get('match') is not None for x in matched),
                               'unmatched_static':sorted(remaining),'gates':matched})
    groups={}
    for r in output['runs']:
        if r['matched']==len(static) and r['captured']==len(static):
            groups[(r['seed'],r['repeat'])]={x['match']:x for x in r['gates']}
    baseline=groups.get((123,1))
    comparisons=[]
    if baseline:
        for key,others in sorted(groups.items()):
            if key==(123,1):continue
            for name,a in baseline.items():
                b=others[name]
                comparisons.append({'baseline_seed':123,'baseline_repeat':1,'seed':key[0],'repeat':key[1],
                                    'gate':name,'position_delta_cm':math.dist(a['position_cm'],b['position_cm']),
                                    'yaw_delta_deg':wrap(a['yaw_deg']-b['yaw_deg']),
                                    'orientation_delta_deg':quaternion_angle(a['quat_xyzw'],b['quat_xyzw']),
                                    'max_scale_delta':max(abs(x-y) for x,y in zip(a['scale'],b['scale'])),
                                    'fname_equal':(a['fname_index'],a['fname_number'])==(b['fname_index'],b['fname_number'])})
    output['comparisons']=comparisons
    args.output.write_text(json.dumps(output,indent=2)+'\n')
    for r in output['runs']:print('seed',r['seed'],'repeat',r['repeat'],'matched',r['matched'],'/',len(static),'captured',r['captured'],'status',r['status'])
    if comparisons:print('comparisons',len(comparisons),'max delta cm',max(x['position_delta_cm'] for x in comparisons),
                         'max yaw deg',max(abs(x['yaw_delta_deg']) for x in comparisons),
                         'max scale delta',max(x['max_scale_delta'] for x in comparisons),
                         'same FName indices',sum(x['fname_equal'] for x in comparisons))

if __name__=='__main__':main()
