from pathlib import Path
p = Path('/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping')
data = p.read_bytes()
for enc in ('utf-16-le', 'latin-1'):
    needle = 'seed'.encode(enc)
    start = 0
    print('====', enc)
    n = 0
    while n < 30:
        i = data.find(needle, start)
        if i < 0:
            break
        lo = max(0, i - 80)
        hi = min(len(data), i + 120)
        chunk = data[lo:hi]
        text = chunk.decode(enc, errors='replace')
        text = ''.join(ch if ch.isprintable() else '.' for ch in text)
        print(hex(i), repr(text))
        start = i + len(needle)
        n += 1
