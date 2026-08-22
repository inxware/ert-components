#!/usr/bin/env bash
#---------------------------------------------------------------
# Copyright (C) 2026, inx limited, UK.
# All Rights Reserved.  LGPLv3 — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# audit_test_apps.sh — classify every Lucid test app by whether it can actually
#                      be asserted, without running anything.
#
# SCOPE — static. No build, no target, no Docker; it reads the exported SODL and
# the directory beside it. run_lucid_apps.sh answers "did it work"; this answers
# the prior question "is this app a test at all".
#
# Input : the test app roots (Common/Components/*/*/test[s]/*, SystemTests/apps/*),
#         or a tree given with --root.
# Output: a table on stdout, and with --csv a machine-readable copy. Exits 1 if
#         any app is NEEDS-BASELINE or STALE-BASELINE.
#
#   ./SystemTests/CI/audit_test_apps.sh
#   ./SystemTests/CI/audit_test_apps.sh --root Common/Components --csv out.csv
#
# CLASSIFICATION
#
# Two independent facts: does the app's SODL write a result, and is there a
# committed baseline to compare it against.
#
#   writes  baseline  class            meaning
#   ------  --------  ---------------  ------------------------------------------
#   yes     yes       AUTOTEST         a real test; run_lucid_apps.sh --assert it
#   yes     no        NEEDS-BASELINE   reports, but nothing to compare -- capture
#                                      test_result.txt and commit it
#   no      yes       STALE-BASELINE   baseline orphaned; the app stopped
#                                      reporting, or the file is left over
#   no      no        MANUAL           interactive/demo app; load check only
#
# The baseline file is the declaration of intent: an app that ships
# test_result.txt is asserting that it produces one. That is what separates a
# test from a demo, and it needs no new marker file or naming convention.

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"

ROOTS=()
CSV=""
SHOW_MANUAL=0
while [ $# -gt 0 ]; do
    case "$1" in
        --root)   ROOTS+=("${2:-}"); shift 2 ;;
        --csv)    CSV="${2:-}"; shift 2 ;;
        --manual) SHOW_MANUAL=1; shift ;;
        -h|--help) sed -n '6,45p' "$0"; exit 0 ;;
        *) echo "unknown option: $1" >&2; exit 2 ;;
    esac
done
if [ "${#ROOTS[@]}" -eq 0 ]; then
    ROOTS=("${ROOT}/Common/Components" "${ROOT}/SystemTests/apps")
fi

source "${ROOT}/scripts/build-function-library/colour.sh"
note() { printf '   %s\n' "$*"; }
bad()  { printf '   %s%s%s\n' "${TXT_FG_BRIGHT_RED}"    "$*" "${TXT_RESET}"; }
warnl(){ printf '   %s%s%s\n' "${TXT_FG_BRIGHT_YELLOW}" "$*" "${TXT_RESET}"; }
good() { printf '   %s%s%s\n' "${TXT_FG_GREEN}"         "$*" "${TXT_RESET}"; }

n_auto=0; n_needs=0; n_stale=0; n_manual=0; n_noexport=0
declare -A EXPC
ROWS=""

printf '\n%s── audit_test_apps%s\n' "${TXT_BOLD}${TXT_FG_BRIGHT_WHITE}" "${TXT_RESET}"

