#!/usr/bin/env python3
"""Verify source, candidate index and ordered state linkage in a synthetic trace."""
import json,subprocess,sys,tempfile,unittest
from pathlib import Path
SCRIPT=Path(__file__).with_name('analyze_line_release_state.py')
class LineReleaseAnalysisTests(unittest.TestCase):
 def test_component_index_and_callback_order(self):
  with tempfile.TemporaryDirectory() as tmp:
   p=Path(tmp);d=p/'seed123_repeat1';d.mkdir();(p/'manifest.json').write_text(json.dumps({'runs':[{'seed':123,'repeat':1}]}))
   (d/'gdb.log').write_text('\n'.join([
    'LINE_FIXTURE selector=1 component=0xaaa','LINE_FIXTURE_RESULT index=7',
    'LINE_FORCE_RELEASE mono_ns=100 state=0x111 value=7',
    'LINE_EVENT_ALLOC mono_ns=110 tid=4 actor=0x222 component=0xaaa candidate_index=7 event=0x333',
    'LINE_EVENT_CALLBACK mono_ns=120 tid=4 event=0x333',
    'LINE_STATE_TWELVE_WRITTEN mono_ns=125 tid=4 released=1 state=0x111 current=12 previous=7',
    'LINE_EVENT_ALLOC mono_ns=130 tid=4 actor=0x222 component=0xaaa candidate_index=6 event=0x444',
    'LINE_EVENT_CALLBACK mono_ns=140 tid=4 event=0x444',
    'LINE_STATE_SAMPLE mono_ns=150 state=0x111 value=12',
    'LINE_STATE_SAMPLE mono_ns=250 state=0x111 value=12'])+'\n')
   subprocess.run([sys.executable,str(SCRIPT),str(p)],check=True,stdout=subprocess.DEVNULL)
   x=json.loads((p/'line_release_analysis.json').read_text())['seed123_repeat1'];self.assertEqual(x['post_release_fixture_pairs'],1)
   self.assertEqual(len(x['fixture_callback_to_state_write_links']),1);self.assertEqual(x['values_after_first_terminal'],[12])
if __name__=='__main__':unittest.main()
