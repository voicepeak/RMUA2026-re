#!/usr/bin/env python3
"""Summarize completed line-release trials, retaining missing/error cases."""
import json
from pathlib import Path

root = Path(__file__).resolve().parents[1]
rows = []
for kind in ('thick', 'thin'):
    for seed in (123, 456, 2026):
        for repeat in (1, 2):
            directory = (f'line_release_{kind}_pilot_20261106' if seed == 123 and repeat == 1
                         else f'line_release_{kind}_seed{seed}_repeat{repeat}_20261106')
            path = root / 'experiments' / directory
            row = {'kind': kind, 'seed': seed, 'repeat': repeat,
                   'evidence': str(path.relative_to(root))}
            try:
                manifest = json.loads((path / 'manifest.json').read_text())
                analyses = json.loads((path / 'line_release_analysis.json').read_text())
                run = manifest['runs'][0]
                result = analyses[f"seed{run['seed']}_repeat{run['repeat']}"]
                row['run_status'] = run['status']
                if 'error' in result:
                    row['error'] = result['error']
                else:
                    for key in ('fixture_selector', 'fixture_return_index', 'release_state',
                                'all_event_pairs', 'unmatched_callbacks', 'post_release_fixture_pairs',
                                'terminal_samples', 'terminal_sample_span_s', 'values_after_first_terminal'):
                        row[key] = result[key]
                    row['linked_writes'] = [{k: link['following_same_thread_write'][k]
                                              for k in ('current', 'previous')}
                                             for link in result['fixture_callback_to_state_write_links']]
            except (OSError, ValueError, KeyError, IndexError) as exc:
                row['error'] = f'{type(exc).__name__}: {exc}'
            rows.append(row)
out = root / 'experiments' / 'line_release_matrix_20261106'
out.mkdir(exist_ok=True)
(out / 'summary.json').write_text(json.dumps({'runs': rows}, indent=2) + '\n')
print(json.dumps({'runs': rows}, indent=2))
