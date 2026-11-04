#!/usr/bin/env python3
"""Check state at return from a synthetic callback; not a physical contact test."""
import argparse,json,re
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();m=json.loads((a.directory/'manifest.json').read_text());out={'ignore_collision':m['ignore_collision'],'runs':{},'limitations':['callback deliberately invoked; no geometric contact or force measured','state and ignore flag overwritten in process memory']}
for r in m['runs']:
 key='seed%d_repeat%d'%(r['seed'],r['repeat']);s=(a.directory/key/'gdb.log').read_text(errors='replace')
 inj=re.findall(r'COLLISION_INJECT state=(\d+) ignore=(\d+)',s);ret=re.findall(r'COLLISION_RETURN state=(\d+)',s)
 out['runs'][key]={'injections':inj,'return_states':[int(x) for x in ret],'status':r['status']}
(a.directory/'analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
