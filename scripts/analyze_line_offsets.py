#!/usr/bin/env python3
"""Validate sampled 0x2fc77b0 offsets from observed FRandomStream states."""
import argparse,json,re,struct
from pathlib import Path
import numpy as np
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();m=json.loads((a.directory/'manifest.json').read_text());out={}
for run in m['runs']:
 key='seed%d_repeat%d'%(run['seed'],run['repeat']);s=(a.directory/key/'gdb.log').read_text(errors='replace')
 rows=re.findall(r'LINE_OFFSET state=(\d+) base_cm=([\d.eE+-]+) offset_cm=([\d.eE+-]+) reverse=(\d+)',s);offsets=[];errs=[];bases=[]
 for st,base,off,rev in rows:
  state=int(st);bits=(state>>9)|0x3f800000;u=np.float32(struct.unpack('<f',struct.pack('<I',bits))[0]-1.0)
  expected=np.float32(np.float32(u*np.float32(7000))+np.float32(1500));expected=-expected if int(rev) else expected
  offsets.append(float(off));bases.append(float(base));errs.append(abs(float(off)-float(expected)))
 out[key]={'samples':len(rows),'offset_abs_minmax_cm':[min(map(abs,offsets)),max(map(abs,offsets))] if rows else None,'base_values_cm':sorted(set(bases)),'max_reconstruction_error_cm':max(errs) if rows else None,'limitation':'Candidate distance offsets at one internal call site, not final successful line positions or inter-obstacle spacing.'}
(a.directory/'offset_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
