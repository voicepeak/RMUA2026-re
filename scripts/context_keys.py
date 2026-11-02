from pathlib import Path
d = Path('/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping').read_bytes()
for k in ['FlightTime', 'CurStateTime', 'SegmentInterval', 'Score', 'DontScore', 'ScoreConstant']:
    for enc in ('utf-16-le', 'utf-8'):
        i = d.find(k.encode(enc))
        if i >= 0:
            lo = max(0, i - 120); hi = min(len(d), i + 240)
            text = d[lo:hi].decode(enc, errors='replace')
            text = ''.join(ch if ch.isprintable() else '.' for ch in text)
            print('====', k, enc, hex(i))
            print(text)
            break
