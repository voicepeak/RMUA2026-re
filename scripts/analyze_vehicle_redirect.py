#!/usr/bin/env python3
"""Summarize entry/return order; branch calls after return are not the injected call."""
import argparse,json,re
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args();m=json.loads((a.directory/'manifest.json').read_text());out={}
for run in m['runs']:
 key='seed%d_repeat%d'%(run['seed'],run['repeat']);log=a.directory/key/'gdb.log'
 if not log.exists():continue
 s=log.read_text(errors='replace');events=re.findall(r'VEHICLE_REDIRECT_RETURNED|VEHICLE_REDIRECT\b|TRACE vehicle_branch\b|TRACE vehicle\b',s)
 try: start=events.index('VEHICLE_REDIRECT');end=events.index('VEHICLE_REDIRECT_RETURNED',start);injected=events[start:end+1]
 except ValueError: injected=[]
 out[key]={'status':run['status'],'events':events,'injected_call_events':injected,'branch_before_return':'TRACE vehicle_branch' in injected,'target_before_return':'TRACE vehicle' in injected}
(a.directory/'analysis.json').write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
