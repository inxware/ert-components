#!/usr/bin/env python3
"""
cdf_geometry.py — CDF block geometry calculator and validator.

Two modes, both intended to be called by an LLM (or developer) when authoring
or reviewing CDF files.

SUGGEST MODE  — call before writing the CDF:
    python3 cdf_geometry.py suggest --labels "enable" "kick" "error id" --last-y 70
    Outputs: Width, Height, RightPortX, LocationX/Y, TextX/Y  ready to paste
    into the <Block> section.

VALIDATE MODE — call after writing the CDF to confirm correctness:
    python3 cdf_geometry.py validate path/to/block.cdf [path/to/block2.cdf ...]
    Exits 0 if all OK, 1 if any errors found.

Key geometry rules (confirmed from adc_read_single.cdf as canonical reference):
    LocationY   = -15   (standard for IO blocks)
    Effective block bottom = LocationY + Height  = Height − 15
    Height      = last_port_Y + 28   (gives 13 units clearance below last port)
    RightPortX  = Width − 5          (right-side ports inset 5 from right edge)
    LeftPortX   = 0
    HiddenPortX = -1  (InternalPort)
    TextX       = 10, TextY = 5      (title at top; LocationY=-15 provides room)

Width selection is advisory — it depends on the longest port label AND block
complexity.  The tiers below are the observed standard widths; the script
suggests the smallest tier that comfortably fits the longest label and flags
blocks that appear under-sized.  The LLM/author should bump to a wider tier
for complex or protocol-heavy blocks.

    Tier  Fits up to   Typical use
    ----  ----------   -----------
      30   2 chars     Logic gates, operators, mux/demux (no meaningful labels)
      65   9 chars     Simple utilities, buffers, file ops
      75  11 chars     Standard peripheral blocks (ADC, GPIO, watchdog, UPS)
      95  14 chars     Config blocks, UART, NV storage
     105  16 chars     Complex blocks: MQTT, RTC, GUI widgets
     115   20+ chars   Protocol stacks: Modbus, BLE, large runtime blocks

Approximate fit: Width ≈ max_label_chars × 6 + 10, rounded up to next tier.
"""

import sys
import os
import xml.etree.ElementTree as ET
import argparse

# ── Constants ────────────────────────────────────────────────────────────────

LOCATION_Y      = -15   # standard offset for IO blocks
BOTTOM_CLEARANCE = 13   # units of clear space required below last port
HEIGHT_OFFSET   = BOTTOM_CLEARANCE - LOCATION_Y   # = 28:  Height = last_port_Y + 28
RIGHT_INSET     = 5     # right ports sit Width-5 from left edge
TEXT_X          = 10
TEXT_Y          = 5
TEXT_SCALE      = 1.25
PX_PER_CHAR     = 6     # approximate pixel width per character at TextScale=1.25

# Ordered tiers: (max_label_chars_that_fit, width)
WIDTH_TIERS = [
    (2,   30),
    (9,   65),
    (11,  75),
    (14,  95),
    (16, 105),
    (999, 115),
]


# ── Width helpers ─────────────────────────────────────────────────────────────

def suggest_width(max_label_len: int) -> int:
    """Return the smallest standard width tier that fits max_label_len chars."""
    for limit, width in WIDTH_TIERS:
        if max_label_len <= limit:
            return width
    return 115


def width_fits(width: int, max_label_len: int) -> bool:
    """True if width is large enough for max_label_len characters."""
    # Available horizontal space for a single label = (Width - 5) pixels
    return (width - RIGHT_INSET) >= max_label_len * PX_PER_CHAR


# ── Port classification ───────────────────────────────────────────────────────

def port_side(port_type: str) -> str:
    if port_type in ('StartPort', 'InputPort'):
        return 'left'
    if port_type in ('FinishPort', 'OutputPort'):
        return 'right'
    if port_type == 'InternalPort':
        return 'hidden'
    return 'unknown'


# ── SUGGEST mode ──────────────────────────────────────────────────────────────

