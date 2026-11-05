#!/usr/bin/env python3
"""Build a separate diagnostic pak changing only the Lobby's serialized map URL."""
import argparse
import hashlib
import json
from pathlib import Path
import repak

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument('source_pak', type=Path)
parser.add_argument('output_pak', type=Path)
parser.add_argument('--manifest', type=Path, required=True)
parser.add_argument('--default-map', action='store_true', help='also change GameDefaultMap in the overlay config')
args = parser.parse_args()
if args.output_pak.exists() or args.manifest.exists():
    parser.error('output and manifest must not already exist')
old = b'/Game/Maps/RMUA2024OL/RMUA2024OL1'
new = b'/Game/Maps/RMUA2024OL/RMUA2024OL2'
prefix = 'RMUA/Content/Maps/RMUA2024OL/RMUA2024Lobby.'
digest = lambda b: hashlib.sha256(b).hexdigest()
with repak.PakBuilder().reader(str(args.source_pak)) as reader:
    files = {prefix + ext: reader.get(prefix + ext) for ext in ('umap', 'uexp')}
    if files[prefix+'uexp'].count(old) != 1 or len(old) != len(new):
        raise ValueError('expected exactly one equal-length Lobby URL')
    modified = dict(files)
    modified[prefix+'uexp'] = modified[prefix+'uexp'].replace(old, new)
    if args.default_map:
        config = 'RMUA/Config/DefaultEngine.ini'
        files[config] = reader.get(config)
        old_setting = b'GameDefaultMap=/Game/Maps/RMUA2024OL/RMUA2024Lobby.RMUA2024Lobby'
        new_setting = b'GameDefaultMap=/Game/Maps/RMUA2024OL/RMUA2024OL2.RMUA2024OL2'
        if files[config].count(old_setting) != 1:
            raise ValueError('expected one GameDefaultMap setting')
        modified[config] = files[config].replace(old_setting, new_setting)
    with repak.PakBuilder().writer(str(args.output_pak), version=reader.version,
                                   mount_point=reader.mount_point,
                                   path_hash_seed=reader.path_hash_seed) as writer:
        for name, data in modified.items():
            writer.write_file(name, data, compress=False)
with repak.PakBuilder().reader(str(args.output_pak)) as overlay:
    if set(overlay.entries()) != set(modified) or any(overlay.get(k) != v for k, v in modified.items()):
        raise ValueError('overlay verification failed')
info = {'source_pak_sha256': digest(args.source_pak.read_bytes()),
        'output_pak_sha256': digest(args.output_pak.read_bytes()),
        'change': {'old': old.decode(), 'new': new.decode(), 'default_map': args.default_map},
        'entries': {k: {'original_sha256': digest(files[k]), 'overlay_sha256': digest(v),
                        'bytes': len(v)} for k, v in modified.items()},
        'scope': 'separate development overlay; original pak remains unchanged'}
args.manifest.write_text(json.dumps(info, indent=2)+'\n')
