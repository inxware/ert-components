#!/usr/bin/env python3
"""
check_cdf_hashes.py — Check function-block ID uniqueness and CDF/header consistency.

For every .cdf file found under the search root this tool:
  1. Collects the <NameHash_CRC16> stored in each CDF alongside its <Class> name.
  2. Checks that every ID value is unique — no two function blocks may share one.
  3. If a matching #define (INXWARE_FB_ID_<Class> or EHS_FB_ID_<Class>) exists
     in a .h file under the same root, checks that the value matches the CDF.

Exit codes:
  0 — all IDs unique and consistent between CDF and header.
  1 — duplicate IDs found, or CDF/header value mismatch.

Warnings (non-fatal):
  Any ID whose stored value does not equal CRC16(<Class>) is listed as a warning.
  These are intentionally assigned values and will be updated in a future pass.

Usage (from repo root):
    python3 scripts/inxware-id-tool/check_cdf_hashes.py
    python3 scripts/inxware-id-tool/check_cdf_hashes.py Common/Components
"""

import os
import sys
import re
import xml.etree.ElementTree as ET
import subprocess
from collections import defaultdict

# Import the canonical CRC16 implementation — used only for the optional
# non-CRC16 warning, not as the source of truth for correctness checks.
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, SCRIPT_DIR)
from hash import Hash16CRC


def crc16(name: str) -> str:
    """Return the CRC16 of a class name as '0xXXXX', for advisory comparison."""
    h = Hash16CRC('16CRC', bytearray(name, 'utf-8'), False)
    return '0x' + h.getHash().upper()


def collect_cdf_ids(root: str) -> list:
    """Walk root and return a list of dicts, one per .cdf file:
      { 'path', 'class', 'id', 'nocode' }
    Files that cannot be parsed or lack <Class>/<NameHash_CRC16> are skipped.
    nocode=True for <ISNoCode>true</ISNoCode> blocks (Sub_System_IO encapsulation
    ports); they intentionally share a class name across type variants and are
    excluded from the uniqueness check.
    """
    entries = []
    for dirpath, _, files in os.walk(root):
        for fname in sorted(files):
            if not fname.endswith('.cdf'):
                continue
            path = os.path.join(dirpath, fname)
            try:
                r = ET.parse(path).getroot()
                cls    = r.findtext('FBID/Class')
                stored = r.findtext('Hashes/NameHash_CRC16')
                nocode = (r.findtext('ISNoCode') or '').strip().lower() == 'true'
            except Exception:
                continue
            if cls and stored:
                entries.append({
                    'path':   path,
                    'class':  cls,
                    'id':     stored.upper().replace('0X', '0x'),
                    'nocode': nocode,
                })
    return entries


def collect_header_ids(root: str) -> dict:
    """Scan all .h files under root and return {macro_name: '0xXXXX'}.

    Matches lines of the form:
        #define INXWARE_FB_ID_ClassName  0xABCD
        #define EHS_FB_ID_ClassName      0xABCD
    """
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


def check(search_root: str):
    """Run all checks and return (errors, warnings).

    errors  — list of strings describing duplicate IDs or CDF/header mismatches.
    warnings — list of strings for IDs that are not the CRC16 of their class name.
    """
    entries    = collect_cdf_ids(search_root)
    header_ids = collect_header_ids(search_root)

    errors   = []
    warnings = []

    # ── 1. Uniqueness check ───────────────────────────────────────────────────
    # Build a map of id_value -> [list of (class, path)] to find duplicates.
    # ISNoCode blocks (Sub_System_IO encapsulation ports) are excluded: they
    # intentionally share a class name across type variants (e.g. xoutputb /
    # xoutputi / xoutputr / xoutputs all use class=xoutput) and have no C
    # implementation whose lookup depends on a unique ID.
    id_map = defaultdict(list)
    for e in entries:
        if not e['nocode']:
            id_map[e['id']].append(e)

    for id_val, owners in sorted(id_map.items()):
        if len(owners) > 1:
            lines = '\n'.join(f'    {o["path"]}  class={o["class"]}' for o in owners)
            errors.append(f'DUPLICATE ID {id_val}:\n{lines}')

    # ── 2. CDF / header consistency check ────────────────────────────────────
    # For each CDF, if a matching header #define exists its value must agree.
    for e in entries:
        header_val = (header_ids.get('INXWARE_FB_ID_' + e['class']) or
                      header_ids.get('EHS_FB_ID_'      + e['class']))
        if header_val is not None and header_val.upper() != e['id'].upper():
            errors.append(
                f'CDF/HEADER MISMATCH  {e["path"]}\n'
                f'    class={e["class"]}  CDF={e["id"]}  header={header_val}'
            )

    # ── 3. Non-CRC16 warning (advisory) ──────────────────────────────────────
    # If the stored ID does not match the CRC16 of the class name, note it.
    # This is not an error — intentionally assigned values are valid — but flags
    # entries that will need attention when IDs are regenerated.
    for e in entries:
        expected = crc16(e['class'])
        if e['id'].upper() != expected.upper():
            warnings.append(
                f'  {e["path"]}  class={e["class"]}  '
                f'stored={e["id"]}  CRC16={expected}'
            )

    return errors, warnings


def main():
    search_root = 'Common/Components'
    if len(sys.argv) > 1 and not sys.argv[1].startswith('--'):
        search_root = sys.argv[1]

    if not os.path.isdir(search_root):
        print(f'ERROR: directory not found: {search_root}', file=sys.stderr)
        sys.exit(1)

    print(f'Checking function-block ID uniqueness and CDF/header consistency...')
    errors, warnings = check(search_root)

    if warnings:
        print(f'\nWARNING — {len(warnings)} ID(s) are not the CRC16 of their class name'
              f' (intentional; will be updated in a future pass):')
        for w in warnings:
            print(w)

    if not errors:
        print(f'\nOK: all IDs unique and consistent in {search_root}')
        sys.exit(0)

    print(f'\nERRORS ({len(errors)}):')
    for e in errors:
        print(f'  {e}')
    print(f'\nERROR: ID check FAILED')
    sys.exit(1)


if __name__ == '__main__':
    main()