def cmd_suggest(args):
    """
    Given a list of port labels and the highest port Y coordinate,
    print recommended Block geometry values.
    """
    labels = [l.strip() for l in args.labels if l.strip() and l.strip() != '--']
    last_y = args.last_y

    max_len      = max((len(l) for l in labels), default=0)
    longest      = max(labels, key=len, default='')
    width        = suggest_width(max_len)
    height       = last_y + HEIGHT_OFFSET
    right_port_x = width - RIGHT_INSET

    print(f"Suggested Block geometry")
    print(f"  Max label : {max_len} chars  (\"{longest}\")")
    print()
    print(f"  <Width>{width}</Width>")
    print(f"  <Height>{height}</Height>   <!-- last_port_Y={last_y} + {HEIGHT_OFFSET} -->")
    print(f"  <TextX>{TEXT_X}</TextX>")
    print(f"  <TextY>{TEXT_Y}</TextY>")
    print(f"  <TextScale>{TEXT_SCALE}</TextScale>")
    print(f"  <TextVertical>0</TextVertical>")
    print(f"  <LocationX>0</LocationX>")
    print(f"  <LocationY>{LOCATION_Y}</LocationY>")
    print()
    print(f"  Right-side port XCoordinate : {right_port_x}  (Width-{RIGHT_INSET})")
    print()
    if max_len > 11:
        print(f"  NOTE: label length {max_len} is above the Width={width} comfortable fit.")
        print(f"        Consider bumping to the next tier if the block is complex.")


# ── VALIDATE mode ─────────────────────────────────────────────────────────────

def validate_cdf(cdf_path: str) -> tuple[bool, list[str], list[str]]:
    """
    Parse a CDF file and check block geometry.
    Returns (ok, errors, warnings).
    """
    errors   = []
    warnings = []

    try:
        tree = ET.parse(cdf_path)
    except ET.ParseError as e:
        return False, [f"XML parse error: {e}"], []

    root  = tree.getroot()
    block = root.find('Block')
    if block is None:
        return False, ['No <Block> element found'], []

    block_type = block.findtext('Type', '')
    is_io      = (block_type == 'IO')

    # ── Read Block fields ────────────────────────────────────────────────────
    raw_width  = block.findtext('Width')
    raw_height = block.findtext('Height')
    raw_loc_x  = block.findtext('LocationX')
    raw_loc_y  = block.findtext('LocationY')
    raw_text_x = block.findtext('TextX')
    raw_text_y = block.findtext('TextY')

    width  = int(raw_width)  if raw_width  else None
    height = int(raw_height) if raw_height else None
    loc_x  = int(raw_loc_x) if raw_loc_x  else None
    loc_y  = int(raw_loc_y) if raw_loc_y  else None
    text_x = int(raw_text_x) if raw_text_x else None
    text_y = int(raw_text_y) if raw_text_y else None

    # ── Parse ports ───────────────────────────────────────────────────────────
    all_labels  = []
    max_port_y  = -1

    for port in root.findall('.//Ports/Port'):
        desc      = (port.findtext('Description') or '').strip()
        ptype     = port.findtext('PortType', '')
        side      = port_side(ptype)
        try:
            px = int(port.findtext('XCoordinate', '0'))
            py = int(port.findtext('YCoordinate', '0'))
        except ValueError:
            continue

        if side == 'hidden':
            continue

        if desc and desc != '--':
            all_labels.append(desc)

        if py > max_port_y:
            max_port_y = py

        # Right-side port X check
        if side == 'right' and width is not None:
            expected_x = width - RIGHT_INSET
            if px != expected_x:
                errors.append(
                    f"Port \"{desc}\" (Y={py}) is right-side but X={px}, "
                    f"expected {expected_x} (Width-{RIGHT_INSET})"
                )

        # Left-side port X check
        if side == 'left' and px != 0:
            warnings.append(
                f"Port \"{desc}\" (Y={py}) is left-side but X={px}, expected 0"
            )

    max_label_len = max((len(l) for l in all_labels), default=0)

    # ── Width checks ─────────────────────────────────────────────────────────
    if width is None:
        errors.append("Missing <Width>")
    else:
        if not width_fits(width, max_label_len):
            suggested = suggest_width(max_label_len)
            warnings.append(
                f"Width={width} may be too narrow for max label "
                f"{max_label_len} chars (\"{max(all_labels, key=len, default='')}\") "
                f"— suggested tier: {suggested}"
            )

    # ── Height checks ─────────────────────────────────────────────────────────
    if height is None:
        if max_port_y >= 0:
            loc_y_val     = loc_y if loc_y is not None else 0
            correct_height = max_port_y + BOTTOM_CLEARANCE - loc_y_val
            errors.append(
                f"Missing <Height> — should be {correct_height} "
                f"(last_port_Y={max_port_y} + {HEIGHT_OFFSET})"
            )
        else:
            errors.append("Missing <Height>")
    else:
        if max_port_y >= 0:
            loc_y_val      = loc_y if loc_y is not None else 0
            effective_bot  = loc_y_val + height
            required_bot   = max_port_y + BOTTOM_CLEARANCE
            correct_height = max_port_y + BOTTOM_CLEARANCE - loc_y_val
            if effective_bot < required_bot:
                errors.append(
                    f"Height={height} too small: effective bottom={effective_bot} "
                    f"but need {required_bot} (last_port_Y={max_port_y} + "
                    f"clearance={BOTTOM_CLEARANCE}) — set Height={correct_height}"
                )

    # ── LocationX / LocationY checks (IO blocks only) ─────────────────────────
    if is_io:
        if loc_y is None:
            errors.append(
                f"Missing <LocationY> — must be {LOCATION_Y} for IO blocks "
                f"(without it the block top is at Y=0 and the title is clipped)"
            )
        elif loc_y != LOCATION_Y:
            warnings.append(f"LocationY={loc_y}, standard IO value is {LOCATION_Y}")

        if loc_x is None:
            errors.append("Missing <LocationX> — should be 0")
        elif loc_x != 0:
            warnings.append(f"LocationX={loc_x}, expected 0")

    # ── Title position checks ─────────────────────────────────────────────────
    if text_y is not None and text_y > 10:
        errors.append(
            f"TextY={text_y}: title is placed inside the port area. "
            f"Use TextY={TEXT_Y} (title sits above ports; LocationY={LOCATION_Y} "
            f"provides space above Y=0 for it)"
        )
    if text_x is not None and text_x > 20:
        warnings.append(f"TextX={text_x} is unusually large (expected ~{TEXT_X})")

    ok = len(errors) == 0
    return ok, errors, warnings


