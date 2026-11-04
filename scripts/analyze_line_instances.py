#!/usr/bin/env python3
"""Summarize observed line component calls and transforms; type/physics need independent proof."""
import argparse,hashlib,json,re,struct
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();m=json.loads((a.directory/'manifest.json').read_text());out={}
for run in m['runs']:
 key='seed%d_repeat%d'%(run['seed'],run['repeat']);log=a.directory/key/'gdb.log'
 if not log.exists():continue
 s=log.read_text(errors='replace');rows=re.findall(r'LINE_INSTANCE component=(0x[0-9a-f]+) selector=(\d+) transform_hex=([0-9a-f]{96})',s);results=[int(x) for x in re.findall(r'LINE_INSTANCE_RESULT index=(-?\d+)',s)]
 counts={}
 for comp,sel,raw in rows:
  if sel not in counts:counts[sel]=0
  counts[sel]+=1
 seq=sorted(sel+':'+raw for _,sel,raw in rows)
 out[key]={'call_count':len(rows),'selector_counts':counts,'asset_component_counts':{'HISM_ThickLine':counts.get('1',0),'HISM_ThinLine':counts.get('0',0)},'return_count':len(results),'return_minmax':[min(results),max(results)] if results else None,'negative_returns':sum(x<0 for x in results),'transform_multiset_sha256':hashlib.sha256('\n'.join(seq).encode()).hexdigest(),'note':'Component mapping follows native constructor and selector; call transforms and nonnegative return values do not establish collision response or successful physical contact.'}
(a.directory/'instances_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
