#!/usr/bin/env python3
"""Compare submitted HISM fixture index to candidate hit-result word at +0x58."""
import argparse, collections, json, re, struct
from pathlib import Path
p=argparse.ArgumentParser(description=__doc__);p.add_argument('directory',type=Path);a=p.parse_args()
m=json.loads((a.directory/'manifest.json').read_text());out={}
for run in m['runs']:
 key='seed%d_repeat%d'%(run['seed'],run['repeat']);path=a.directory/key/'gdb.log'
 if not path.exists():continue
 s=path.read_text(errors='replace')
 fixtures=re.findall(r'LINE_FIXTURE selector=(\d+) component=(0x[0-9a-f]+)',s)
 indices=[int(x) for x in re.findall(r'LINE_FIXTURE_RESULT index=(-?\d+)',s)]
 hits=re.findall(r'HIT_RESULT component=(0x[0-9a-f]+) bytes=([0-9a-f]{272})',s)
 component=fixtures[0][1] if len(fixtures)==1 else None
 matching=[bytes.fromhex(raw) for c,raw in hits if c==component]
 values=collections.Counter(struct.unpack_from('<i',raw,0x58)[0] for raw in matching)
 other=collections.Counter(struct.unpack_from('<i',bytes.fromhex(raw),0x58)[0] for c,raw in hits if c!=component)
 out[key]={'fixtures':fixtures,'return_indices':indices,'total_hit_results':len(hits),
           'same_component_results':len(matching),'candidate_offset_hex':'0x58',
           'candidate_values':dict(values), 'other_component_values':dict(other), 'fixed_pwm_command_exit_code':run.get('fixed_pwm_command_exit_code'),
           'unique_return_index':len(indices)==1 and indices[0]>=0,
           'all_candidate_values_match':len(indices)==1 and indices[0]>=0 and len(matching)>0 and all(v==indices[0] for v in values),
           'limitations':['offset is a candidate inferred from controlled submissions, not independently verified type metadata',
                          'forced scene and GDB scheduling differ from natural progression']}
(a.directory/'hit_instance_analysis.json').write_text(json.dumps(out,indent=2)+'\n')
print(json.dumps(out,indent=2))