def cmd_validate(args):
    any_error = False
    for path in args.cdf:
        ok, errors, warnings = validate_cdf(path)
        name = os.path.basename(path)
        if errors or warnings:
            print(f"\n{'='*60}")
            print(f"{'FAIL' if errors else 'WARN'}  {name}")
            for e in errors:
                print(f"  ERROR   {e}")
            for w in warnings:
                print(f"  WARNING {w}")
            if errors:
                any_error = True
        else:
            print(f"OK    {name}")

    if any_error:
        print()
        print(f"Height formula:  Height = last_port_Y + {HEIGHT_OFFSET}  "
              f"(LocationY={LOCATION_Y}, clearance={BOTTOM_CLEARANCE})")
        print(f"Right port X:    Width - {RIGHT_INSET}")

    sys.exit(1 if any_error else 0)


# ── Entry point ───────────────────────────────────────────────────────────────

def main():
    parser = argparse.ArgumentParser(
        description='CDF block geometry calculator and validator',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog=__doc__
    )
    sub = parser.add_subparsers(dest='command', required=True)

    # suggest
    p_suggest = sub.add_parser(
        'suggest',
        help='Calculate geometry from planned port info (use before writing CDF)'
    )
    p_suggest.add_argument(
        '--labels', nargs='+', required=True,
        help='All port Description strings (both sides; "--" entries are ignored)'
    )
    p_suggest.add_argument(
        '--last-y', type=int, required=True,
        help='Y coordinate of the lowest port'
    )

    # validate
    p_validate = sub.add_parser(
        'validate',
        help='Validate geometry of one or more existing CDF files'
    )
    p_validate.add_argument(
        'cdf', nargs='+',
        help='Path(s) to .cdf file(s)'
    )

    args = parser.parse_args()

    if args.command == 'suggest':
        cmd_suggest(args)
    elif args.command == 'validate':
        cmd_validate(args)


if __name__ == '__main__':
    main()
