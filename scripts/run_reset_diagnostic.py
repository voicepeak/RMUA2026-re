#!/usr/bin/env python3
"""Run the existing fresh-process tracer with an extra read-only reset observer."""
import hashlib
import json
from pathlib import Path
import sys
import run_readonly_trace as trace

trace.LAUNCH=trace.LAUNCH.replace('/analysis/scripts/runtime_trace.gdb', '/analysis/scripts/reset_probe.gdb')
try:
    trace.main()
finally:
    if '--output' in sys.argv:
        out=Path(sys.argv[sys.argv.index('--output')+1])
        if out.exists():
            root=Path(__file__).resolve().parent
            hashes={name:hashlib.sha256((root/name).read_bytes()).hexdigest()
                    for name in ('reset_probe.py','reset_probe.gdb','runtime_trace.gdb')}
            (out/'reset_probe_metadata.json').write_text(json.dumps({'gdb_scripts_sha256':hashes,
                'note':'manifest gdb_script_sha256 names the original base script; the reset probe scripts above were also sourced'},indent=2)+'\n')
