import subprocess

out = subprocess.check_output(
    ['nm', '-D', '-n', '--defined-only',
     '/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping'],
    text=True, errors='replace')
rows = []
for line in out.splitlines():
    parts = line.split(None, 2)
    if len(parts) == 3:
        try:
            addr = int(parts[0], 16)
        except ValueError:
            continue
        rows.append((addr, parts[2]))
rows.sort()
for target in [0x328b530, 0x33381f0, 0x33385c0, 0x3338650, 0x3338f10, 0x3338f30,
               0x3339200, 0x33396e0, 0x3339f70, 0x333a1d0, 0x333a230, 0x333a280,
               0x333a380, 0x333ffb0, 0x33b03f0, 0x33b5b20]:
    prev = None
    nxt = None
    for addr, name in rows:
        if addr <= target:
            prev = (addr, name)
        else:
            nxt = (addr, name)
            break
    print(hex(target), 'prev', hex(prev[0]) if prev else None, prev[1][:120] if prev else None,
          'next', hex(nxt[0]) if nxt else None, nxt[1][:120] if nxt else None)
