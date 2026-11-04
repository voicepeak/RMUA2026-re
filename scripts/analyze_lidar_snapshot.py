#!/usr/bin/env python3
"""Validate captured PointCloud2 layout and summarize finite XYZ payload coordinates."""
import argparse,hashlib,json,math,struct
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();manifest=json.loads((a.directory/'manifest.json').read_text());out={}
for run in manifest['runs']:
 key='seed%d_repeat%d'%(run['seed'],run['repeat']);d=a.directory/key;meta_path=d/'lidar_metadata.json';data_path=d/'lidar_payload.bin'
 if not meta_path.exists() or not data_path.exists():out[key]={'error':'snapshot or metadata missing'};continue
 meta=json.loads(meta_path.read_text());data=data_path.read_bytes();fields={v['name']:v for v in meta['fields']}
 if not all(x in fields and fields[x]['datatype']==7 and fields[x]['count']==1 for x in 'xyz'):
  out[key]={'error':'unsupported XYZ fields','fields':meta['fields']};continue
 endian='>' if meta['is_bigendian'] else '<';axes={x:[] for x in 'xyz'};finite=0;invalid=0
 for row in range(meta['height']):
  for col in range(meta['width']):
   pos=row*meta['row_step']+col*meta['point_step']
   try:v=[struct.unpack_from(endian+'f',data,pos+fields[x]['offset'])[0] for x in 'xyz']
   except struct.error:invalid+=1;continue
   if not all(math.isfinite(x) for x in v):invalid+=1;continue
   finite+=1
   for axis,value in zip('xyz',v):axes[axis].append(value)
 out[key]={'payload_bytes':len(data),'sha256_matches':hashlib.sha256(data).hexdigest()==meta['payload_sha256'],'declared_points':meta['width']*meta['height'],'finite_points':finite,'invalid_points':invalid,'bounds':{x:[min(v),max(v)] if v else None for x,v in axes.items()},'frame_id':meta['frame_id'],'note':'One snapshot at the initial vehicle location, not a dynamic obstacle visibility or geometry equivalence test.'}
(a.directory/'lidar_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
