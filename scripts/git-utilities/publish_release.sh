#!/usr/bin/env bash
#---------------------------------------------------------------
# Copyright (C) 2026, inx limited, UK.
# All Rights Reserved.  LGPLv3 — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# publish_release.sh — one pass of the whole community publish.
#
# SCOPE — the whole publish, across five repositories. Calls publish_set.py for
# the ert-components half and does the rest itself.
#
# Input : --tag <name>, the internal working trees, and the mirror checkouts at
#         ../../inxware-github/
# Output: with --apply, the five mirror WORKING TREES are modified in place
#         (rsync --delete, so removals happen too) and a provenance tag is
#         written on both sides. Patches for any incoming community commits are
#         written to ../TARGET_TREES/incoming-<tag>/.
#         Nothing is committed and nothing is pushed -- that is left to you.
#         Without --apply nothing is written at all; every step only reports.
#
#   ./scripts/git-utilities/publish_release.sh
#
# Steps, in this order for a reason:
#
#   0  pre-flight        publish_set.py --check — refuse early if the allow-list
#                        can reach anything private
#   1  INCOMING          what changed on the public side since the last tag,
#                        exported as patches. Runs FIRST: step 3 uses
#                        rsync --delete, so anything unmerged here is destroyed.
#   2  OUTGOING merge    meld the hand-merge paths. Runs SECOND so the operator
#                        starts work immediately rather than waiting on rsync.
#   3  COPY              publish_set.py --sync, and tag both repos
#   4  DEPENDENCY REPOS  prune ert-build-support and ert-contrib-middleware to
#                        what the published set needs, then copy those across
#   5  KERNEL REPO       ert-kernels, pruned to the keys the published set needs
#   6  APPS              demos, tutorials, systemapps -- top-level dirs only
#   7  COMMIT/TAG/PUSH   per repo, with confirmation and an optional review.
#                        Dependencies first, ert-components last: pushing it
#                        starts the GitHub Actions build, which needs the others
#                        already in place. Tags both sides after a successful
#                        push.
#
# Provenance is the tag, in both repos. Deliberately not a metadata file: a
# dotfile in the mirror is invisible in `git log` and in the GitHub UI, and will
# not be maintained.
#
# OPTIONS
#
#   --tag <name>   Provenance tag applied to both sides. Optional: without it the
#                  next free merge-to-github-NNN in this repo is used. Step 1
#                  diffs the mirror against the PREVIOUS tag of the same pattern
#                  to find incoming changes.
#   --apply        Act. Without it every step is a dry run and prints what it
#                  would do. Default is dry run, deliberately.
#   --skip-meld    Do not open meld in step 2. Useful for a first look.
#
# BEFORE YOU RUN
#
#   * Every checkout must be on its default branch (master internally, main on
#     the mirrors). Step 0 refuses on --apply otherwise: a mirror on a feature
#     branch publishes to that branch, not the one GitHub serves.
#   * Mirror checkouts must be clean. A detached HEAD or an interrupted rebase
#     will be rsynced over.
#   * ../apps must be clean, or targetenv refuses the RELEASE-PRODUCTION switch.
#   * An untagged mirror is assumed to be current — there is nothing to diff
#     against. Tagging starts from the first --apply. Until then, check by hand
#     for commits made directly on a mirror: step 3 deletes.
#
# Steps 0-6 only write files. Step 7 commits, pushes and tags, asking per repo
# and offering a GUI review first. Nothing leaves this machine before then.
#
# Full operator guide: docs/community-github-mirror.md

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
GH="$(cd "${ROOT}/../../inxware-github" 2>/dev/null && pwd)"
PY="${SCRIPT_DIR}/publish_set.py"
TAG=""; DRY=1; SKIP_MELD=0
TAG_PREFIX="merge-to-github-"

while [ $# -gt 0 ]; do
    case "$1" in
        --tag)        TAG="${2:-}"; TAG_GIVEN=1; shift 2 ;;
        --apply)      DRY=0; shift ;;
        --skip-meld)  SKIP_MELD=1; shift ;;
        -h|--help)    sed -n '2,51p' "$0"; exit 0 ;;
        *) echo "Unknown option: $1" >&2; exit 2 ;;
    esac
