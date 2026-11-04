#!/usr/bin/env python3
"""Compare internal candidate point multisets, ignoring process-specific actor addresses."""
import argparse,hashlib,json,re,struct
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();m=json.loads((a.directory/'manifest.json').read_text())
out={'runs':{},'repeats':{},'limitations':['Internal point arrays used for spacing checks, not verified spawned actor transforms','Actor addresses are process-specific; points are compared as a multiset']}
for run in m['runs']:
 key='seed%d_repeat%d'%(run['seed'],run['repeat']);log=a.directory/key/'gdb.log'
 if not log.exists():continue
 s=log.read_text(errors='replace');points=re.findall(r'OBSTACLE_POINT actor=0x[0-9a-f]+ index=\d+ hex=([0-9a-f]{24})',s); arrays=re.findall(r'OBSTACLE_ARRAY actor=0x[0-9a-f]+ ptr=0x[0-9a-f]+ count=(\d+) cap=(\d+)',s)
 out['runs'][key]={'points':len(points),'arrays':len(arrays),'declared_points':sum(int(x[0]) for x in arrays),'point_multiset_sha256':hashlib.sha256(bytes.fromhex(''.join(sorted(points)))).hexdigest(),'read_errors':s.count('OBSTACLE_READ_ERROR'),'status':run['status']}
for seed in m['seeds']:
 group=[v for k,v in out['runs'].items() if k.startswith('seed%d_'%seed)]
 out['repeats'][str(seed)]={'runs':len(group),'same_point_multiset':len(group)>1 and len({v['point_multiset_sha256'] for v in group})==1}
(a.directory/'analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
