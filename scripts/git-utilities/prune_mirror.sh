#!/usr/bin/env bash
#---------------------------------------------------------------
# Copyright (C) 2026, inx limited, UK.
# All Rights Reserved.  LGPLv3 — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# prune_mirror.sh — remove directories from the public mirror that the
#                   published set does not include.
#
# SCOPE — withdrawal only. It adds and updates nothing; publish_release.sh does
# that.
#
# Input : the derived plan (publish_set.py) and the mirror checkout at
#         ../../inxware-github/ert-components
# Output: reports every directory in the mirror that is not in the plan, and
#         with --apply deletes it from the mirror working tree. Commits nothing
#         and pushes nothing.
#
# WHY THIS IS NEEDED SEPARATELY
#
# publish_release.sh rsyncs each published platform individually --
# target/platform/<name>/ , target/os-arch/<name>/ -- so its --delete only
# removes files *inside* a directory it is syncing. Nothing rsyncs the parent,
# so a directory the plan no longer includes is never considered: the sync can
# add and update platforms but cannot withdraw one.
#
# That is how customer-named platforms published before the allow-list existed
# stayed in the mirror through every later sync. publish_set.py --check does not
# catch it either: it validates the source plan, not the mirror's contents.
#
# USAGE
#
#   ./scripts/git-utilities/prune_mirror.sh            # report only
#   ./scripts/git-utilities/prune_mirror.sh --apply    # delete from the mirror
#   ./scripts/git-utilities/prune_mirror.sh --private  # restrict to names matching
#                                                      # the private regex, and
#                                                      # nothing else. Combine with
#                                                      # --apply to remove the
#                                                      # customer leak on its own.
#
# Deleting removes a directory from the mirror's HEAD. It stays in the
# repository's history and remains fetchable, so if the concern is disclosure
# rather than tidiness, the history needs rewriting as well — a separate and
# much larger decision.

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
GH="$(cd "${ROOT}/../../inxware-github" 2>/dev/null && pwd)"
PY="${SCRIPT_DIR}/publish_set.py"
MIRROR="${GH}/ert-components"

# Parents whose children the plan lists individually. These are the ones rsync
# --delete cannot reach.
PARENTS=(target/platform target/os-arch)

# Kept regardless: not platforms, and not something the plan enumerates.
KEEP='^(ARCHIVE|base_full|base_small|.*_ALL)$'

DRY=1
PRIVATE_ONLY=0
while [ $# -gt 0 ]; do
    case "$1" in
        --apply)   DRY=0 ;;
        --private) PRIVATE_ONLY=1 ;;
        -h|--help) sed -n '2,41p' "$0"; exit 0 ;;
        *) echo "unknown option: $1" >&2; exit 2 ;;
    esac
    shift
done

source "${ROOT}/scripts/build-function-library/colour.sh"
note() { printf '   %s\n' "$*"; }
bad()  { printf '   %s%s%s\n' "${TXT_FG_BRIGHT_RED}"    "$*" "${TXT_RESET}"; }
warnl(){ printf '   %s%s%s\n' "${TXT_FG_BRIGHT_YELLOW}" "$*" "${TXT_RESET}"; }
good() { printf '   %s%s%s\n' "${TXT_FG_GREEN}"         "$*" "${TXT_RESET}"; }

[ -d "$MIRROR/.git" ] || { echo "mirror not found at ${MIRROR}" >&2; exit 1; }

# The private pattern comes from publish_set.py so there is one definition.
PRIVATE_RE="$(python3 - "$PY" <<'PYEOF'
import importlib.util, sys
spec = importlib.util.spec_from_file_location("ps", sys.argv[1])
m = importlib.util.module_from_spec(spec); spec.loader.exec_module(m)
print(m.PRIVATE.pattern)
PYEOF
)"

printf '\n%s── prune_mirror%s\n' "${TXT_BOLD}${TXT_FG_BRIGHT_WHITE}" "${TXT_RESET}"
[ "$DRY" -eq 1 ] && bad "REPORT ONLY — nothing will be deleted. Add --apply to act."
note "mirror: ${MIRROR}"
printf '\n'
note "Listed = in the mirror, not in the allow-list. All of it goes with --apply."
bad   "  red    the name also matches the private regex in publish_set.py"
warnl "  yellow it does not"
note "The regex only highlights; it is not what selects. Anything absent from the"
note "allow-list is deleted either way — so read the yellow list too."

PLAN="$(python3 "$PY" --plan 2>/dev/null)" || {
    echo "could not derive the published plan" >&2; exit 1; }

