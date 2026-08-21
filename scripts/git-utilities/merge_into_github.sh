#!/usr/bin/env bash
# ==============================================================================
# merge_into_github.sh — thin wrapper around publish_set.py
#
# SCOPE — none of its own. It maps three old menu letters onto publish_set.py
# modes and execs it, so the outputs are whatever that mode produces (see its
# header). Covers ert-components only, so it cannot complete a publish.
# Use publish_release.sh.
#
# SUPERSEDED. The published set is no longer enumerated by hand in
# github_merge.list; it is derived from the allow-list
# scripts/git-utilities/publish_set.txt. See publish_set.py.
#
# This wrapper survives because the menu is in people's fingers. It maps the old
# letters onto the new modes and adds nothing of its own.
#
#   a  →  --merge   meld each hand-merge path that differs, one at a time
#   b  →  --sync    apply (asks for confirmation, then passes --yes)
#   c  →  --diff    dry run: what would change, plus what needs hand-merging
#   d  →  --plan    the derived path set
#   k  →  --check   validate the allow-list and the leak checks only
#
# Why the old list went away: it had drifted to the point where whole os-arch
# families (Zephyr, xcore, QNX, macOS, Qt) were missing from it while four of its
# entries named platforms that no longer existed — and being a flat list it could
# not express the one check that matters most, which is that a generically-named
# target must not drag a customer devman-config into the public mirror.
# ==============================================================================

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "${SCRIPT_DIR}/../.." && pwd)"
TOOL="${SCRIPT_DIR}/publish_set.py"

[ -f "$TOOL" ] || { echo "publish_set.py not found next to this script" >&2; exit 1; }
cd "$ROOT_DIR" || exit 1

# Pass-through: anything that looks like a flag goes straight to the tool.
if [ $# -gt 0 ]; then
    exec python3 "$TOOL" "$@"
fi

cat <<'MENU'

=== ERT-Components → GitHub Mirror Sync ===

  a)  Meld review  — open meld for each hand-merge path that differs
  b)  Sync all     — apply the derived set (rsync --delete)
  c)  Diff summary — dry run, no changes made
  d)  Plan         — print the derived path set
  k)  Check only   — validate the allow-list and the leak checks
  q)  Quit

MENU
read -rp "Choice [a/b/c/d/k/q]: " choice
echo

case "$choice" in
    a|A) exec python3 "$TOOL" --merge ;;
    c|C) exec python3 "$TOOL" --diff  ;;
    d|D) exec python3 "$TOOL" --plan  ;;
    k|K) exec python3 "$TOOL" --check ;;
    b|B)
        echo "--sync uses rsync --delete: anything in the mirror outside the derived"
        echo "set is removed. Run 'c' first and read it — as of the last check that"
        echo "would remove 53 of the mirror's 75 platform directories, because the"
        echo "allow-list is still narrower than what the mirror actually publishes."
        echo
        read -rp "Type 'sync' to proceed: " confirm
        [ "$confirm" = "sync" ] || { echo "Aborted."; exit 0; }
        exec python3 "$TOOL" --sync --yes
        ;;
    q|Q) echo "Quit."; exit 0 ;;
    *)   echo "Unknown choice '$choice'." >&2; exit 1 ;;
esac
