#!/usr/bin/env python3
"""Exercise batch retry/cleanup using fake Docker, without launching a simulator."""
import contextlib
import io
import json
import sys
import tempfile
import unittest
from pathlib import Path
from types import SimpleNamespace
from unittest.mock import patch
import run_pool_batches as runner

class BatchTests(unittest.TestCase):
    def run_case(self, cleanup_failure=False):
        with tempfile.TemporaryDirectory() as tmp:
            output=Path(tmp)/'out'; calls=[]
            def fake_command(command, logfile):
                calls.append(command[2])
                logfile.write_text('mock\n')
                if command[2]=='start':
                    target=Path(command[command.index('--output')+1]);target.mkdir()
                    (target/'pool.json').write_text(json.dumps({'instances':[{'id':'fake','container':'fake'}]}))
                    return 1 if calls.count('start')==1 and not cleanup_failure else 0
                return 1 if cleanup_failure else 0
            def fake_inspect(*args,**kwargs):
                return SimpleNamespace(returncode=0,stdout='{"Running": true}',stderr='')
            argv=['run_pool_batches.py','--simulator','/tmp/sim','--settings','/tmp/settings',
                  '--seeds','123,456','--output',str(output),'--seconds','0.001','--retries','1']
            with patch.object(sys,'argv',argv), patch.object(runner,'cmd_run',fake_command), \
                 patch.object(runner.subprocess,'run',fake_inspect), contextlib.redirect_stdout(io.StringIO()):
                if cleanup_failure:
                    with self.assertRaisesRegex(RuntimeError,'refusing'):runner.main()
                else:
                    self.assertEqual(runner.main(),0)
            data=json.loads((output/'results.json').read_text())
            self.assertEqual(calls,['start','stop'] if cleanup_failure else ['start','stop','start','stop'])
            self.assertEqual(data['batches'][0]['success'],not cleanup_failure)
            if not cleanup_failure:self.assertNotEqual(data['batches'][0]['attempts'][0]['port'],data['batches'][0]['attempts'][1]['port'])
    def test_retry_uses_fresh_port_and_cleans(self):self.run_case()
    def test_cleanup_failure_blocks_retry(self):self.run_case(cleanup_failure=True)

if __name__=='__main__':unittest.main()
