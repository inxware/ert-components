#!/bin/bash
#---------------------------------------------------------------
# Build the Boot2Qt launcher tile image for the eRT app.
#
# Output geometry (1191x687) matches the stock startupscreen tile so the
# grid lays out identically.  Background #003D46 is sampled from that same
# tile, which sits in the launcher's own dark-teal palette
# (ViewSettings.backgroundColor = #00414a).
#
# The mark is the inxware WORDMARK in off-white, straight from the brand
# kit - no recolouring, no effects, no stretching.  Per
# Marketing/Claude/assets/inx-branding/branding_kit_sheet.md:
#   - off-white #f8f8f8 is the mark colour on dark backgrounds
#   - the wordmark is 1400x500 and must stay a horizontal lockup
#   - minimum clear space is 9% of the mark's width on every side
# No icon+wordmark combined lockup is used because the kit states one does
# not exist yet and inventing the ratio/gap needs design sign-off.
#---------------------------------------------------------------
set -e

# The brand assets live in the Marketing repo, checked out beside the inxware tree -
# same sibling-repo convention as ../EHS-kernel and ../ert-contrib-middleware.
# Override with BRAND=<dir> if yours is somewhere else.
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
BRAND="${BRAND:-${SCRIPT_DIR}/../../../../../../Marketing/Claude/assets/inx-branding}"
SRC="${BRAND}/inxware-logo-v2-borderless-white.svg"
OUT="${1:-preview.png}"

CANVAS_W=1191
CANVAS_H=687
BG="#003D46"
MARK_FRAC=0.72          # wordmark width as a fraction of canvas width
SS=3                    # supersample factor - thin strokes alias badly at 1x

[ -f "$SRC" ] || { echo "brand asset not found: $SRC" >&2; exit 1; }

MARK_W=$(python3 -c "print(int($CANVAS_W * $MARK_FRAC))")
MARK_H=$(python3 -c "print(round($MARK_W * 500 / 1400))")

TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT

rsvg-convert -w $((MARK_W * SS)) -h $((MARK_H * SS)) "$SRC" -o "$TMP/mark.png"

python3 - "$TMP/mark.png" "$OUT" "$CANVAS_W" "$CANVAS_H" "$BG" "$MARK_W" "$MARK_H" <<'PY'
import sys
from PIL import Image

mark_path, out, cw, ch, bg, mw, mh = sys.argv[1:8]
cw, ch, mw, mh = int(cw), int(ch), int(mw), int(mh)

mark = Image.open(mark_path).convert("RGBA").resize((mw, mh), Image.LANCZOS)
canvas = Image.new("RGBA", (cw, ch), bg)
canvas.alpha_composite(mark, ((cw - mw) // 2, (ch - mh) // 2))

# Palette-mode, like the stock tiles - keeps the file small on a read-only rootfs.
canvas.convert("RGB").save(out, "PNG", optimize=True)

clear_h, clear_v = (cw - mw) // 2, (ch - mh) // 2
print(f"wrote {out}  {cw}x{ch}  mark {mw}x{mh}")
print(f"clear space: {clear_h}px h / {clear_v}px v  (kit minimum {round(mw*0.09)}px)")
PY
