#!/usr/bin/env python3
"""Correlate state-setter entry, text-pointer write and return with a line hit."""
import json
import re
import subprocess
import sys
from pathlib import Path

path = Path(sys.argv[1])
subprocess.run([sys.executable, str(Path(__file__).with_name('analyze_line_release_state.py')), str(path)],
               check=True, stdout=subprocess.DEVNULL)
manifest = json.loads((path / 'manifest.json').read_text())
release = json.loads((path / 'line_release_analysis.json').read_text())
result = {}
for run in manifest['runs']:
    name = f"seed{run['seed']}_repeat{run['repeat']}"
    log = (path / name / 'gdb.log').read_text(errors='replace')
    records = []
    for match in re.finditer(r'^SETTLEMENT_(ENTRY|RETURN|TEXT_POINTER_WRITTEN) (.*)$', log, re.M):
        fields = dict(re.findall(r'(\w+)=(\S*)', match[2]))
        fields['kind'] = match[1]
        fields['mono_ns'] = int(fields['mono_ns'])
        fields['tid'] = int(fields['tid'])
        records.append(fields)
    links = []
    for link in release[name].get('fixture_callback_to_state_write_links', []):
        hit, write = link['hit'], link['following_same_thread_write']
        events = [x for x in records if x['tid'] == write['tid'] and x['state'] == write['state']]
        entries = [x for x in events if x['kind'] == 'ENTRY' and hit['callback_ns'] <= x['mono_ns'] <= write['mono_ns']]
        if not entries:
            links.append({'hit': hit, 'state_write': write, 'error': 'entry not found'})
            continue
        entry = entries[-1]
        next_entry = min((x['mono_ns'] for x in events if x['kind'] == 'ENTRY' and x['mono_ns'] > entry['mono_ns']), default=None)
        after = [x for x in events if x['mono_ns'] > write['mono_ns'] and (next_entry is None or x['mono_ns'] < next_entry)]
        links.append({'hit': hit, 'state_write': write, 'entry': entry,
                      'text_pointer_writes': [x for x in after if x['kind'] == 'TEXT_POINTER_WRITTEN'],
                      'returns': [x for x in after if x['kind'] == 'RETURN']})
    result[name] = {'run_status': run['status'], 'matched_fixture_links': links,
                    'all_settlement_records': records,
                    'out_of_scope_count': log.count('SETTLEMENT_RETURN_OUT_OF_SCOPE'),
                    'limitations': ['Fixture and initial state are debugger-injected; timing uses previous posthoc truth.',
                                    'The event index is inferred; same-thread ordering is not a force measurement.',
                                    'The setter return and text pointer write do not prove complete score reporting or episode termination.']}
(path / 'settlement_analysis.json').write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps(result, indent=2))
