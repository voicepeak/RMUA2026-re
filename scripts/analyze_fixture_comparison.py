#!/usr/bin/env python3
"""Posthoc comparison of fixed fixture groups; not proof of contact identity."""
import argparse,csv,json
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directories',nargs='+',type=Path);p.add_argument('--output',type=Path,required=True);a=p.parse_args();out={}
for group in a.directories:
 manifest=json.loads((group/'manifest.json').read_text());runs={}
 for run in manifest['runs']:
  key='seed%d_repeat%d'%(run['seed'],run['repeat']);d=group/key
  with (d/'pose_gt_posthoc.csv').open() as f: rows=list(csv.DictReader(f))
  z=[float(r['field.pose.position.z']) for r in rows];text=(d/'gdb.log').read_text(errors='replace')
  cmds=[json.loads(x) for x in (d/'pwm_commands.jsonl').read_text().splitlines()];pwm=[json.loads(x) for x in (d/'pwm.jsonl').read_text().splitlines()]
  runs[key]={'z_min':min(z) if z else None,'z_max':max(z) if z else None,'pose_samples':len(z),'command_samples':len(cmds),'max_observed_pwm':max((max(x['pwm']) for x in pwm if 'pwm' in x),default=None),'fixture_writes':text.count('LINE_FIXTURE '),'collision_callbacks':text.count('TRACE collision_callback')}
 out[group.name]={'selector':manifest['line_fixture_selector'],'runs':runs}
out['limitations']=['Fixed fixture position is a scenario input; debug pose used only posthoc','Callback does not identify the contacted component','Repeated forced state and debugger pause change scheduling; comparison does not establish causality or collision channels']
a.output.write_text(json.dumps(out,indent=2)+'\n');print(json.dumps(out,indent=2))
