from elftools.elf.elffile import ELFFile
from elftools.dwarf.callframe import CallFrameInfo, FDE
from elftools.dwarf.structs import DWARFStructs
from elftools.elf.sections import SymbolTableSection

path = '/mnt/d/RMUA2026-re/sim/simulator_12.0.0.5/Build/LinuxNoEditor/RMUA/Binaries/Linux/RMUA-Linux-Shipping'
targets = [0x2fa125c, 0x2fa24a0, 0x2fa3190, 0x2fa3dd0, 0x2fa8405, 0x2faa9b8,
           0x2fb2388, 0x2fbf258, 0x2fb3b83, 0x2fcc86b, 0x2fd2dce]
with open(path, 'rb') as f:
    elf = ELFFile(f)
    sec = elf.get_section_by_name('.eh_frame')
    data = sec.data()
    structs = DWARFStructs(little_endian=True, dwarf_format=64, address_size=8)
    cfi = CallFrameInfo(data, sec['sh_addr'], structs=structs)
    entries = list(cfi.parse_entries())
    fdes = [e for e in entries if isinstance(e, FDE)]
    print('fdes', len(fdes))
    for t in targets:
        hits = []
        for e in fdes:
            start = e['initial_location']
            size = e['address_range']
            if start <= t < start + size:
                hits.append((start, size, start + size))
        print(hex(t), [(hex(a), hex(s), hex(b)) for a, s, b in hits])
