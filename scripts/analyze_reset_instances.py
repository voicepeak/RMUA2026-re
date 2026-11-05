#!/usr/bin/env python3
"""Compare line component submissions and wind arrays before/after ROS reset."""
import argparse,hashlib,json,re
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();m=json.loads((a.directory/'manifest.json').read_text());out={}
for r in m['runs']:
 key='seed%d_repeat%d'%(r['seed'],r['repeat']);path=a.directory/key/'gdb.log'
 if not path.exists():continue
 s=path.read_text(errors='replace');phases=s.split('INJECT state=')[1:];groups=[]
 for phase in phases:
  lines=re.findall(r'RESET_LINE_INSTANCE selector=(\d+) transform_hex=([0-9a-f]{96})',phase); canonical=sorted(selector+':'+raw for selector,raw in lines)
  winds=[]
  for index,raw in re.findall(r'WIND_ITEM\s+(\d+) hex=([0-9a-f]{32})',phase):
   if int(index)==0:winds.append([])
   if winds:winds[-1].append(raw)
  groups.append({'line_count':len(lines),'line_multiset_sha256':hashlib.sha256('\n'.join(canonical).encode()).hexdigest(),'wind_dumps':[{'count':len(w),'sha256':hashlib.sha256(bytes.fromhex(''.join(w))).hexdigest()} for w in winds]})
 out[key]={'reset_response_success':r.get('reset_response_success'),'experiment_scope':m.get('stage_coverage'),'stream_reset_before':[list(map(int,x)) for x in re.findall(r'STREAM_RESET_BEFORE initial=(\d+) current=(\d+)',s)],'game_reset_enter':[list(map(int,x)) for x in re.findall(r'GAME_RESET_ENTER initial=(\d+) current=(\d+)',s)],'generations':groups,'line_equal':len(groups)>=2 and groups[0]['line_multiset_sha256']==groups[1]['line_multiset_sha256'],'wind_equal':len(groups)>=2 and bool(groups[0]['wind_dumps']) and bool(groups[1]['wind_dumps']) and groups[0]['wind_dumps'][0]['sha256']==groups[1]['wind_dumps'][0]['sha256'],'limitations':['service success alone does not prove physical scene replay; see experiment_scope for interventions','line transform submission is not proof of actual physical scene replay','lingering return breakpoint may duplicate second wind dump']}
(a.directory/'reset_analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
