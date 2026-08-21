#!/usr/bin/env bash
#---------------------------------------------------------------
# Copyright (C) 2026, inx limited, UK.
# All Rights Reserved.  LGPLv3 — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# check_target_keys.sh — assert that every platform target's resolved dependency
# keys exist on disk.
#
# Two keys are derived per target (see docs/target-libs-naming-audit.md):
#
#   kernel  : ert-build-support/support_libs/target_libs/$(EHS_GNU_OS_ARCH)/kernel/
#   contrib : ert-contrib-middleware/target_libs/$(COMPONENT_BASE_TECHNOLOGIES)/build/
#
# ert-components and EHS-kernel compose these independently and nothing checks
# that a producer and a consumer agree. A target whose key has no directory does
# not fail loudly — it links a stale archive or silently falls back. This makes
# that condition visible.
#
# Keys are read back from the build system (make chkconfig) rather than
# re-derived here. Re-deriving the composition rule by hand has been wrong
# repeatedly, notably for MCU targets whose contrib key carries a toolchain
# suffix.
#
# Usage:
#   check_target_keys.sh                 report, and fail on NEW breakage
#   check_target_keys.sh --list          report every target, always exit 0
#   check_target_keys.sh --update-baseline   re-record the known-broken set
#
# Exit: 0 = no new breakage.  1 = a target broke that the baseline does not know
# about.  A target that *starts* resolving is reported but is not a failure.
#
# Contrib note: not every os-arch has contrib libraries (no zephyr target does,
# for instance), so a missing contrib key is reported as INFO, not breakage.
# Only the kernel key is load-bearing for every target.

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/../.." && pwd)"
BASELINE="${ROOT_DIR}/SystemTests/CI/known-broken-keys.txt"

cd "${ROOT_DIR}" || exit 1

MODE="check"
case "${1:-}" in
    --list)            MODE="list" ;;
    --update-baseline) MODE="update" ;;
    "")                ;;
    *) echo "Unknown option: $1" >&2; exit 2 ;;
esac

# TARGET.cfg is global mutable state and ./configure rewrites it. Restore it on
# every exit path so running this never disturbs the working target.
SAVED_TARGET=""
[ -f TARGET.cfg ] && SAVED_TARGET="$(cat TARGET.cfg)"
restore_target() {
    [ -n "${SAVED_TARGET}" ] && printf '%s\n' "${SAVED_TARGET}" > TARGET.cfg
}
trap restore_target EXIT
trap 'exit 130' INT
trap 'exit 143' TERM

broken=()   # kernel key missing — real breakage
noinfo=()   # contrib key missing — informational
ok=0

for p in $(ls -1 target/platform/ | grep -v '\.mk$'); do
    [ -f "target/platform/${p}/config.mk" ] || continue
    if ! ./configure "$p" >/dev/null 2>&1; then
        broken+=("${p}|CONFIGURE_FAILED|-")
        continue
    fi
    out="$(make chkconfig 2>&1 | sed 's/\x1b\[[0-9;]*m//g')"
    knl="$(printf '%s\n' "$out" | grep -m1 'EHS KERNEL:'  || true)"
    cnt="$(printf '%s\n' "$out" | grep -m1 'CONTRIB INC:' || true)"
    knl_key="$(printf '%s' "$knl" | sed 's|.*/target_libs/||; s|/kernel/.*||')"
    cnt_key="$(printf '%s' "$cnt" | sed 's|.*/target_libs/||; s|/build.*||')"

    if printf '%s' "$knl" | grep -q 'EXISTS'; then
        ok=$((ok + 1))
    else
        broken+=("${p}|${knl_key:-<unresolved>}|kernel")
    fi
    printf '%s' "$cnt" | grep -q 'EXISTS' || noinfo+=("${p}|${cnt_key:-<unresolved>}")
done

restore_target

# ── Report ────────────────────────────────────────────────────────────────────
printf '\n%s\n' "Target key check — ${ok} resolving, ${#broken[@]} broken, ${#noinfo[@]} without contrib libs"

if [ "${#broken[@]}" -gt 0 ]; then
    printf '\n  Kernel key has no directory (cannot link):\n'
    for b in "${broken[@]}"; do printf '    %-46s %s\n' "${b%%|*}" "$(printf '%s' "$b" | cut -d'|' -f2)"; done
fi
if [ "$MODE" = "list" ] && [ "${#noinfo[@]}" -gt 0 ]; then
    printf '\n  No contrib libraries (informational — not every os-arch has them):\n'
    for n in "${noinfo[@]}"; do printf '    %-46s %s\n' "${n%%|*}" "${n##*|}"; done
fi

current="$(printf '%s\n' "${broken[@]+"${broken[@]}"}" | cut -d'|' -f1 | grep -v '^$' | sort -u)"

if [ "$MODE" = "update" ]; then
    { echo "# known-broken-keys.txt — targets whose kernel key has no directory."
      echo "# Regenerate: scripts/software-utilities/check_target_keys.sh --update-baseline"
      echo "# A target here is already broken and does not fail the check. Removing a"
      echo "# target from this file is how you assert it must stay fixed."
      echo "# Generated $(date -u '+%Y-%m-%d %H:%M UTC')"
      echo
      printf '%s\n' "$current"
    } > "$BASELINE"
    echo
    echo "  Baseline updated: ${BASELINE} ($(printf '%s\n' "$current" | grep -c .) targets)"
    exit 0
fi
[ "$MODE" = "list" ] && exit 0

# ── Compare against the baseline ──────────────────────────────────────────────
if [ ! -f "$BASELINE" ]; then
    echo
    echo "  No baseline at ${BASELINE} — run with --update-baseline to create one."
    exit 1
fi
known="$(grep -v '^\s*#' "$BASELINE" | grep -v '^\s*$' | sort -u)"
new="$(comm -23 <(printf '%s\n' "$current") <(printf '%s\n' "$known"))"
fixed="$(comm -13 <(printf '%s\n' "$current") <(printf '%s\n' "$known"))"

if [ -n "$fixed" ]; then
    printf '\n  Now resolving (remove from the baseline to lock the fix in):\n'
    printf '%s\n' "$fixed" | sed 's/^/    /'
fi
if [ -n "$new" ]; then
    printf '\n  NEW breakage — these resolved before and do not now:\n'
    printf '%s\n' "$new" | sed 's/^/    /'
    printf '\n'
    exit 1
fi
printf '\n  No new breakage.\n'
exit 0
