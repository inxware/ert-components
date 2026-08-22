#!/usr/bin/env bash
#---------------------------------------------------------------
# Copyright (C) 2026, inx limited, UK.
# All Rights Reserved.  LGPLv3 — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# truncate_remote_history.sh — replace a remote repository's history with a
#                              single commit of its current tree.
#
# SCOPE — one repository, one branch. Irreversible on the remote.
#
# Input : a checkout whose remote you want truncated.
# Output: with --apply, force-pushes an orphan commit over the branch, so the
#         remote has exactly one commit. Writes a full backup bundle first.
#         Report-only without --apply.
#
#   ./truncate_remote_history.sh --repo ../../inxware-github/ert-components
#   ./truncate_remote_history.sh --repo ../../inxware-github/ert-components --apply
#
# ONE BRANCH IS NOT ENOUGH. Every remote branch and every tag is a root: leave
# one behind and the whole old history is still reachable, and the repository
# does not shrink either. The script lists them and will not claim success while
# any remain — use --drop-branches / --drop-tags to remove them too.
#
# ─────────────────────────────────────────────────────────────────────────────
# READ THIS BEFORE USING IT FOR DISCLOSURE REMOVAL
#
# Truncating history is NOT a reliable way to un-publish something. It reduces
# exposure; it does not end it. On GitHub specifically:
#
#   * Unreachable objects stay served. A commit removed from a branch is still
#     fetchable by its SHA through the web UI and the API until GitHub garbage
#     collects, which does not happen on a schedule you control. Anyone with a
#     SHA — from a log, a CI badge, an email, a cached page — can still read it.
#     Removing them for real requires a GitHub Support request naming the repo
#     and asking for unreachable objects to be purged.
#   * Forks keep everything. Objects live in a shared fork network, so a single
#     fork keeps the old history reachable through the parent's URL space.
#     This script refuses to run if the repository has forks.
#   * Pull requests survive. PR diffs and their refs are not part of the branch
#     and are unaffected.
#   * Existing clones are complete copies. Nothing you do to the remote reaches
#     them.
#   * Mirrors and archives. Software Heritage, GH Archive, code-search indexes
#     and proxies may already hold it.
#
# So: for a LEAKED CREDENTIAL, truncation is not the remedy — rotate the
# credential. That is the only action that actually works. For names, paths and
# internal notes, truncation plus a Support purge is a reasonable reduction, and
# is worth doing, but do not record it as "removed".
#
# ─────────────────────────────────────────────────────────────────────────────
# EFFICACY FOR REDUCING CHECKOUT SIZE
#
# Effective for plain git objects, and only for content that is no longer in the
# tree. If a 100 MB binary is still checked in, truncating history does not help
# — it is in the one remaining commit.
#
# git-LFS is the exception that matters here: LFS objects are not in the git
# object store, so dropping the commits that referenced them does NOT delete
# them from the remote's LFS storage, and does not reduce LFS billing. GitHub
# offers no self-service way to prune orphaned LFS objects; that is another
# Support request. What truncation does fix is content committed as real blobs
# that should have been LFS pointers — of which this mirror has several thousand.
#
# ─────────────────────────────────────────────────────────────────────────────
# WHAT YOU LOSE, PERMANENTLY
#
#   blame, bisect, log, and all authorship attribution before the truncation
#   every tag pointing into the old history (this script lists them)
#   commit SHAs referenced anywhere: docs, tickets, release notes, CI runs
#   the ability to diff against any earlier release
#
# A backup bundle is written before the push, so the history survives locally.
# Keep it somewhere durable — it is the only remaining copy once the remote is
# rewritten and old clones are gone.

set -uo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"

REPO=""; DRY=1; DROP_TAGS=0; DROP_BRANCHES=0
BACKUP_DIR="${HOME}/git-history-backups"
while [ $# -gt 0 ]; do
    case "$1" in
        --repo)          REPO="${2:-}"; shift 2 ;;
        --apply)         DRY=0; shift ;;
        --drop-tags)     DROP_TAGS=1; shift ;;
        --drop-branches) DROP_BRANCHES=1; shift ;;
        --backup-dir)    BACKUP_DIR="${2:-}"; shift 2 ;;
        -h|--help)       sed -n '6,77p' "$0"; exit 0 ;;
        *) echo "unknown option: $1" >&2; exit 2 ;;
    esac
done

source "${ROOT}/scripts/build-function-library/colour.sh"
note() { printf '   %s\n' "$*"; }
bad()  { printf '   %s%s%s\n' "${TXT_FG_BRIGHT_RED}"    "$*" "${TXT_RESET}"; }
warnl(){ printf '   %s%s%s\n' "${TXT_FG_BRIGHT_YELLOW}" "$*" "${TXT_RESET}"; }
good() { printf '   %s%s%s\n' "${TXT_FG_GREEN}"         "$*" "${TXT_RESET}"; }
die()  { bad "$*"; exit 1; }

