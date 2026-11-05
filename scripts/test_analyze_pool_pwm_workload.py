#!/usr/bin/env python3
"""Check workload/observer overlap and missing publisher evidence handling."""
import json,subprocess,sys,tempfile,unittest
from pathlib import Path
SCRIPT=Path(__file__).with_name('analyze_pool_pwm_workload.py')
class WorkloadAnalysisTests(unittest.TestCase):
 def test_overlap_and_missing_action_markers(self):
  with tempfile.TemporaryDirectory() as tmp:
   p=Path(tmp);(p/'pool').mkdir();(p/'stream').mkdir()
   (p/'workload_manifest.json').write_text(json.dumps({'status':'failed','requested_workload_s':6,'limitations':[],'publisher_exit_codes':{'a':0,'b':1}}))
   (p/'pool/pool.json').write_text(json.dumps({'instances':[{'container':'a','seed':1},{'container':'b','seed':2}]}))
   actions=[{'event':'started','mono_ns':2_000_000_000},{'event':'command','mono_ns':3_000_000_000,'value':0.3},{'event':'finished','mono_ns':8_000_000_000}]
   (p/'a.commands.jsonl').write_text(''.join(json.dumps(x)+'\n' for x in actions));(p/'b.commands.jsonl').write_text('')
   samples=[{'event':'sample','topic':'imu','monotonic_ns':x*1_000_000_000,'stamp_ns':(x-1)*1_000_000_000} for x in (1,5,9)]
   for name in ('a','b'):(p/'stream'/f'{name}.jsonl').write_text(''.join(json.dumps(x)+'\n' for x in samples))
   subprocess.run([sys.executable,str(SCRIPT),str(p)],check=True,stdout=subprocess.DEVNULL)
   data=json.loads((p/'workload_analysis.json').read_text())['instances']
   self.assertEqual(data['a']['topics']['imu']['overlap_with_publisher_s'],6)
   self.assertEqual(data['a']['topics']['imu']['zero_stamps'],1)
   self.assertIsNone(data['b']['topics']['imu']['overlap_with_publisher_s'])
if __name__=='__main__':unittest.main()
