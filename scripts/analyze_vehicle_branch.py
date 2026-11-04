#!/usr/bin/env python3
"""Reconstruct vehicle branch bucket from logged FRandomStream state."""
import argparse,json,re,struct
from pathlib import Path
import numpy as np
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();m=json.loads((a.directory/'manifest.json').read_text());out={}
for run in m['runs']:
 key='seed%d_repeat%d'%(run['seed'],run['repeat']);s=(a.directory/key/'gdb.log').read_text(errors='replace');rows=[]
 for state,bucket,direct in re.findall(r'VEHICLE_DECISION state=(\d+) bucket=(\d+) direct=(\d+)',s):
  n=int(state);u=np.float32(struct.unpack('<f',struct.pack('<I',(n>>9)|0x3f800000))[0]-1.0);expected=int(np.float32(u*np.float32(11)))
  rows.append({'state':n,'bucket':int(bucket),'direct':bool(int(direct)),'reconstructed_bucket':expected,'matches':expected==int(bucket) and (expected<6)==bool(int(direct))})
 out[key]=rows
(a.directory/'branch_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