[ -n "$REPO" ] || die "--repo is required"
[ -d "${REPO}/.git" ] || die "not a git checkout: ${REPO}"
REPO="$(cd "$REPO" && pwd)"
NAME="$(basename "$REPO")"
BRANCH="$(git -C "$REPO" rev-parse --abbrev-ref HEAD)"
URL="$(git -C "$REPO" remote get-url origin 2>/dev/null)" || die "no origin remote"

printf '\n%s── truncate_remote_history%s\n' "${TXT_BOLD}${TXT_FG_BRIGHT_WHITE}" "${TXT_RESET}"
[ "$DRY" -eq 1 ] && bad "REPORT ONLY — nothing will be changed. Add --apply to act."
note "repo   : ${REPO}"
note "remote : ${URL}"
note "branch : ${BRANCH}"

# ── Refusals ────────────────────────────────────────────────────────────────
# Each of these makes truncation either destructive beyond the intent, or
# pointless because the history stays reachable anyway.
[ -n "$(git -C "$REPO" status --porcelain)" ] && \
    die "working tree is dirty. Commit or stash first — the orphan commit takes the tree as it stands."

_unpushed=$(git -C "$REPO" rev-list --count "origin/${BRANCH}..HEAD" 2>/dev/null || echo 0)
[ "$_unpushed" -gt 0 ] && \
    die "${_unpushed} unpushed commit(s). Push or discard them first; truncation would bury them."

if command -v gh >/dev/null 2>&1; then
    # owner/repo from either git@host:owner/repo.git or https://host/owner/repo
    _slug="$(printf '%s' "$URL" | sed -E 's|\.git$||; s|^[^:]+://[^/]+/||; s|^.*:||')"
    _forks=$(gh api "repos/${_slug}" --jq '.forks_count' 2>/dev/null) || _forks="?"
    case "$_forks" in ''|*[!0-9]*) _forks="?" ;; esac
    _prs=$(gh pr list --repo "$_slug" --state all --limit 1 --json number --jq 'length' 2>/dev/null) || _prs="?"
    case "$_prs" in ''|*[!0-9]*) _prs="?" ;; esac
    note "forks  : ${_forks}    pull requests (any state): ${_prs}"
    [ "$_forks" = "?" ] && warnl "could not query ${_slug} — check the fork count by hand before --apply"
    if [ "$_forks" != "0" ] && [ "$_forks" != "?" ]; then
        bad "This repository has ${_forks} fork(s)."
        bad "Forks share an object network, so the old history stays reachable"
        bad "through this repository's own URLs. Truncation will not hide anything."
        die "Refusing. Delete the forks, or accept that this is size-only."
    fi
    [ "$_prs" != "0" ] && [ "$_prs" != "?" ] && \
        warnl "There are pull requests. Their diffs and refs survive truncation."
else
    warnl "gh not found — cannot check for forks. A single fork defeats the disclosure purpose."
fi

# ── What is lost ────────────────────────────────────────────────────────────
_commits=$(git -C "$REPO" rev-list --count HEAD)
_authors=$(git -C "$REPO" log --format='%an' | sort -u | wc -l)
_first=$(git -C "$REPO" log --reverse --format='%ad' --date=short | head -1)
_tags=$(git -C "$REPO" tag | wc -l)
_lfs=$(git -C "$REPO" lfs ls-files 2>/dev/null | wc -l)
printf '\n'
warnl "would discard ${_commits} commit(s) by ${_authors} author(s), back to ${_first}"
note  "  .git is currently $(du -sh "${REPO}/.git" 2>/dev/null | cut -f1); the tree is $(du -sh --exclude=.git "$REPO" 2>/dev/null | cut -f1)"
note  "  the tree is what remains, so the saving is roughly .git minus the tree's own objects"
if [ "$_lfs" -gt 0 ]; then
    warnl "  ${_lfs} LFS-tracked file(s): truncation does NOT free their remote LFS storage"
fi
if [ "$_tags" -gt 0 ]; then
    warnl "  ${_tags} tag(s) point into the old history:"
    git -C "$REPO" tag | head -10 | sed 's/^/        /'
    [ "$DROP_TAGS" -eq 1 ] && note "        --drop-tags: these will be deleted locally and on the remote" \
                           || bad  "        NOT removed. Each one keeps the old history reachable. Use --drop-tags."
fi

# Any other remote branch is a root of its own. Left alone, it holds the whole
# old history — so both the disclosure and the size goal fail silently.
# Filter on the full refname, and use the bare prefix rather than a glob:
# refs/remotes/origin/* matches only one level and would miss chore/foo, while
# %(refname:short) shortens refs/remotes/origin/HEAD to plain "origin".
_others="$(git -C "$REPO" for-each-ref --format='%(refname)' refs/remotes/origin \
           | sed 's|^refs/remotes/origin/||' | grep -vxE "HEAD|${BRANCH}" || true)"
