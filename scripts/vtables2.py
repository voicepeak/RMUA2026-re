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
    print('ELF type', elf.header['e_type'])
    dynsym = elf.get_section_by_name('.dynsym')
    sections = list(elf.iter_sections())
    symtab = {s.name: s for s in dynsym.iter_symbols()}
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

    for name in targets:
        sym = symtab.get(name)
        if sym is None:
            print('MISSING', name)
            continue
        addr = sym['st_value']
        size = sym['st_size']
        data = read_vaddr(addr, size)
        print()
        print('===', name, hex(addr), 'size', size)
        if data is None:
            print('  unreadable')
            continue
        for i in range(size // 8):
            v = struct.unpack_from('<Q', data, i * 8)[0]
            if text_lo <= v < text_hi:
                print(f'  [{i:3d}] {hex(v)}')
