#!/usr/bin/env python3
"""Use the existing isolated runtime harness with a read-only frequency probe.

Arguments are identical to run_readonly_trace.py, but use no --reset and use a
fresh output directory. Results describe all-stop debugger-altered scheduling.
"""
import run_readonly_trace as harness
harness.LAUNCH = harness.LAUNCH.replace('runtime_trace.gdb','frequency_overlap_probe.gdb')
# The upstream harness hashes runtime_trace.gdb; add the actual hashes afterwards.
if __name__ == '__main__':
    import json
    from pathlib import Path
    import sys
    import argparse
    parser=argparse.ArgumentParser(add_help=False)
    parser.add_argument('--output',type=Path,required=True)
    known,_=parser.parse_known_args()
    harness.main()
    manifest=known.output/'manifest.json'
    if manifest.exists():
        data=json.loads(manifest.read_text())
        base=Path(__file__).resolve().parent
        data['actual_gdb_script_sha256']=harness.sha(base/'frequency_overlap_probe.gdb')
        data['actual_gdb_python_sha256']=harness.sha(base/'frequency_overlap_probe.py')
        data['gdb_script_sha256_note']='legacy runtime_trace.gdb field inherited from harness is not the selected script'
        data['probe_limit']='All-stop GDB changes scheduling. Overlap confirms simultaneous active calls, not concurrent instruction execution or data race. No overlap is not evidence of absence.'
        manifest.write_text(json.dumps(data,indent=2)+'\n')
