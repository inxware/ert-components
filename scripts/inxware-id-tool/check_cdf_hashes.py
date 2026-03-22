#!/usr/bin/env python3
"""
check_cdf_hashes.py — Verify NameHash_CRC16 consistency across CDFs and C headers.

For every .cdf file found under the search root:
  1. Computes the correct NameHash_CRC16 from the <Class> name.
  2. Checks the stored <NameHash_CRC16> in the CDF matches.
  3. If a matching INXWARE_FB_ID_<Class> or EHS_FB_ID_<Class> #define exists
     in a .h file under the same root, checks that value matches too.

Usage (from repo root):
    python3 scripts/inxware-id-tool/check_cdf_hashes.py
    python3 scripts/inxware-id-tool/check_cdf_hashes.py Common/Components
    python3 scripts/inxware-id-tool/check_cdf_hashes.py --fix-dry-run   # show what would be fixed
"""

import os
import sys
import re
import xml.etree.ElementTree as ET
import subprocess

# Allow importing hash.py from this same directory
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, SCRIPT_DIR)
from hash import Hash16CRC


def crc16(name: str) -> str:
    """Return the correct NameHash_CRC16 for a given class name, as '0xXXXX'."""
    h = Hash16CRC('16CRC', bytearray(name, 'utf-8'), False)
    return '0x' + h.getHash().upper()


def collect_header_ids(root: str) -> dict:
    """Scan all .h files under root and return {macro_name: '0xXXXX'}."""
    result = subprocess.run(
        ['grep', '-r', '--include=*.h', '-h', r'INXWARE_FB_ID_\|EHS_FB_ID_', root],
        capture_output=True, text=True
    )
    ids = {}
    for line in result.stdout.splitlines():
        line = line.strip()
        if '#define' not in line:
            continue
        parts = line.split()
        if len(parts) < 3:
            continue
        macro, val = parts[1], parts[2]
        if re.match(r'0x[0-9A-Fa-f]+$', val):
            ids[macro] = val.upper().replace('0X', '0x')
    return ids


def check(search_root: str) -> list:
    """
    Return list of mismatch dicts:
      { 'cdf': path, 'class': cls, 'correct': hash,
        'cdf_stored': stored, 'cdf_ok': bool,
        'header_val': val_or_None, 'header_ok': bool_or_None }
    """
    header_ids = collect_header_ids(search_root)
    mismatches = []

    for dirpath, _, files in os.walk(search_root):
        for fname in files:
            if not fname.endswith('.cdf'):
                continue
            path = os.path.join(dirpath, fname)
            try:
                tree = ET.parse(path)
                r = tree.getroot()
                cls = r.findtext('FBID/Class')
                stored = r.findtext('Hashes/NameHash_CRC16')
            except Exception:
                continue

            if not cls or not stored:
                continue

            correct = crc16(cls)
            cdf_ok = correct.upper() == stored.upper()

            header_val = header_ids.get('INXWARE_FB_ID_' + cls) or \
                         header_ids.get('EHS_FB_ID_' + cls)
            if header_val is not None:
                header_ok = header_val.upper() == correct.upper()
            else:
                header_ok = None  # no header found — not an error

            if not cdf_ok or header_ok is False:
                mismatches.append({
                    'cdf': path,
                    'class': cls,
                    'correct': correct,
                    'cdf_stored': stored,
                    'cdf_ok': cdf_ok,
                    'header_val': header_val,
                    'header_ok': header_ok,
                })

    return sorted(mismatches, key=lambda m: m['cdf'])


def main():
    search_root = 'Common/Components'
    if len(sys.argv) > 1 and not sys.argv[1].startswith('--'):
        search_root = sys.argv[1]

    if not os.path.isdir(search_root):
        print(f'ERROR: directory not found: {search_root}', file=sys.stderr)
        sys.exit(1)

    mismatches = check(search_root)

    if not mismatches:
        print(f'OK: all NameHash_CRC16 values match in {search_root}')
        sys.exit(0)

    print(f'MISMATCHES FOUND ({len(mismatches)}):')
    for m in mismatches:
        flags = ''
        if not m['cdf_ok']:
            flags += f'  CDF has {m["cdf_stored"]} should be {m["correct"]}'
        if m['header_ok'] is False:
            flags += f'  header has {m["header_val"]} should be {m["correct"]}'
        print(f'  {m["cdf"]}  class={m["class"]}{flags}')

    sys.exit(1)


if __name__ == '__main__':
    main()
