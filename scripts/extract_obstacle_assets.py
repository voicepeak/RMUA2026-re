#!/usr/bin/env python3
"""Extract selected packaged obstacle blueprints with repak, preserving source hashes."""
import argparse
import hashlib
import json
from pathlib import Path
import repak

NAMES = ('Resources/Hover/BP_Hover_1', 'Resources/Line/BP_ThickLine',
         'Resources/Line/BP_ThinLine')
MESHES = ('Resources/HoverTruck/HoverTruck', 'Resources/Line/thickline',
          'Resources/Line/thinline')

def digest(data): return hashlib.sha256(data).hexdigest()

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('pak', type=Path)
    parser.add_argument('output_dir', type=Path)
    parser.add_argument('--meshes', action='store_true', help='extract mesh resources instead of Blueprints')
    args = parser.parse_args()
    args.output_dir.mkdir(parents=True, exist_ok=True)
    reader = repak.PakBuilder().reader(str(args.pak))
    rows = []
    for name in (MESHES if args.meshes else NAMES):
        for ext in ('.uasset', '.uexp'):
            path = 'RMUA/Content/' + name + ext
            data = reader.get(path)
            if ext == '.uasset' and data[:4] != bytes.fromhex('c1832a9e'):
                raise ValueError('unexpected asset header for ' + path)
            target = args.output_dir / (Path(name).name + ext)
            if target.exists(): raise FileExistsError(target)
            target.write_bytes(data)
            rows.append({'pak_path':path, 'output':str(target), 'bytes':len(data), 'sha256':digest(data)})
    reader.close()
    pak_hash = hashlib.sha256()
    with args.pak.open('rb') as f:
        for chunk in iter(lambda:f.read(1024*1024),b''): pak_hash.update(chunk)
    print(json.dumps({'pak':str(args.pak.resolve()),'pak_sha256':pak_hash.hexdigest(),'files':rows},indent=2))

if __name__ == '__main__': main()