total_extra=0; total_private=0
for parent in "${PARENTS[@]}"; do
    [ -d "${MIRROR}/${parent}" ] || continue
    wanted="$(printf '%s\n' "$PLAN" | grep -oE "${parent}/[^/]+/" \
              | sed "s|${parent}/||; s|/$||" | sort -u)"
    extra=""; private=""
    for d in "${MIRROR}/${parent}"/*/; do
        [ -d "$d" ] || continue
        b="$(basename "$d")"
        printf '%s\n' "$wanted" | grep -qx "$b" && continue
        echo "$b" | grep -qE "$KEEP" && continue
        if echo "$b" | grep -qiE "$PRIVATE_RE"; then
            private="${private} ${b}"
        else
            extra="${extra} ${b}"
        fi
    done

    _np=$(printf '%s' "$private" | wc -w); _ne=$(printf '%s' "$extra" | wc -w)
    total_private=$((total_private + _np)); total_extra=$((total_extra + _ne))
    printf '\n'
    note "${parent}: ${_np} red, ${_ne} yellow"

    for group in private extra; do
        [ "$group" = extra ] && [ "$PRIVATE_ONLY" -eq 1 ] && continue
        eval "names=\$$group"
        [ -n "$names" ] || continue
        for b in $names; do
            if [ "$DRY" -eq 0 ]; then
                rm -rf "${MIRROR}/${parent}/${b}"
                [ "$group" = private ] && bad "   DELETED (customer): ${b}" \
                                       || warnl "   deleted: ${b}"
            else
                [ "$group" = private ] && bad "   would DELETE (customer): ${b}" \
                                       || warnl "   would delete: ${b}"
            fi
        done
    done
done

# ── Explicitly excluded paths ────────────────────────────────────────────────
# Two separate mechanisms, and the plan comparison above only covers the first:
#   * the allow-list decides which platforms are published (handled above)
#   * NEVER_PUBLISH in publish_set.py names paths that must never travel, even
#     inside a parent that is published as a whole tree
# NEVER_PUBLISH is an rsync --exclude, and rsync protects excluded paths from
# --delete, so anything already in the mirror stays there. That is what this
# pass is for.
printf '\n'
_nev=0
if [ "$PRIVATE_ONLY" -eq 1 ]; then
    note "explicitly excluded: skipped (--private restricts to private names only)"
else
note "explicitly excluded (NEVER_PUBLISH in publish_set.py):"
# NEVER_PUBLISH holds rsync patterns in three forms, and find needs a different
# predicate for each. Matching them all with -name silently missed every
# slash-containing entry, which is how the customer CI projects reached the
# mirror despite being listed.
while read -r _pat; do
    [ -n "$_pat" ] || continue
    _name="${_pat%/}"
    case "$_pat" in
        /*)   # anchored to a transfer root. Transfer roots are the plan's own
              # entries, so in mirror terms that is the mirror root or one level
              # below it — NOT any depth, or this would delete paths rsync
              # happily republishes (e.g. a deep .../build/reports).
              _find=(-path "${MIRROR}${_name}" -o -path "${MIRROR}/*${_name}" -a -not -path "${MIRROR}/*/*${_name}") ;;
        */*)  # matches the end of a relative path
              _find=(-path "*/${_name}") ;;
        *)    # bare name, any depth
              _find=(-name "${_name}") ;;
    esac
    while read -r _hit; do
        [ -n "$_hit" ] || continue
        _nev=$((_nev + 1))
        if [ "$DRY" -eq 0 ]; then
            rm -rf "$_hit"; bad "   DELETED: ${_hit#${MIRROR}/}"
        else
            bad "   would DELETE: ${_hit#${MIRROR}/}"
        fi
    done <<EOF
$(find "$MIRROR" -path "$MIRROR/.git" -prune -o \( "${_find[@]}" \) -print 2>/dev/null)
EOF
done <<EOF
$(python3 - "$PY" <<'PYEOF'
import importlib.util, sys
spec = importlib.util.spec_from_file_location("ps", sys.argv[1])
m = importlib.util.module_from_spec(spec); spec.loader.exec_module(m)
print("\n".join(m.NEVER_PUBLISH))
PYEOF
)
EOF
[ "$_nev" -eq 0 ] && good "   none present"
fi
true

printf '\n'
if [ "$((total_private + total_extra + _nev))" -eq 0 ]; then
    good "mirror matches the published set — nothing to prune."
else
    note "total: ${total_private} red, ${total_extra} yellow, ${_nev} explicitly excluded"
    if [ "$DRY" -eq 1 ]; then
        bad "Nothing was deleted. Re-run with --apply."
    else
        note "Deleted from the mirror working tree. Review, then commit and push:"
        note "    git -C ${MIRROR} status"
        warnl "History still holds them — deleting from HEAD does not unpublish."
    fi
fi
