import re
from pathlib import Path
text = Path('/root/frames.txt').read_text(errors='replace')
pat = re.compile(r'pc=([0-9a-f]+)\.\.([0-9a-f]+)')
ranges = []
for line in text.splitlines():
    m = pat.search(line)
    if m:
        ranges.append((int(m.group(1), 16), int(m.group(2), 16)))
ranges.sort()
targets = [0x2fb712e, 0x2fb92d3, 0x2fb9410, 0x2fb94ff, 0x2fb948d, 0x2fb9195,
           0x2fb7fe9, 0x2fb8090, 0x2fb812e, 0x2fb816e, 0x2fb86dc, 0x2fb88fb,
           0x2fc003a, 0x2fb4e80, 0x2fcc5a0, 0x2fd29c0, 0x2fb3a50, 0x2fa0e80,
           0x2fa24a0, 0x2fa3190, 0x2fa3dd0]
for t in targets:
    hits = [(a, b) for a, b in ranges if a <= t < b]
    print(hex(t), [(hex(a), hex(b)) for a, b in hits])
