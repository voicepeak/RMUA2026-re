#!/usr/bin/env python3
"""Convert already exported static spline 1 to beta geometry and NED guides."""
import argparse,hashlib,json,math
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--input',type=Path,default=Path('experiments/stage_probe_20261104/static_spline_candidates.json'));p.add_argument('--output',type=Path,required=True);p.add_argument('--route',choices=('spline1','candidate_1_3'),default='spline1');p.add_argument('--connector-lift-m',type=float,default=0.);a=p.parse_args()
if not 0<=a.connector_lift_m<=8 or (a.connector_lift_m and a.route!='candidate_1_3'):p.error('lift requires candidate_1_3 and must be between 0 and 8 metres')
raw=a.input.read_bytes();data=json.loads(raw);route=next(x for x in data['routes'] if x['index']==1);pts=list(route['points_ned_m']);connector_m=0.
if a.route=='candidate_1_3':
 target=list(reversed(next(x for x in data['routes'] if x['index']==3)['points_ned_m']))
 start,end=pts[-1],target[0];connector_m=math.dist(start,end);steps=math.ceil(connector_m/5.)
 pts += [[start[j]+(end[j]-start[j])*i/steps for j in range(3)] for i in range(1,steps)] + target
s=0.;stations=[0.]
for prev,cur in zip(pts,pts[1:]):
 s+=math.hypot(cur[0]-prev[0],cur[1]-prev[1]);stations.append(s)
if a.connector_lift_m:
 entry=len(route['points_ned_m'])-1;start_s=stations[entry];end_s=start_s+connector_m
 pts=[[pt[0],pt[1],pt[2]-a.connector_lift_m*min(1.,max(0.,(st-(start_s-80.))/80.),max(0.,((end_s+80.)-st)/80.))] for st,pt in zip(stations,pts)]
a.output.mkdir(parents=True,exist_ok=False)
(a.output/'route.json').write_text(json.dumps({'routes':{'static_spline1':[[x,y,-z] for x,y,z in pts]}},indent=2)+'\n')
(a.output/'guides.json').write_text(json.dumps({'altitude_guides':[{'s':s,'z':pt[2],'source':'static_cooked_map'} for s,pt in zip(stations,pts)]},indent=2)+'\n')
(a.output/'provenance.json').write_text(json.dumps({'source':str(a.input),'sha256':hashlib.sha256(raw).hexdigest(),'map':data['map'],'route':a.route,'straight_unverified_connector_m':connector_m,'candidate_lift_m':a.connector_lift_m,'lift_ramp_m':80 if a.connector_lift_m else 0,'station':'2D cumulative length, matching beta RouteGeometry','route_z':'-NED_z (UP); route geometry consumes x/y, not direct altitude','guide_z':'NED_z','count':len(pts),'length_xy_m':s,'limitations':data['limitations']+(['straight connector is an unverified candidate, guarded by online LiDAR; not a mapped free-space claim'] if connector_m else [])+(['lift is a hypothesis based on prior LiDAR returns, not evidence that overhead is free'] if a.connector_lift_m else [])},indent=2)+'\n')
