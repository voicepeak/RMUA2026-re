#!/usr/bin/env python3
"""Summarize recorded PWM by command phase without inferring actuator latency."""
import argparse
from collections import Counter
import json
from pathlib import Path

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('directory', type=Path)
args = parser.parse_args()
manifest = json.loads((args.directory / 'manifest.json').read_text())
result = {}
for run in manifest['runs']:
    name = 'seed%d_repeat%d' % (run['seed'], run['repeat'])
    directory = args.directory / name
    try:
        commands = [json.loads(line) for line in (directory / 'pwm_commands.jsonl').open()]
        outputs = [json.loads(line) for line in (directory / 'pwm.jsonl').open()]
    except FileNotFoundError:
        result[name] = {'error': 'missing command or output stream'}
        continue
    transitions = []
    for command in commands:
        if not transitions or command['command'] != transitions[-1]['command']:
            transitions.append(command)
    phases = []
    for i, command in enumerate(transitions):
        start = command['header_ns']
        # The final observed command does not describe an unlimited phase.
        end = (transitions[i + 1]['header_ns'] if i + 1 < len(transitions)
               else commands[-1]['header_ns'])
        values = [sample['pwm'][0] for sample in outputs if 'pwm' in sample
                  and start <= sample['header_stamp_ns'] <= end]
        counts = Counter(round(value, 4) for value in values)
        phases.append({'command': command['command'][0], 'start_header_ns': start,
                       'end_header_ns': end, 'output_samples': len(values),
                       'matching_samples': sum(abs(value - command['command'][0]) < 1e-4
                                               for value in values),
                       'most_common_outputs': counts.most_common(5)})
    result[name] = {'phases': phases, 'limitations':
        'Header timestamps provide approximate phase alignment only. Output values do not establish physical motor response, causal failure, or calibrated latency; the final phase ends at the last recorded command.'}
path = args.directory / 'phase_coverage.json'
path.write_text(json.dumps(result, indent=2) + '\n')
print(json.dumps(result, indent=2))
