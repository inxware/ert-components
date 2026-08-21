#!/usr/bin/env python3
"""
check_cdf_internalport_consistency.py — Cross-check that a CDF's InternalPort
function references and the matching FB's C-side EHS_FB_FUNCTION_ENTRY table
agree about which functions are callbacks.

Companion to:
  - check_cdf_function_args.py (in-CDF arg contiguity / ID uniqueness)
  - check_cdf_hashes.py        (block FBID uniqueness across the tree)

This one walks both the CDF and the FB .c file together. The bug it
prevents is the LoRaWAN-style runtime crash at EXCVADDR=0x18 caused by
a mismatch between:

  - The CDF's <Ports> InternalPort declarations (which Lucid serialises in
    document order to determine each callback's slot in pCallbackTable[]),
    and
  - The FB's EHS_FB_FUNCTION_ENTRY table (which the FB-init code's
    positional macro EHS_FB_INIT_CALLBACK_FUNCTION_INSTANCE(-1 - n)
    indexes into via an enum or hand-written number).

Four classes of mismatch are reported:

  E1 InternalPort references a function ID that doesn't exist in the FB's
     EHS_FB_FUNCTION_ENTRY table.
     → The kernel will fail to look up the run-func at parse time.

  E2 EHS_FB_FUNCTION_ENTRY declares a function whose CDF entry exists but
     has no <Port> referencing it (no StartPort, no FinishPort, no
     InternalPort, no Input/OutputPort).
     → Dead function in the FB; cannot be triggered.

  E3 Multiple InternalPorts reference the same function ID.
     → Two callback slots will resolve to the same FB function, almost
     certainly an authoring slip.

  E4 Two EHS_FB_FUNCTION_ENTRY lines in the FB share the same 8-bit
     function ID for distinct function names.
     → Function IDs are unique only WITHIN one FB (the same byte may be
     reused across different FBs — they each have their own slice of
     EhsCallbackInstanceDataTable). But within one FB an ID collision is
     a hard authoring bug: the kernel's EhsBlockRef_findFunction returns
     the first match, and the new BY_FNID callback lookup will too, so
     the second function silently shadows the first at parse time.
     check_cdf_function_args.py also flags this from the CDF side; this
     check covers the case where the C-side table has drifted from the
     CDF (e.g. a function added to the FB but missed in the CDF).

Note on order checking
----------------------
This tool deliberately does NOT compare the CDF's InternalPort document
order against the FB's EHS_FB_FUNCTION_ENTRY declaration order. The two
orderings are unrelated: EHS_FB_FUNCTION_ENTRY drives the kernel's
name→run-func lookup at parse time and can be in any order. The contract
that actually matters is between the CDF's InternalPort sequence and the
FB-init code's hand-written cb-dispatch table (e.g. lorawan's
gfEhsLorawanFBCBFuncs[] indexed by e_ehs_lorawan_api_cmd_t), which is
FB-specific and not statically discoverable across components.

That contract is the implicit one this whole exercise is about. Static
analysis can't catch it from a CDF + .c file pair alone — the lookup-by-
function-id refactor (CLAUDE-callback-lookup-by-fnid-design.md) removes
it entirely by making CDF order a don't-care. Until that lands, the
mitigation is:
  - CDF must list InternalPorts in the order the FB-init code's cb-array
    expects them (FB-author responsibility).
  - This checker catches the easier-to-detect mismatches (E1/E2/E3) which
    are necessary but not sufficient.

Exit codes:
  0 — clean (no E1/E2/E3 violations).
  1 — at least one E1/E2/E3 violation.

Usage (from repo root):
    python3 scripts/inxware-id-tool/check_cdf_internalport_consistency.py
    python3 scripts/inxware-id-tool/check_cdf_internalport_consistency.py Common/Components
    python3 scripts/inxware-id-tool/check_cdf_internalport_consistency.py path/to/lorawan.cdf
"""

