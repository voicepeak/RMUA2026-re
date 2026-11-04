#!/usr/bin/env python3
"""Convert already exported static spline 1 to beta geometry and NED guides."""
import argparse,hashlib,json,math
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('--input',type=Path,default=Path('experiments/stage_probe_20261104/static_spline_candidates.json'));p.add_argument('--output',type=Path,required=True);a=p.parse_args()
raw=a.input.read_bytes();data=json.loads(raw);route=next(x for x in data['routes'] if x['index']==1);pts=route['points_ned_m'];s=0.;stations=[0.]
for prev,cur in zip(pts,pts[1:]):
 s+=math.hypot(cur[0]-prev[0],cur[1]-prev[1]);stations.append(s)
a.output.mkdir(parents=True,exist_ok=False)
(a.output/'route.json').write_text(json.dumps({'routes':{'static_spline1':[[x,y,-z] for x,y,z in pts]}},indent=2)+'\n')
(a.output/'guides.json').write_text(json.dumps({'altitude_guides':[{'s':s,'z':pt[2],'source':'static_cooked_map'} for s,pt in zip(stations,pts)]},indent=2)+'\n')
(a.output/'provenance.json').write_text(json.dumps({'source':str(a.input),'sha256':hashlib.sha256(raw).hexdigest(),'map':data['map'],'route_index':1,'station':'2D cumulative length, matching beta RouteGeometry','route_z':'-NED_z (UP); route geometry consumes x/y, not direct altitude','guide_z':'NED_z','count':len(pts),'length_xy_m':s,'limitations':data['limitations']},indent=2)+'\n')
