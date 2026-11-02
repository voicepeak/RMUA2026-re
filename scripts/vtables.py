from elftools.elf.elffile import ELFFile
from pathlib import Path

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
    '_ZTV22AARSharedWorldGameState',
]

with open(path, 'rb') as f:
    elf = ELFFile(f)
    dynsym = elf.get_section_by_name('.dynsym')
    rela = elf.get_section_by_name('.rela.dyn')
    symtab = {s.name: s for s in dynsym.iter_symbols()}
    relocs = {}
    for r in rela.iter_relocations():
        relocs[r['r_offset']] = r['r_addend']
    for name in targets:
        sym = symtab.get(name)
        if sym is None:
            print('MISSING', name)
            continue
        addr = sym['st_value']
        size = sym['st_size']
        n = size // 8
        print()
        print('===', name, hex(addr), 'size', size, 'slots', n)
        for i in range(n):
            slot = addr + i * 8
            target = relocs.get(slot)
            if target is not None:
                print(f'  [{i:3d}] {hex(slot)} -> {hex(target)}')
