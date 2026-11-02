import struct
from elftools.elf.elffile import ELFFile

path = '/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping'
targets = [
    '_ZTV17AGameConfig2024_1',
    '_ZTV17ARMUAGameModeBase',
    '_ZTV15GameState2024_1',
    '_ZTV16ARouteGate2024_1',
    '_ZTV18ARouteSpline2024_1',
    '_ZTV19ARouteSegment2024_1',
    '_ZTV20ARouteJunction2024_1',
    '_ZTV16ARoutePort2024_1',
    '_ZTV19ARouteFactory2024_1',
    '_ZTV18AAirSimROSWindZone',
    '_ZTV21AAirSimROSTimeChecker',
]

with open(path, 'rb') as f:
    elf = ELFFile(f)
    dynsym = elf.get_section_by_name('.dynsym')
    symtab = {s.name: s for s in dynsym.iter_symbols()}
    sections = list(elf.iter_sections())
    text = elf.get_section_by_name('.text')
    text_lo = text['sh_addr']
    text_hi = text_lo + text['sh_size']

    def read_vaddr(addr, size):
        for sec in sections:
            lo = sec['sh_addr']
            hi = lo + sec['sh_size']
            if lo <= addr and addr + size <= hi:
                off = sec['sh_offset'] + (addr - lo)
                f.seek(off)
                return f.read(size)
        return None

    all_targets = {}
    for name in targets:
        sym = symtab.get(name)
        if sym is None:
            continue
        data = read_vaddr(sym['st_value'], sym['st_size'])
        vals = []
        for i in range(sym['st_size'] // 8):
            v = struct.unpack_from('<Q', data, i * 8)[0]
            if text_lo <= v < text_hi:
                vals.append(v)
        all_targets[name] = vals
        game = sorted(v for v in set(vals) if 0x3000000 <= v < 0x4000000)
        print(name, 'slots', len(vals), 'unique', len(set(vals)), 'game_range', len(game))
        print('  ', [hex(v) for v in game])
    print()
    union = sorted(set(v for vals in all_targets.values() for v in vals if 0x3000000 <= v < 0x4000000))
    print('union game range count', len(union))
    print('min', hex(min(union)), 'max', hex(max(union)))
    print(' '.join(hex(v) for v in union))