import os
import re
import sys
import xml.etree.ElementTree as ET
from collections import defaultdict


def find_cdf_files(target):
    if os.path.isfile(target):
        if target.endswith('.cdf'):
            yield target
        return
    for dirpath, _, files in os.walk(target):
        for fname in sorted(files):
            if fname.endswith('.cdf'):
                yield os.path.join(dirpath, fname)


def find_fb_c_file(cdf_path, class_name):
    """Locate the FB's .c implementation. Convention is one of:
         <dir>/<class>.c
         <dir>/inx-<class>.c
       Falls back to scanning the directory for a file containing
       EHS_FB_FUNCTIONS_START(<class>).
    """
    cdf_dir = os.path.dirname(cdf_path)
    candidates = [
        os.path.join(cdf_dir, f'{class_name}.c'),
        os.path.join(cdf_dir, f'inx-{class_name}.c'),
    ]
    for c in candidates:
        if os.path.isfile(c):
            return c
    # Fallback scan
    pattern = re.compile(rf'EHS_FB_FUNCTIONS_START(?:_API2)?\s*\(\s*{re.escape(class_name)}\b')
    for fname in sorted(os.listdir(cdf_dir)):
        if not fname.endswith('.c'):
            continue
        path = os.path.join(cdf_dir, fname)
        try:
            with open(path, 'r', encoding='utf-8', errors='replace') as f:
                if pattern.search(f.read()):
                    return path
        except OSError:
            continue
    return None


# Strip C/C++ comments and string literals so EHS_FB_FUNCTION_ENTRY appearing
# in commented-out code or example strings doesn't pollute the parse.
def strip_c_comments(src):
    # Remove block comments first
    src = re.sub(r'/\*.*?\*/', '', src, flags=re.DOTALL)
    # Then line comments
    src = re.sub(r'//[^\n]*', '', src)
    return src


# Captures: name (string literal), id (int), run-func token. Tolerant of
# whitespace/newlines inside the macro args.
RE_FB_FUNCTION_ENTRY = re.compile(
    r'EHS_FB_FUNCTION_ENTRY(?:_API2)?\s*\(\s*'
    r'"([^"]+)"\s*,\s*'
    r'(0[xX][0-9a-fA-F]+|\d+)\s*,\s*'
    r'([A-Za-z_][A-Za-z0-9_]*)',
    re.MULTILINE,
)


def parse_fb_function_entries(c_path, class_name):
    """Return ordered list of (name, id_int, run_func) tuples for the FB
    block whose class is `class_name` in c_path.

    Some legacy components (Common/Components/deprecated/operator.c is the
    canonical example) declare multiple FB classes in a single file, each
    with its own EHS_FB_FUNCTIONS_START(<class>) ... EHS_FB_FUNCTIONS_END
    block. Function IDs are scoped per-FB, so we must only consider entries
    inside the block matching `class_name` — otherwise IDs that legitimately
    repeat across siblings flag as duplicates.

    If the file uses the simpler one-FB-per-file pattern (no
    EHS_FB_FUNCTIONS_START at all, or only one) we fall back to taking
    every entry."""
    try:
        with open(c_path, 'r', encoding='utf-8', errors='replace') as f:
            src = f.read()
    except OSError as e:
        return None, [f'cannot read {c_path}: {e}']
    src = strip_c_comments(src)

    # Find all (start_marker_pos, fb_class) pairs. Use the regex's match end
    # as the start of the FB's body. The body extends to the next
    # EHS_FB_FUNCTIONS_END or to the next EHS_FB_FUNCTIONS_START, whichever
    # comes first.
    start_re = re.compile(
        r'EHS_FB_FUNCTIONS_START(?:_API2)?\s*\(\s*'
        r'([A-Za-z_][A-Za-z0-9_]*)',
    )
    end_re = re.compile(r'EHS_FB_FUNCTIONS_END(?:_API2)?')
    starts = [(m.end(), m.group(1)) for m in start_re.finditer(src)]

    if starts:
        # Build the byte range belonging to `class_name`.
        target_range = None
        for i, (pos, fb) in enumerate(starts):
            if fb != class_name:
                continue
            next_start = starts[i + 1][0] if i + 1 < len(starts) else len(src)
            end_match = end_re.search(src, pos, next_start)
            end_pos = end_match.start() if end_match else next_start
            target_range = (pos, end_pos)
            break
        if target_range is None:
            # FB class not declared in this file — caller's fallback search
            # found us by mistake. Skip rather than emit spurious E4s.
            return [], []
        scoped_src = src[target_range[0]:target_range[1]]
    else:
        # No FUNCTIONS_START anchors at all — file uses a different idiom
        # (e.g. const_*, primitives). Take everything.
        scoped_src = src

    entries = []
    errs = []
    for m in RE_FB_FUNCTION_ENTRY.finditer(scoped_src):
        name, id_str, run_func = m.group(1), m.group(2), m.group(3)
        try:
            id_int = int(id_str, 0)
        except ValueError:
            errs.append(f'{c_path}: cannot parse id {id_str!r} for {name!r}')
            continue
        entries.append((name, id_int, run_func))
    return entries, errs


