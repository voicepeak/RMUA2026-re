#!/usr/bin/env python3
"""Link a vehicle-source callback to the state setter, text store and return."""
import json
import re
import subprocess
import sys
from pathlib import Path

p = Path(sys.argv[1])
for script in ('analyze_vehicle_parked.py', 'analyze_vehicle_contact_detail.py', 'analyze_vehicle_release_state.py'):
    subprocess.run([sys.executable, str(Path(__file__).with_name(script)), str(p)],
                   check=True, stdout=subprocess.DEVNULL)
release = json.loads((p / 'release_state_analysis.json').read_text())
manifest = json.loads((p / 'manifest.json').read_text())
result = {}
for run in manifest['runs']:
    name = f"seed{run['seed']}_repeat{run['repeat']}"
    log = (p / name / 'gdb.log').read_text(errors='replace')
    records = []
    for m in re.finditer(r'^SETTLEMENT_(ENTRY|RETURN|TEXT_POINTER_WRITTEN) (.*)$', log, re.M):
        x = dict(re.findall(r'(\w+)=(\S*)', m[2]))
        x.update(kind=m[1], mono_ns=int(x['mono_ns']), tid=int(x['tid']))
        records.append(x)
    links = []
    for item in release[name].get('vehicle_callback_to_state_write_links', []):
        event, write = item['event'], item['following_same_thread_state_write']
        events = [x for x in records if x['tid'] == write['tid'] and x['state'] == write['state']]
        entries = [x for x in events if x['kind'] == 'ENTRY' and
                   int(event['callback_mono_s'] * 1e9) <= x['mono_ns'] <= write['mono_ns']]
        if not entries:
            links.append({'event': event, 'state_write': write, 'error': 'entry not found'})
            continue
        entry = entries[-1]
        next_entry = min((x['mono_ns'] for x in events if x['kind'] == 'ENTRY' and x['mono_ns'] > entry['mono_ns']), default=None)
        after = [x for x in events if x['mono_ns'] > write['mono_ns'] and (next_entry is None or x['mono_ns'] < next_entry)]
        links.append({'event': event, 'state_write': write, 'entry': entry,
                      'text_pointer_writes': [x for x in after if x['kind'] == 'TEXT_POINTER_WRITTEN'],
                      'returns': [x for x in after if x['kind'] == 'RETURN']})
    result[name] = {'run_status': run['status'], 'vehicle_links': links,
                    'all_settlement_records': records,
                    'out_of_scope_count': log.count('SETTLEMENT_RETURN_OUT_OF_SCOPE'),
                    'limitations': ['Vehicle path, initial state and stage are debugger-injected; timings use previous posthoc truth.',
                                    'The inferred child component and same-thread ordering do not prove force or natural race completion.',
                                    'Setter return does not prove full score reporting or episode termination.']}
(p / 'settlement_analysis.json').write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps(result, indent=2))