_nb=$(printf '%s' "$_others" | grep -c . || true)
if [ "${_nb:-0}" -gt 0 ]; then
    warnl "  ${_nb} other remote branch(es):"
    printf '%s\n' "$_others" | sed 's/^/        /'
    if [ "$DROP_BRANCHES" -eq 1 ]; then
        note "        --drop-branches: these will be deleted on the remote"
    else
        bad "        NOT removed. Any one of these keeps the entire old history"
        bad "        reachable and keeps the repository the same size. Truncating"
        bad "        ${BRANCH} alone accomplishes nothing. Use --drop-branches."
    fi
fi

if [ "$DRY" -eq 1 ]; then
    printf '\n'
    bad "Nothing was changed. Re-run with --apply."
    note "After a real run you must also ask GitHub Support to purge unreachable"
    note "objects, or the old commits stay fetchable by SHA."
    exit 0
fi

# ── Confirm ─────────────────────────────────────────────────────────────────
printf '\n'
bad "This rewrites ${URL} branch ${BRANCH}. It cannot be undone on the remote."
printf '   Type the repository name (%s) to continue: ' "$NAME"
read -r _typed </dev/tty || _typed=""
[ "$_typed" = "$NAME" ] || die "not confirmed — nothing done."

# ── Backup first ────────────────────────────────────────────────────────────
mkdir -p "$BACKUP_DIR"
_stamp="$(git -C "$REPO" log -1 --format=%h)"
_bundle="${BACKUP_DIR}/${NAME}-before-truncate-${_stamp}.bundle"
note "writing backup bundle..."
git -C "$REPO" bundle create "$_bundle" --all >/dev/null 2>&1 \
    || die "backup failed — refusing to continue"
good "backup: ${_bundle} ($(du -sh "$_bundle" | cut -f1))"
note "restore with:  git clone ${_bundle} <dir>"

# ── Truncate ────────────────────────────────────────────────────────────────
_orphan="truncate-$(git -C "$REPO" log -1 --format=%h)"
git -C "$REPO" checkout -q --orphan "$_orphan" || die "could not create orphan branch"
git -C "$REPO" add -A
git -C "$REPO" commit -q -m "Initial commit

History truncated. Earlier commits are not published: they contained content
withdrawn from this repository, and material that inflated every clone.

This tree is the whole project as released. Nothing functional was removed." \
    || die "orphan commit failed"

git -C "$REPO" branch -M "$BRANCH" || die "could not rename ${_orphan} to ${BRANCH}"

if [ "$DROP_TAGS" -eq 1 ] && [ "$_tags" -gt 0 ]; then
    for t in $(git -C "$REPO" tag); do
        git -C "$REPO" push -q --delete origin "$t" 2>/dev/null
        git -C "$REPO" tag -d "$t" >/dev/null 2>&1
    done
    good "tags deleted locally and on the remote"
fi

note "force-pushing ${BRANCH}..."
git -C "$REPO" push -q --force origin "$BRANCH" || die "push failed — the remote is unchanged"
good "pushed. ${URL} branch ${BRANCH} now has one commit."

if [ "$DROP_BRANCHES" -eq 1 ] && [ "${_nb:-0}" -gt 0 ]; then
    # After main is truncated. Deleting the default branch is not possible, and
    # the order does not otherwise matter.
    for b in $_others; do
        git -C "$REPO" push -q --delete origin "$b" 2>/dev/null \
            && good "deleted remote branch: ${b}" \
            || warnl "could not delete remote branch: ${b} (protected?)"
    done
fi

git -C "$REPO" reflog expire --expire=now --all >/dev/null 2>&1
git -C "$REPO" gc --prune=now --aggressive >/dev/null 2>&1
good "local .git is now $(du -sh "${REPO}/.git" | cut -f1)"

printf '\n'
warnl "STILL TO DO — the remote is not clean yet:"
note  "  1. Ask GitHub Support to purge unreachable objects for this repository."
note  "     Until they do, every old commit is still fetchable by its SHA, and"
note  "     the repository on GitHub does not actually get smaller."
note  "  2. If a credential was ever committed, rotate it. Truncation is not a fix."
note  "  3. Tell anyone with a clone to re-clone; their copy still has everything."
[ "$_lfs" -gt 0 ] && \
note  "  4. Orphaned LFS objects need a separate Support request to remove."
if [ "$DROP_BRANCHES" -eq 0 ] && [ "${_nb:-0}" -gt 0 ]; then
    bad "  ${_nb} branch(es) still hold the old history. This run did not achieve"
    bad "  either goal. Re-run with --drop-branches."
fi
if [ "$DROP_TAGS" -eq 0 ] && [ "$_tags" -gt 0 ]; then
    bad "  ${_tags} tag(s) still hold the old history. Re-run with --drop-tags."
fi
exit 0