def parse_cdf(cdf_path):
    """Return (class_name, functions, ports) where:
         functions: ordered list of (name, id_int) from <Functions>
         ports: list of dicts with keys: type, cname, fids (list of ints)
    """
    try:
        root = ET.parse(cdf_path).getroot()
    except ET.ParseError as e:
        return None, None, None, [f'XML parse error: {e}']

    fbid = root.find('./FBID/Class')
    class_name = fbid.text.strip() if fbid is not None and fbid.text else None
    if class_name is None:
        return None, None, None, [f'{cdf_path}: missing <FBID><Class>']

    functions = []
    for fn in root.findall('./Functions/Function'):
        name_el = fn.find('./name')
        id_el = fn.find('./ID/ERT1_ID')
        if name_el is None or id_el is None:
            continue
        try:
            id_int = int(id_el.text.strip(), 0)
        except (ValueError, AttributeError):
            continue
        functions.append((name_el.text.strip(), id_int))

    ports = []
    for port in root.findall('./Ports/Port'):
        ptype_el = port.find('./PortType')
        if ptype_el is None or not ptype_el.text:
            continue
        cname_el = port.find('./CName')
        cname = cname_el.text.strip() if cname_el is not None and cname_el.text else None
        fids = []
        for fn in port.findall('./Function'):
            id_el = fn.find('./Function_ERT1_ID')
            if id_el is not None and id_el.text:
                try:
                    fids.append(int(id_el.text.strip(), 0))
                except ValueError:
                    pass
        ports.append({
            'type': ptype_el.text.strip(),
            'cname': cname,
            'fids': fids,
        })

    return class_name, functions, ports, []


