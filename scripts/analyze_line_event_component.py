#!/usr/bin/env python3
"""Compare pre-dispatch event arguments with the submitted line HISM component."""
import argparse, collections, json, re
from pathlib import Path
p = argparse.ArgumentParser(description=__doc__)
p.add_argument('directory', type=Path)
a = p.parse_args()
m = json.loads((a.directory / 'manifest.json').read_text())
out = {}
for run in m['runs']:
    key = 'seed%d_repeat%d' % (run['seed'], run['repeat'])
    log = a.directory / key / 'gdb.log'
    if not log.exists():
        continue
    text = log.read_text(errors='replace')
    fixtures = re.findall(r'LINE_FIXTURE selector=(\d+) component=(0x[0-9a-f]+)', text)
    events = re.findall(r'TRACE event_source rdi=(0x[0-9a-f]+) rsi=(0x[0-9a-f]+) rdx=(0x[0-9a-f]+) rcx=(0x[0-9a-f]+)', text)
    component = fixtures[0][1] if len(fixtures) == 1 else None
    counts = collections.Counter(event[3] for event in events)
    out[key] = {'fixture_records': fixtures, 'event_count': len(events),
                'event_rcx_counts': dict(counts), 'same_component_events': counts.get(component, 0),
                'fixed_pwm_command_exit_code': run.get('fixed_pwm_command_exit_code'),
                'limitation': 'HISM component is shared by instances; pointer match does not identify which instance was hit.'}
(a.directory / 'event_component_analysis.json').write_text(json.dumps(out, indent=2) + '\n')
print(json.dumps(out, indent=2))
