from pathlib import Path
for name in ['RMUA2024Lobby.uexp', 'RMUA2024Lobby.umap', 'GUI2024.uasset', 'GUI2024.uexp']:
    p = Path('/mnt/d/RMUA2026-re/extracted/Lobby') / name
    data = p.read_bytes()
    print('====', name, len(data))
    for needle in ['RMUA2024OL', 'GameConfig']:
        start = 0
        for _ in range(5):
            i = data.find(needle.encode('utf-16-le'), start)
            if i < 0:
                break
            lo = max(0, i - 96)
            hi = min(len(data), i + 192)
            text = data[lo:hi].decode('utf-16-le', errors='replace')
            text = ''.join(ch if ch.isprintable() else '.' for ch in text)
            print(needle, hex(i), repr(text))
            start = i + 2
    # print all runs of printable utf16
    runs = []
    i = 0
    while i + 1 < len(data):
        chars = []
        j = i
        while j + 1 < len(data) and data[j+1] == 0 and 32 <= data[j] < 127:
            chars.append(chr(data[j]))
            j += 2
        if len(chars) >= 4:
            runs.append((i, ''.join(chars)))
            i = j
        else:
            i += 2
    print('runs:')
    for off, s in runs[:80]:
        print(hex(off), repr(s))