def check_cdf(cdf_path):
    """Return (errors, warnings) for one CDF + its FB .c file."""
    class_name, cdf_functions, ports, parse_errs = parse_cdf(cdf_path)
    if parse_errs:
        return parse_errs, []
    if class_name is None:
        return [], []

    c_path = find_fb_c_file(cdf_path, class_name)
    if c_path is None:
        # No matching .c — silently skip. Some CDFs (e.g. const_*) live in
        # generated form. The check_cdf_hashes script handles those.
        return [], []

    fb_entries, read_errs = parse_fb_function_entries(c_path, class_name)
    if read_errs:
        return read_errs, []
    if not fb_entries:
        return [], []  # FB file uses a different registration scheme

    cdf_id_to_name = {fid: nm for nm, fid in cdf_functions}
    fb_id_to_name = {fid: nm for nm, fid, _ in fb_entries}
    fb_name_to_id = {nm: fid for nm, fid, _ in fb_entries}

    # Functions referenced by *any* port in the CDF.
    referenced_ids = set()
    for p in ports:
        for fid in p['fids']:
            referenced_ids.add(fid)

    # InternalPort-only references (callback slot population).
    internal_ports = [p for p in ports if p['type'] == 'InternalPort']

    errors = []
    warnings = []

    # E1 — InternalPort references a function ID not in the FB table.
    for p in internal_ports:
        for fid in p['fids']:
            if fid not in fb_id_to_name:
                cdf_name = cdf_id_to_name.get(fid, f'<unknown id {fid}>')
                errors.append(
                    f'E1 InternalPort {p["cname"]!r} references function id '
                    f'{fid} ({cdf_name!r}) which is not in the FB\'s '
                    f'EHS_FB_FUNCTION_ENTRY table ({os.path.basename(c_path)})'
                )

    # E2 — function declared in EHS_FB_FUNCTION_ENTRY whose CDF function exists
    # but no <Port> references it. Restricted to functions that *also* appear
    # in the CDF's <Functions> block — function block helpers (e.g. *_API2
    # wrappers or non-CDF entries) are out of scope.
    for fb_name, fb_id, _run_func in fb_entries:
        cdf_fn_name = cdf_id_to_name.get(fb_id)
        if cdf_fn_name is None:
            # FB declares a function the CDF doesn't list at all — likely a
            # non-CDF helper. Skip; not the implicit-contract failure mode.
            continue
        if fb_id not in referenced_ids:
            errors.append(
                f'E2 function {fb_name!r} (id {fb_id}) is in the FB and CDF '
                f'<Functions> table but no <Port> references it — dead function'
            )

    # E3 — multiple InternalPorts pointing at the same function id.
    seen_internal_fids = defaultdict(list)
    for p in internal_ports:
        for fid in p['fids']:
            seen_internal_fids[fid].append(p['cname'])
    for fid, cnames in seen_internal_fids.items():
        if len(cnames) > 1:
            fb_name = fb_id_to_name.get(fid, cdf_id_to_name.get(fid, f'<id {fid}>'))
            errors.append(
                f'E3 multiple InternalPorts reference the same function id '
                f'{fid} ({fb_name!r}): {cnames}'
            )

    # E4 — duplicate function IDs within the FB's EHS_FB_FUNCTION_ENTRY table.
    # Function IDs are scoped to one FB (the kernel's EhsBlockRef_findFunction
    # walks per-FB only, and the new BY_FNID callback lookup walks per-FB
    # only), so the right uniqueness boundary is one FB, not the app.
    # check_cdf_function_args.py independently checks the same constraint on
    # the CDF <Functions> side; E4 catches the case where the FB's C table
    # has drifted (e.g. a duplicated row in EHS_FB_FUNCTION_ENTRY).
    fb_id_to_names = defaultdict(list)
    for nm, fid, _ in fb_entries:
        # Track per-name to ignore harmless re-declarations of the same name
        # (would still be a smell, but not the bug we're catching).
        if nm not in fb_id_to_names[fid]:
            fb_id_to_names[fid].append(nm)
    for fid, names in fb_id_to_names.items():
        if len(names) > 1:
            errors.append(
                f'E4 EHS_FB_FUNCTION_ENTRY id 0x{fid:02X} is reused across '
                f'distinct function names in {os.path.basename(c_path)}: {names}. '
                f'Function IDs must be unique within one FB.'
            )

    # Order checking is intentionally NOT done here — see module docstring.
    return errors, warnings


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
        errs, _warns = check_cdf(path)
        if errs:
            print(f'\n{path}:')
            for e in errs:
                print(f'  ERR  {e}')
            bad_files += 1
            total_errors += len(errs)

    print()
    if total_errors:
        print(f'FAIL: {total_errors} error(s) across {bad_files} file(s).')
        sys.exit(1)
    print(f'OK: all {len(cdf_paths)} CDF(s) clean.')
    sys.exit(0)


if __name__ == '__main__':
    main()
