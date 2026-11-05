#!/usr/bin/env python3
"""Verify a hit is linked only to its same-thread setter transaction."""
import json
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

SCRIPT = Path(__file__).with_name('analyze_line_settlement.py')

class SettlementAnalysisTests(unittest.TestCase):
    def test_same_thread_write_and_return(self):
        with tempfile.TemporaryDirectory() as tmp:
            p = Path(tmp)
            d = p / 'seed123_repeat1'
            d.mkdir()
            (p / 'manifest.json').write_text(json.dumps({'runs': [{'seed': 123, 'repeat': 1, 'status': 'observed'}]}))
            lines = [
                'LINE_FIXTURE selector=1 component=0xaaa', 'LINE_FIXTURE_RESULT index=7',
                'LINE_FORCE_RELEASE mono_ns=100 state=0x111 value=7',
                'LINE_EVENT_ALLOC mono_ns=110 tid=4 actor=0x222 component=0xaaa candidate_index=7 event=0x333',
                'LINE_EVENT_CALLBACK mono_ns=120 tid=4 event=0x333',
                'SETTLEMENT_ENTRY mono_ns=121 tid=9 state=0x111 flag=1 score=5 label=Other',
                'SETTLEMENT_ENTRY mono_ns=122 tid=4 state=0x111 flag=0 score=0 label=Finished',
                'LINE_STATE_TWELVE_WRITTEN mono_ns=125 tid=4 released=1 state=0x111 current=12 previous=7',
                'SETTLEMENT_TEXT_POINTER_WRITTEN mono_ns=126 tid=9 state=0x111 label=Other',
                'SETTLEMENT_TEXT_POINTER_WRITTEN mono_ns=127 tid=4 state=0x111 label=Finished',
                'SETTLEMENT_RETURN mono_ns=128 tid=4 state=0x111 flag=0 score=0 label=Finished',
                'LINE_STATE_SAMPLE mono_ns=150 state=0x111 value=12',
            ]
            (d / 'gdb.log').write_text('\n'.join(lines) + '\n')
            subprocess.run([sys.executable, str(SCRIPT), str(p)], check=True, stdout=subprocess.DEVNULL)
            links = json.loads((p / 'settlement_analysis.json').read_text())['seed123_repeat1']['matched_fixture_links']
            self.assertEqual(len(links), 1)
            self.assertEqual(links[0]['entry']['flag'], '0')
            self.assertEqual([r['label'] for r in links[0]['text_pointer_writes']], ['Finished'])
            self.assertEqual(len(links[0]['returns']), 1)

if __name__ == '__main__':
    unittest.main()