for r in "${ROOTS[@]}"; do
    [ -d "$r" ] || continue
    while read -r lpj; do
        [ -n "$lpj" ] || continue
        app="$(dirname "$lpj")"
        name="${app#${ROOT}/}"

        # Prefer a v1 export; fall back to the v0 one purely to read strings.
        sodl=""
        for d in export-ert1 export export-ert0; do
            [ -f "${app}/${d}/t.sdl" ] && { sodl="${app}/${d}/t.sdl"; break; }
        done

        # Runnability is a separate axis from assertability. The kernel loads
        # only t.sdl (EHS_DEFAULT_SODL_NAME), so a v1 SODL sitting in
        # DeviceData/SODL/ert1.sdl is Lucid's working copy and not deployable --
        # but that app is one export away from working, unlike a v0-only one.
        exp="none"
        for d in export-ert1 export export-ert0; do
            [ -f "${app}/${d}/t.sdl" ] || continue
            if [ "$(od -An -tx1 -N1 "${app}/${d}/t.sdl" 2>/dev/null | tr -d ' ')" = "02" ]; then
                exp="v1"; break
            fi
            exp="v0-only"
        done
        [ "$exp" = "v0-only" ] && [ -f "${app}/DeviceData/SODL/ert1.sdl" ] && exp="v1-unexported"

        base="no"; [ -f "${app}/test_result.txt" ] && base="yes"

        if [ -z "$sodl" ]; then
            writes="?"; class="NO-EXPORT"; n_noexport=$((n_noexport + 1))
        else
            writes="no"
            strings "$sodl" 2>/dev/null | grep -q 'test_result.txt' && writes="yes"
            if   [ "$writes" = yes ] && [ "$base" = yes ]; then class="AUTOTEST";       n_auto=$((n_auto + 1))
            elif [ "$writes" = yes ] && [ "$base" = no  ]; then class="NEEDS-BASELINE"; n_needs=$((n_needs + 1))
            elif [ "$writes" = no  ] && [ "$base" = yes ]; then class="STALE-BASELINE"; n_stale=$((n_stale + 1))
            else                                                class="MANUAL";         n_manual=$((n_manual + 1))
            fi
        fi
        ROWS="${ROWS}${class}|${writes}|${base}|${exp}|${name}"$'\n'
        EXPC[$exp]=$(( ${EXPC[$exp]:-0} + 1 ))
    done < <(find "$r" -name '*.lpj' -not -path '*/DEP/*' 2>/dev/null | sort)
done

_emit() {  # $1 = class, $2 = colour fn
    local cls="$1" fn="$2" any=0
    while IFS='|' read -r c w b e n; do
        [ "$c" = "$cls" ] || continue
        [ "$any" -eq 0 ] && { printf '\n'; note "${cls}:"; any=1; }
        $fn "   ${n}"
    done <<< "$ROWS"
}

_emit NEEDS-BASELINE warnl
_emit STALE-BASELINE bad
_emit NO-EXPORT      bad
_emit AUTOTEST       good
[ "$SHOW_MANUAL" -eq 1 ] && _emit MANUAL note

printf '\n'
printf '   %-16s %5s   %s\n' "class" "count" "action"
printf '   %-16s %5s   %s\n' "----------------" "-----" "--------------------------------------"
printf '   %-16s %5d   %s\n' "AUTOTEST"       "$n_auto"     "asserted by run_lucid_apps.sh --assert"
printf '   %-16s %5d   %s\n' "NEEDS-BASELINE" "$n_needs"    "capture test_result.txt and commit it"
printf '   %-16s %5d   %s\n' "STALE-BASELINE" "$n_stale"    "app stopped reporting, or leftover file"
printf '   %-16s %5d   %s\n' "MANUAL"         "$n_manual"   "load check only (--manual to list)"
printf '   %-16s %5d   %s\n' "NO-EXPORT"      "$n_noexport" "no SODL exported from Lucid"

# Runnability. A v0-only export cannot be parsed by any ERT_SODL_VERSION=1
# target at all, so these apps are dormant regardless of their class above.
printf '\n'
printf '   %-16s %5s   %s\n' "export" "count" "meaning"
printf '   %-16s %5s   %s\n' "----------------" "-----" "--------------------------------------"
printf '   %-16s %5d   %s\n' "v1"            "${EXPC[v1]:-0}"            "runnable on a SODL v1 target"
printf '   %-16s %5d   %s\n' "v1-unexported" "${EXPC[v1-unexported]:-0}" "v1 in DeviceData/SODL; re-export from Lucid"
printf '   %-16s %5d   %s\n' "v0-only"       "${EXPC[v0-only]:-0}"       "deprecated format; needs migrating to ERT1"
printf '   %-16s %5d   %s\n' "none"          "${EXPC[none]:-0}"          "never exported"

if [ -n "$CSV" ]; then
    { printf 'class,writes_result,has_baseline,export,app\n'
      while IFS='|' read -r c w b e n; do
          [ -n "$c" ] && printf '%s,%s,%s,%s,"%s"\n' "$c" "$w" "$b" "$e" "$n"
      done <<< "$ROWS"
    } > "$CSV"
    note "csv: ${CSV}"
fi

printf '\n'
if [ "$((n_needs + n_stale))" -eq 0 ]; then
    good "every reporting app has a baseline"
    exit 0
fi
warnl "$((n_needs + n_stale)) app(s) cannot be asserted as they stand."
exit 1
