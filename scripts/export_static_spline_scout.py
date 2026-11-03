#!/usr/bin/env python3
"""Sample exported UE spline geometry. Offline proposal, not a validated flight path."""
import argparse, json, math
from pathlib import Path

def point(v): return [float(v[k]) for k in ('X','Y','Z')]
def length(a,b): return math.dist(a,b)
def spline(spec):
    angle=math.radians(spec['rotation']['Yaw']);co,si=math.cos(angle),math.sin(angle)
    offset=point(spec['location']); samples=[]
    pts=spec['points']
    for p,q in zip(pts,pts[1:]):
        p0,p1,m0,m1=[point(a) for a in (p['pos'],q['pos'],p['leave'],q['arrive'])]
        for i in range(1001):
            t=i/1000.;h=(2*t**3-3*t*t+1,t**3-2*t*t+t,-2*t**3+3*t*t,t**3-t*t)
            x,y,z=[h[0]*p0[k]+h[1]*m0[k]+h[2]*p1[k]+h[3]*m1[k] for k in range(3)]
            samples.append([(co*x-si*y+offset[0])/100,(si*x+co*y+offset[1])/100,-(z+offset[2])/100])
    return samples

def sample(points,spacing):
    out=[points[0]];distance=0.
    for i in range(1,len(points)):
        distance+=length(points[i-1],points[i])
        if distance>=spacing:out.append(points[i]);distance=0.
    if out[-1]!=points[-1]:out.append(points[-1])
    return out

def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--map-json',type=Path,default=Path('artifacts/maps/maps_summary_all.json'))
    p.add_argument('--map',default='RMUA2024OL1');p.add_argument('--spacing-m',type=float,default=10.)
    p.add_argument('--output',type=Path,required=True);a=p.parse_args()
    configs=json.loads(a.map_json.read_text())[a.map]['config']['RouteConfigs']
    rows=[]
    for item in configs:
        spec=item['spline'];points=sample(spline(spec),a.spacing_m)
        rows.append({'index':item['RouteIndex'],'actor':spec['actor'],'points_ned_m':points})
    gaps=[]
    for left,right in zip(rows,rows[1:]):
        gaps.append({'from':left['actor'],'to':right['actor'],'end_to_begin_m':length(left['points_ned_m'][-1],right['points_ned_m'][0]),
                     'end_to_end_m':length(left['points_ned_m'][-1],right['points_ned_m'][-1])})
    a.output.write_text(json.dumps({'map':a.map,'basis':'static export; yaw-only transform; UE cm to NED m',
       'limitations':['not validated collision-free','no terrain or stage-trigger knowledge','gaps need an independently verified route','component hierarchy/rotation conversion not established in flight'],
       'routes':rows,'adjacent_route_gaps':gaps},indent=2)+'\n')
if __name__=='__main__':main()
