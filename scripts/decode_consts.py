from pathlib import Path
p = Path('/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping')
data = p.read_bytes()
addrs = [0x6cf540, 0x6cc9e0, 0x6cdf60, 0x6cb4e0, 0x6d2a60, 0x6d20b0, 0x6d4b20,
         0x6cdf50, 0x6caa90, 0x6d3be0, 0x6c3b00, 0x6d45e0, 0x6c72e0, 0x6d5d10,
         0x6c8710, 0x6ca640, 0x6d1c00, 0x6cbb20, 0x6d0670, 0x6ce570, 0x6d7100]
for a in addrs:
    off = a - 0x200000
    chunk = data[off:off+64]
    print(hex(a), 'raw:', chunk[:32].hex())
    print('  u16:', chunk.decode('utf-16-le', errors='replace')[:40])
    print('  asc:', ''.join(chr(b) if 32 <= b < 127 else '.' for b in chunk[:40]))