done

# Default the tag to the next free number in this repo. Numbering follows the
# internal repo, not the mirror: the mirror's tags are written by this script
# and can be missing if a publish was reviewed but never committed.
if [ -z "$TAG" ]; then
    _last=$(git -C "$ROOT" tag --list "${TAG_PREFIX}[0-9]*" \
            | sed "s|^${TAG_PREFIX}||" | grep -E '^[0-9]+$' | sort -n | tail -1)
    # 10# so a zero-padded number is not read as octal.
    TAG=$(printf "${TAG_PREFIX}%03d" $(( 10#${_last:-0} + 1 )))
fi

case "$TAG" in
    *[!A-Za-z0-9._/-]*) echo "tag contains characters git will reject: ${TAG}" >&2; exit 2 ;;
esac
if git -C "$ROOT" rev-parse -q --verify "refs/tags/${TAG}" >/dev/null; then
    echo "tag already exists in $(basename "$ROOT"): ${TAG}" >&2
    echo "  pass a different --tag, or delete it first." >&2
    exit 2
fi
[ -d "$GH" ]    || { echo "mirror checkout not found at ../../inxware-github" >&2; exit 1; }
cd "$ROOT" || exit 1

# Shared palette: honours NO_COLOR and drops colour when stdout is not a tty,
# so redirected output stays plain. Provides heading/warn/err.
# shellcheck source=../build-function-library/colour.sh
source "${ROOT}/scripts/build-function-library/colour.sh"

hdr()  { printf '\n%s── %s%s\n' "${TXT_BOLD}${TXT_FG_BRIGHT_WHITE}" "$*" "${TXT_RESET}"; }
note() { printf '   %s\n' "$*"; }
# Colour carries severity, so the eye finds the destructive lines first:
#   red    something will be deleted, or a step failed
#   yellow needs a human before --apply
#   green  done, or nothing to do
#   plain  informational
bad()  { printf '   %s%s%s\n' "${TXT_FG_BRIGHT_RED}"    "$*" "${TXT_RESET}"; }
warnl(){ printf '   %s%s%s\n' "${TXT_FG_BRIGHT_YELLOW}" "$*" "${TXT_RESET}"; }
good() { printf '   %s%s%s\n' "${TXT_FG_GREEN}"         "$*" "${TXT_RESET}"; }
[ "$DRY" -eq 1 ] && bad "DRY RUN — no changes will be written. Add --apply to act."
if [ -n "${_last:-}" ]; then
    note "tag: ${TAG}  (next after ${TAG_PREFIX}${_last})"
elif [ -z "${TAG_GIVEN:-}" ]; then
    note "tag: ${TAG}  (first — no ${TAG_PREFIX}* tag in this repo yet)"
else
    note "tag: ${TAG}"
fi

# ── 0. Pre-flight ─────────────────────────────────────────────────────────────
hdr "0. Pre-flight"

# Every checkout must be on its default branch. A mirror left on a feature
# branch publishes there instead of to the branch GitHub serves, and an internal
# repo on one means the tree being published is not what master holds. The
# default is read from origin/HEAD rather than assumed, because the internal
# repos use master and the mirrors use main.
_branch_bad=""
for _spec in "${ROOT}|internal ert-components" \
             "${ROOT}/../EHS-kernel|internal EHS-kernel" \
             "${ROOT}/../ert-contrib-middleware|internal ert-contrib-middleware" \
             "${ROOT}/../ert-kernels|internal ert-kernels" \
             "${ROOT}/../apps|internal apps" \
             "${GH}/ert-components|mirror ert-components" \
             "${GH}/ert-contrib-middleware|mirror ert-contrib-middleware" \
             "${GH}/ert-build-support|mirror ert-build-support" \
             "${GH}/ert-kernels|mirror ert-kernels" \
             "${GH}/apps|mirror apps"; do
    _d="${_spec%%|*}"; _label="${_spec##*|}"
    [ -d "${_d}/.git" ] || continue
    _on="$(git -C "$_d" rev-parse --abbrev-ref HEAD 2>/dev/null)"
    _def="$(git -C "$_d" symbolic-ref --short refs/remotes/origin/HEAD 2>/dev/null | sed 's|^origin/||')"
    # No origin/HEAD (a fresh or local-only clone): accept either convention.
    if [ -z "$_def" ]; then
        case "$_on" in master|main) continue ;; esac
        _def="master or main"
    fi
    [ "$_on" = "$_def" ] && continue
    _branch_bad="${_branch_bad}\n      ${_label}: on '${_on}', expected '${_def}'"
done
if [ -n "$_branch_bad" ]; then
    warnl "not on the default branch:"
    printf '%b\n' "$_branch_bad"
    if [ "$DRY" -eq 0 ]; then
        err "Refusing to publish. A mirror on a feature branch publishes to that"
        err "branch, not the one GitHub serves. Switch them, or merge and delete"
        err "the branch first."
        exit 1
    fi
    warnl "(--apply would refuse until these are on their default branch)"
fi
python3 "$PY" --check || { echo "   allow-list check failed — stopping." >&2; exit 1; }

# ── 1. Incoming ───────────────────────────────────────────────────────────────
# The three mirrors are read for community commits since the previous tag. Only
# ert-components is expected to receive any; the other two are listed so a
# surprise is visible rather than silently overwritten in step 4.
hdr "1. Incoming — commits made on the public side, not yet in the internal repos"
note "Each of the five mirror repos is compared against its own previous"
note "${TAG_PREFIX}* tag. Anything found is exported as patches for you to apply"
note "internally FIRST — step 3 copies with --delete and would destroy it."
PATCH_DIR="${ROOT}/../TARGET_TREES/incoming-${TAG}"
_untagged=""
_quiet=""
_nocheckout=""
for repo in ert-components ert-contrib-middleware ert-build-support ert-kernels apps; do
    d="${GH}/${repo}"
    [ -d "$d/.git" ] || { _nocheckout="${_nocheckout:-} ${repo}"; continue; }
    prev="$(git -C "$d" tag --list "${TAG_PREFIX}*" --sort=-v:refname | head -1)"
    # An untagged mirror is treated as already up to date. Tagging starts from
    # this publish, so there is nothing to diff against and nothing to import.
    [ -z "$prev" ] && { _untagged="${_untagged} ${repo}"; continue; }

    n="$(git -C "$d" rev-list --count "${prev}..HEAD" 2>/dev/null || echo 0)"
    [ "$n" = "0" ] && { _quiet="${_quiet} ${repo}"; continue; }

    warnl "repo ${repo}: ${n} commit(s) on the mirror since ${prev} — NOT in the internal repo"
    git -C "$d" log --oneline "${prev}..HEAD" | sed 's/^/       /'
    if [ "$DRY" -eq 0 ]; then
        mkdir -p "${PATCH_DIR}/${repo}"
        git -C "$d" format-patch -o "${PATCH_DIR}/${repo}" "${prev}..HEAD" >/dev/null
        note "       patches → ${PATCH_DIR}/${repo}"
    fi
    warnl "      ⚠ apply these to the internal repo BEFORE step 3 — --delete will destroy them"
done
[ -n "$_quiet" ]    && good "repos with nothing new: $(printf '%s' "$_quiet" | sed 's/^ *//; s/ /, /g')"
[ -n "$_untagged" ] && warnl "repos never tagged, so assumed up to date: $(printf '%s' "$_untagged" | sed 's/^ *//; s/ /, /g')"
[ -n "$_nocheckout" ] && warnl "repos not checked out, skipped: $(printf '%s' "$_nocheckout" | sed 's/^ *//; s/ /, /g')"
true

# ── 2. Outgoing merge ─────────────────────────────────────────────────────────
hdr "2. Hand-merge paths (meld)"
if [ "$SKIP_MELD" -eq 1 ]; then
    note "skipped (--skip-meld)"
elif [ "$DRY" -eq 1 ]; then
    # Preview only. --apply opens meld on each of these in turn.
    python3 "$PY" --diff 2>/dev/null | sed -n '/hand-merge path(s) differ/,$p' | sed 's/^/   /'
    note "(--apply opens meld on each of these)"
else
    python3 "$PY" --merge
fi

# ── 3. Copy ───────────────────────────────────────────────────────────────────
# Copy order does not matter — nothing is visible to GitHub until step 7 pushes,
# and step 7 deliberately pushes ert-components last.
hdr "3. Publish ert-components"
if [ "$DRY" -eq 1 ]; then
    python3 "$PY" --diff | sed -n '/path(s) would change/,$p' | head -2
else
    # No --tag here: step 7 tags, after a successful push.
    python3 "$PY" --sync --yes || exit 1
fi

# ── 4. Dependency repos ───────────────────────────────────────────────────────
# Derived from the published set, not hand-listed: publish_set.py --deps resolves
# each published target's EHS_GNU_OS_ARCH, TOOLCHAIN_PATH and
# COMPONENT_BASE_TECHNOLOGIES exactly as platform.mk does.
hdr "4. Dependency repos (ert-build-support, ert-contrib-middleware)"
note "Writes to the MIRROR only — the internal repos are read, never modified."
note "Each directory below is either copied to the mirror, absent here so nothing"
note "is copied, or present in the mirror but not needed and so deleted from it."

# Scope is deliberately narrow (Pierre): prune only inside the directories the
# derivation covers. Everything else at the top level of those repos —
# contrib/ (5.7 GB of third-party source, deliberately published),
# inx_build_scripts/, sys-executables/, README, LICENSE — is left untouched.
# toolchains/i686 is also left alone: no published target reaches it, but it is
# outside the derived scope and deleting it is a separate decision.
DEPS_TSV="$(python3 "$PY" --deps --porcelain)" || { echo "   deps derivation failed" >&2; exit 1; }

# support_libs/target_libs is deliberately not in this list. platform.mk still
# reads it for a target's libc include/lib path (build/include, build/lib), but
# nothing populates it any more: cross targets get their libraries from the
# Docker image, and the kernel moved to ert-kernels. Every target resolves it as
# N/A, and only one directory in it still holds content — one no published
# target asks for. Add it back if a target ever needs a vendored sysroot again.
for spec in "ert-build-support|toolchains/x86_64" \
            "ert-contrib-middleware|target_libs"; do
    repo="${spec%%|*}"; sub="${spec##*|}"
    src="${ROOT}/../${repo}/${sub}"
    dst="${GH}/${repo}/${sub}"
    [ -d "$src" ] || { note "${repo}/${sub}: absent internally, skipped"; continue; }

    # The derivation emits toolchains as "x86_64/<name>"; strip the arch prefix
    # so it matches the directory we are pruning.
    want="$(printf '%s\n' "$DEPS_TSV" \
            | awk -F'\t' -v r="$repo" -v s="${sub%%/x86_64}" '$1==r && $2==s {print $3}' \
            | sed 's|^x86_64/||' | sort -u)"

    keep=0; miss=0; drop=0; missing_names=""; kept_names=""; drop_names=""; pub_names=""
    note "${repo}/${sub}"
    for n in $want; do
        if [ -d "${src}/${n}" ]; then
            keep=$((keep+1)); pub_names="${pub_names} ${n}"
            [ "$DRY" -eq 0 ] && rsync -a --mkpath --delete "${src}/${n}/" "${dst}/${n}/"
        else
            miss=$((miss+1)); missing_names="${missing_names} ${n}"
        fi
    done
    if [ -d "$dst" ]; then
        for have in "$dst"/*/; do
            [ -d "$have" ] || continue
            b="$(basename "$have")"
            printf '%s\n' "$want" | grep -qx "$b" && continue
            # A DEPENDENTS.md marks a directory reached by a build or utility
            # script rather than by any platform config — the derivation cannot
            # see those. Deleting them is how the first prune attempt nearly
            # removed espressif-4.4.x and xtensa-esp32s3-elf-4.4.1.
            if [ -f "${src}/${b}/DEPENDENTS.md" ]; then
                kept_names="${kept_names} ${b}"; pub_names="${pub_names} ${b}"
                [ "$DRY" -eq 0 ] && rsync -a --mkpath --delete "${src}/${b}/" "${dst}/${b}/"
                keep=$((keep+1))
                continue
            fi
            drop=$((drop+1)); drop_names="${drop_names} ${b}"
            [ "$DRY" -eq 0 ] && rm -rf "$have"
        done
    fi
    note "      ${keep} to copy, ${miss} absent here, ${drop} to delete from mirror"
    [ -n "$pub_names" ] && good "      $([ "$DRY" -eq 0 ] && echo 'COPIED to mirror' || echo 'would COPY to mirror') (${keep}):${pub_names}"
    # Absent is normal: the directory is supplied by a Docker image, or the
    # os-arch has no libraries of that kind. Nothing is copied and nothing in the
    # mirror is touched for these.
    [ -n "$missing_names" ] && note "      absent in this checkout, nothing copied (${miss}):${missing_names}"
    [ -n "$kept_names" ]    && note "      (of those copied, these are referenced by scripts not configs — see DEPENDENTS.md):${kept_names}"
    [ -n "$drop_names" ]    && bad  "      $([ "$DRY" -eq 0 ] && echo 'DELETED FROM MIRROR' || echo 'would DELETE FROM MIRROR') (${drop}):${drop_names}"
    true
done

# ── 5. Kernel repo ────────────────────────────────────────────────────────────
hdr "5. Kernel repo (ert-kernels)"

# Split out of ert-build-support: 47 MB of frequently-rebuilt archives were the
# only changing content in an 8.6 GB LFS-backed tree of static toolchains. Plain
# git, no LFS. Only the keys the published set needs are mirrored — the same
# support_libs set derived in step 4.
KSRC="${ROOT}/../ert-kernels"
KDST="${GH}/ert-kernels"
if [ ! -d "$KSRC" ]; then
    note "../ert-kernels not found — nothing to publish."
elif [ ! -d "$KDST" ]; then
    note "mirror checkout ${KDST} does not exist yet."
    note "Create it (git init + remote) before the first publish; nothing done here."
else
    want="$(printf '%s\n' "$DEPS_TSV" | awk -F'\t' '$2=="support_libs/target_libs" {print $3}' | sort -u)"
    keep=0; drop=0; kdrop=""; kpub=""
    for n in $want; do
        [ -d "${KSRC}/${n}/kernel" ] || continue
        keep=$((keep+1)); kpub="${kpub} ${n}"
        [ "$DRY" -eq 0 ] && rsync -a --mkpath --delete "${KSRC}/${n}/" "${KDST}/${n}/"
    done
    for have in "$KDST"/*/; do
        [ -d "$have" ] || continue
        b="$(basename "$have")"
        printf '%s\n' "$want" | grep -qx "$b" && continue
        drop=$((drop+1)); kdrop="${kdrop} ${b}"
        [ "$DRY" -eq 0 ] && rm -rf "$have"
    done
    note "      ${keep} to copy, ${drop} to delete from mirror"
    [ -n "${kpub:-}" ] && good "      $([ "$DRY" -eq 0 ] && echo 'COPIED to mirror' || echo 'would COPY to mirror') (${keep}):${kpub}"
    [ -n "${kdrop:-}" ] && bad  "      $([ "$DRY" -eq 0 ] && echo 'DELETED FROM MIRROR' || echo 'would DELETE FROM MIRROR') (${drop}):${kdrop}"
    true

fi

# ── 6. Apps ───────────────────────────────────────────────────────────────────
# The community apps repo is a curated subset, and the curation is expressed
# purely as a top-level directory list -- no per-app allow-list, deliberately.
# The directory an app lives in is the only thing that decides whether it ships,
# which is why client work was moved to apps/demos-client/ rather than filtered
# by name.
#
# Excluded on purpose: customer-apps, demos-client, hardware-demos,
# hardware-test-apps, Education, events, "Function Block Fridays",
# legacy_demos, legacy_tutorials, "Lucid Marketing", Resources, Windows.
hdr "6. Apps (demos, tutorials, systemapps)"
note "Writes to the MIRROR only. Top-level directories not in the publish set are"
note "deleted from the mirror — that is how a client demo gets taken back down."

APPS_PUBLISH="demos tutorials systemapps"
ASRC="${ROOT}/../apps"
ADST="${GH}/apps"

if [ ! -d "$ASRC" ]; then
    note "../apps not found — nothing to publish."
elif [ ! -d "$ADST" ]; then
    note "mirror checkout ${ADST} does not exist — skipped."
else
    for d in $APPS_PUBLISH; do
        if [ ! -d "${ASRC}/${d}" ]; then
            note "      absent internally, skipped: ${d}"
            continue
        fi
        if [ "$DRY" -eq 0 ]; then
            rsync -a --mkpath --delete "${ASRC}/${d}/" "${ADST}/${d}/"
            good "      published: ${d}"
        else
            note "      would COPY to mirror: ${d} ($(find "${ASRC}/${d}" -type f 2>/dev/null | wc -l) files)"
        fi
    done

    # Anything else at the mirror's top level is either withdrawn curation or
    # drift. HRDC Demo reached the public repo this way, so report it loudly
    # rather than silently leaving it in place.
    for have in "$ADST"/*/; do
        [ -d "$have" ] || continue
        b="$(basename "$have")"
        printf '%s\n' $APPS_PUBLISH | grep -qx "$b" && continue
        if [ "$DRY" -eq 0 ]; then
            rm -rf "$have"; note "      REMOVED from mirror (not in the publish set): ${b}"
        else
            bad "      WOULD REMOVE from mirror (not in the publish set): ${b}"
        fi
    done

fi

# ── 7. Commit, tag and push ───────────────────────────────────────────────────
# The copy steps above only write files. This step is what makes a publish real,
# and it asks per repo rather than doing all five at once: a mirror is a public
# repository and the operator should see what is going out.
#
# Tags are written here, after a successful push, on BOTH sides — internal and
# mirror. Step 1 reads the mirror's tag to work out what arrived on the public
# side since last time, so a mirror that is never tagged can never report
# incoming changes.
hdr "7. Commit, tag and push"

if [ "$DRY" -eq 1 ]; then
    note "skipped on a dry run — nothing has been written to commit."
else
    # ert-components LAST. Pushing it is what starts the GitHub Actions build,
    # and that build checks out the dependency repos at their current state — so
    # if the toolchains, contrib libraries, kernel archives or apps have not
    # landed yet, the first CI run fails on a tree that is only half published.
    for _pair in "ert-contrib-middleware|${GH}/ert-contrib-middleware|${ROOT}/../ert-contrib-middleware" \
                 "ert-build-support|${GH}/ert-build-support|" \
                 "ert-kernels|${GH}/ert-kernels|${ROOT}/../ert-kernels" \
                 "apps|${GH}/apps|${ROOT}/../apps" \
                 "ert-components|${GH}/ert-components|${ROOT}"; do
        _name="${_pair%%|*}"; _rest="${_pair#*|}"
        _mir="${_rest%%|*}"; _int="${_rest#*|}"
        [ -d "${_mir}/.git" ] || { note "${_name}: no mirror checkout, skipped"; continue; }

        # Two separate questions: is there anything to commit, and is there
        # anything to push? A mirror can be clean yet hold unpushed commits from
        # an earlier run that stopped part-way, and skipping on a clean tree
        # alone strands those commits unpushed and untagged.
        _n=$(git -C "$_mir" status --porcelain | wc -l)
        _br=$(git -C "$_mir" rev-parse --abbrev-ref HEAD)
        _un=$(git -C "$_mir" rev-list --count "origin/${_br}..HEAD" 2>/dev/null || echo 0)
        if [ "$_n" -eq 0 ] && [ "$_un" -eq 0 ]; then
            good "${_name}: nothing changed and nothing unpushed"
            continue
        fi

        echo
        warnl "${_name}: ${_n} path(s) changed, ${_un} commit(s) already unpushed"
        if [ "$_n" -gt 0 ]; then
            git -C "$_mir" status --short | head -12 | sed 's/^/      /'
            [ "$_n" -gt 12 ] && note "      ... $((_n - 12)) more"
        fi
        [ "$_un" -gt 0 ] && git -C "$_mir" log --oneline "origin/${_br}..HEAD" | head -6 | sed 's/^/      /'
        true

        # Review before anything leaves the machine. What matters here is the
        # UNCOMMITTED working tree — what step 7 is about to commit — so the
        # diff is offered first. gitk shows committed history, which is a
        # different question, so it is a separate choice.
        printf '   Review %s? [d]iff / [g]ui / [Enter] skip: ' "$_name"
        read -r _ans </dev/tty || _ans=""
        case "$_ans" in
            [Dd]*)
                git -C "$_mir" diff --stat HEAD | tail -20 | sed 's/^/      /'
                printf '   Show the full diff in a pager? [y/N] '
                read -r _ans2 </dev/tty || _ans2=n
                case "$_ans2" in [Yy]*) git -C "$_mir" --no-pager diff HEAD | ${PAGER:-less} ;; esac
                ;;
            [Gg]*)
                if command -v git-gui >/dev/null 2>&1; then
                    ( cd "$_mir" && git gui >/dev/null 2>&1 )
                elif command -v gitk >/dev/null 2>&1; then
                    # -C is not enough: gitk is a separate program, so it must be
                    # started with the repo as its working directory.
                    ( cd "$_mir" && gitk --all >/dev/null 2>&1 & )
                    note "      gitk opened for ${_name} (shows committed history,"
                    note "      not the pending changes listed above)"
                else
                    warnl "      neither git-gui nor gitk is installed"
                fi
                ;;
        esac

        printf '   Commit and push %s to %s? [y/N] ' "$_name" \
               "$(git -C "$_mir" remote get-url origin 2>/dev/null)"
        read -r _ans </dev/tty || _ans=n
        case "$_ans" in
            [Yy]*) ;;
            *) warnl "      skipped — ${_name} left uncommitted for you to handle"; continue ;;
        esac

        if [ "$_n" -gt 0 ]; then
            git -C "$_mir" add -A
            git -C "$_mir" commit -q -m "Publish ${TAG}

Synced from the internal repositories by
ert-components/scripts/git-utilities/publish_release.sh." || {
                err "      commit failed for ${_name}"; continue; }
        fi

        if git -C "$_mir" push -q 2>/dev/null; then
            good "      pushed"
        else
            err "      push FAILED for ${_name} — commit is local, tag not written"
            continue
        fi

        # Tag both sides only once the push succeeded, so a tag never claims a
        # publish that did not land.
        git -C "$_mir" tag -f "$TAG" >/dev/null 2>&1 \
            && git -C "$_mir" push -q --force origin "refs/tags/${TAG}" 2>/dev/null \
            && good "      tagged mirror ${TAG}" \
            || err "      mirror tag failed for ${_name}"
        if [ -n "$_int" ] && [ -d "${_int}/.git" ]; then
            git -C "$_int" tag -f "$TAG" >/dev/null 2>&1 \
                && good "      tagged internal ${TAG}" \
                || err "      internal tag failed for ${_name}"
        fi
    done
fi

hdr "Done"
[ "$DRY" -eq 1 ] && bad "This was a dry run. Re-run with --apply to act." || true
[ "$DRY" -eq 1 ] && note "Nothing was written. Steps 0-6 copy; step 7 commits, tags and pushes." || true
