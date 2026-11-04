#!/usr/bin/env python3
"""Compare raw generated wind arrays across the two observed injected resets."""
import hashlib,json,re
from pathlib import Path
import argparse
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args()
m=json.loads((a.directory/'manifest.json').read_text()); result={}
for r in m['runs']:
 key='seed%d_repeat%d'%(r['seed'],r['repeat']);text=(a.directory/key/'gdb.log').read_text(errors='replace'); arrays=[]
 for x in re.finditer(r'WIND_ITEM\s+(\d+) hex=([0-9a-f]{32})',text):
  if int(x[1])==0: arrays.append([])
  if arrays: arrays[-1].append(x[2])
 hashes=[hashlib.sha256(bytes.fromhex(''.join(a))).hexdigest() for a in arrays]
 result[key]={'reset_response_success':r.get('reset_response_success'),'injection_count':text.count('INJECT state='),'array_lengths':[len(a) for a in arrays],'array_sha256':hashes,'first_two_equal':len(hashes)>1 and hashes[0]==hashes[1],'note':'The third dump repeats the second due to a lingering breakpoint; it is not a third generation.'}
(a.directory/'analysis.json').write_text(json.dumps(result,indent=2)+'\n');print(json.dumps(result,indent=2))
