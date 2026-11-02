import re
from pathlib import Path
text = Path('/root/frames.txt').read_text(errors='replace')
pat = re.compile(r'pc=([0-9a-f]+)\.\.([0-9a-f]+)')
ranges = []
for line in text.splitlines():
    m = pat.search(line)
    if m:
        ranges.append((int(m.group(1), 16), int(m.group(2), 16), line.strip()))
ranges.sort()
targets = [0x2fa125c, 0x2fa24a0, 0x2fa3190, 0x2fa3dd0, 0x2fa8405, 0x2faa9b8,
           0x2fb2388, 0x2fbf258, 0x2fb3b83, 0x2fcc86b, 0x2fd2dce, 0x2fd3402]
for t in targets:
    hits = [(a, b, line) for a, b, line in ranges if a <= t < b]
    print(hex(t), [(hex(a), hex(b)) for a, b, _ in hits])
    for a, b, line in hits:
        print('   ', line)
