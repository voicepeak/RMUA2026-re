import json
from pathlib import Path
p = Path('/root/test_disable.json')
d = json.loads(p.read_text())
print('keys', list(d.keys()))
print('limitations', json.dumps(d.get('limitations'), indent=2))
print('profile params', json.dumps(d.get('analysis_profile', {}).get('parameters', {}), indent=2)[:3000])
