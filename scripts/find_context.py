from pathlib import Path

path = Path('/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping')
data = path.read_bytes()

def show(needle, enc, before=120, after=240, limit=6):
    b = needle.encode(enc)
    start = 0
    n = 0
    while n < limit:
        i = data.find(b, start)
        if i < 0:
            break
        lo = max(0, i - before)
        hi = min(len(data), i + len(b) + after)
        chunk = data[lo:hi]
        try:
            text = chunk.decode(enc, errors='replace')
        except Exception:
            text = ''
        text = ''.join(ch if (ch.isprintable() or ch in '\n\t') else '.' for ch in text)
        print('---', needle, enc, hex(i), '---')
        print(text)
        start = i + len(b)
        n += 1

for needle in ['RMUA2024', 'RMUA2023', 'settings_Game', 'Game2024_1', 'Game2023_1']:
    show(needle, 'utf-16-le')
    show(needle, 'latin-1')
