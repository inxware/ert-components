#!/usr/bin/env python3
"""
check_cdf_function_args.py — Per-CDF internal-shape checks the SODL writer
silently relies on but never enforces.

Companion to check_cdf_hashes.py (which checks block-level FBID uniqueness
and CDF/header consistency) and cdf_validate.py (which checks CDF/C-code
mapping). This one stays inside a single CDF and verifies:

  1. Function 8-bit ID uniqueness per function NAME within the block.
     Two distinct function names sharing the same on-the-wire byte get
     dispatched to whichever the runtime's lookup table picks first.

  2. Per (function, port-type) the 'argument' values form contiguous
     1..N with no holes and no duplicates.
     The SODL writer's port loops index by position after applying
     updateFunctionArg's offsets:
         input  args:  1..N
         output args:  N+1..N+M
         finish args:  N+M+1..N+M+K
     A hole in the per-port-type sequence becomes a hole in the offset
     sequence, and the writer reads default-constructed (zero / -1)
     entries for the missing slot — silently emitting wrong type bytes
     and wrong line IDs. A duplicate causes one entry to overwrite the
     other under SetAtGrow, losing data.

Start ports and internal ports do not enter the data-ID enumeration
(their argument numbering is independent and the writer emits their
trigger line IDs by a different code path), so they are excluded from
the contiguity check.

Exit codes:
  0 — all CDFs clean.
  1 — at least one rule violated.

Usage (from repo root):
    python3 scripts/inxware-id-tool/check_cdf_function_args.py
    python3 scripts/inxware-id-tool/check_cdf_function_args.py Common/Components
    python3 scripts/inxware-id-tool/check_cdf_function_args.py path/to/lorawan.cdf
"""

import os
import sys
import xml.etree.ElementTree as ET
from collections import defaultdict


# Port types whose <Function argument="..."> values participate in the
# data-ID enumeration the SODL writer assembles. StartPort and InternalPort
# trigger functions but their argument numbers are not part of the data slot
# layout, so we skip them.
DATA_PORT_TYPES = ('InputPort', 'OutputPort', 'FinishPort')


def find_cdf_files(target):
    """Yield paths to .cdf files under target (which may be a file or dir)."""
    if os.path.isfile(target):
        if target.endswith('.cdf'):
            yield target
        return
    for dirpath, _, files in os.walk(target):
        for fname in sorted(files):
            if fname.endswith('.cdf'):
                yield os.path.join(dirpath, fname)


def is_contiguous_from_one(values):
    """True iff values is a permutation of 1..len(values) — no holes or dupes."""
    if not values:
        return True
    return sorted(values) == list(range(1, len(values) + 1))


def check_cdf(path):
    """Return list of error strings for a single CDF path."""
    try:
        root = ET.parse(path).getroot()
    except ET.ParseError as e:
        return [f'XML parse error: {e}']

    errors = []

    # Function ID -> name table from <Functions>.
    id_to_name = {}
    for fn in root.findall('./Functions/Function'):
        name_el = fn.find('./name')
        id_el   = fn.find('./ID/ERT1_ID')
        if name_el is None or id_el is None:
            continue
        id_to_name[id_el.text.strip()] = name_el.text.strip()

    # (1) 8-bit ID collisions across distinct names. Same-name reuse is fine
    # (the same function referenced by multiple ports), distinct names sharing
    # the low byte is a collision the SODL writer will emit silently.
    seen_byte = {}  # low_byte -> name
    for sid, name in id_to_name.items():
        try:
            low = int(sid, 0) & 0xFF
        except ValueError:
            errors.append(f'function ID {sid!r} for {name!r} is not an integer')
            continue
        if low in seen_byte and seen_byte[low] != name:
            errors.append(
                f'8-bit ID 0x{low:02X} collides between '
                f'{name!r} and {seen_byte[low]!r}'
            )
        else:
            seen_byte[low] = name

    # (2) Per (function, port-type) arg contiguity. Group raw 'argument' values
    # by function-id then port-type, then verify each group is 1..N.
    by_fn_then_type = defaultdict(lambda: defaultdict(list))
    for port in root.findall('./Ports/Port'):
        ptype_el = port.find('./PortType')
        if ptype_el is None:
            continue
        ptype = ptype_el.text.strip()
        if ptype not in DATA_PORT_TYPES:
            continue
        for fn in port.findall('./Function'):
            arg = fn.attrib.get('argument')
            id_el = fn.find('./Function_ERT1_ID')
            if arg is None or id_el is None:
                continue
            try:
                arg_v = int(arg)
            except ValueError:
                errors.append(
                    f'non-integer argument {arg!r} on port '
                    f'{port.findtext("./Description") or "<unnamed>"!r}'
                )
                continue
            by_fn_then_type[id_el.text.strip()][ptype].append(arg_v)

    for fid in sorted(by_fn_then_type, key=lambda s: int(s) if s.isdigit() else s):
        fname = id_to_name.get(fid, f'<unknown id {fid}>')
        for ptype in DATA_PORT_TYPES:
            vs = by_fn_then_type[fid].get(ptype, [])
            if not is_contiguous_from_one(vs):
                errors.append(
                    f'{fname} ({ptype}): args {sorted(vs)} not contiguous '
                    f'1..{len(vs)}'
                )

    return errors


def main():
    if len(sys.argv) > 1 and not sys.argv[1].startswith('--'):
        target = sys.argv[1]
    else:
        target = 'Common/Components'

    if not os.path.exists(target):
        print(f'ERROR: not found: {target}', file=sys.stderr)
        sys.exit(1)

    cdf_paths = list(find_cdf_files(target))
    if not cdf_paths:
        print(f'No .cdf files found under {target}', file=sys.stderr)
        sys.exit(1)

    print(f'Checking {len(cdf_paths)} CDF file(s) under {target} ...')
    total_errors = 0
    bad_files = 0
    for path in cdf_paths:
        errs = check_cdf(path)
        if errs:
            bad_files += 1
            total_errors += len(errs)
            print(f'\n{path}')
            for e in errs:
                print(f'  - {e}')

    print()
    if total_errors == 0:
        print(f'OK: all {len(cdf_paths)} CDF(s) clean.')
        sys.exit(0)

    print(f'FAIL: {total_errors} issue(s) across {bad_files} file(s).')
    sys.exit(1)


if __name__ == '__main__':
    main()
