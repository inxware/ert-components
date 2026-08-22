# scripts/git-utilities

## GitHub mirroring

Publishing the internal repositories to <https://github.com/inxware>.

**Full procedure: [`docs/community-github-mirror.md`](../../docs/community-github-mirror.md).**

### Run this one

**`publish_release.sh`**: This does the  **whole publish** all six steps:
1.  Incoming patches
2. hand-merge
3. ert-components
4. contrib
5. build-support
6. ert-kernels
7. apps. Dry run unless given `--apply`. `--help` lists the steps, options and pre-flight checks. 

> This is the only script here you normally call directly
> Note this does not update Lucid builds and demo applications to appland.inxware.io (see community-github-mirror.md)
> DOES NOT COMMIT TO REPOS YOU NEED TO DO THAT AFTERWARDS!!

### Sub Scripts for Selective Mirroring Repos

`publish_set.py`: Decides and applies *what* is published for ert-components to an external repo:
1. reads `publish_set.txt`, 
2. resolves each target's `include` closure - WHAT DOES THIS MEAN???
3. refuses anything matching the private pattern. 
4. Modes: `--check --diff --plan --merge --sync --deps`. 
5. **Useful standalone for** `--check` and `--diff` two repositories for changes.

### Inputs, not outputs

Hand-maintained. Editing these changes what ships.

`publish_set.txt`       : The allow-list. a *leaf* platform targets only. Parents come automatically via the include closure (??? WHAT DOES THIS MEAN)
`publish_set_merge.list`: Paths that legitimately differ between internal and public and must never be blindly overwritten. Step 2 opens each in meld.

### Superseded

 `merge_into_github.sh`   Old menu wrapper. Maps the old `a`/`b`/`c` letters onto `publish_set.py --merge/--sync/--diff` and adds nothing. Kept only because the menu is in people's fingers. 
 > Use `publish_release.sh` instead !!! 

---

## Withdrawing things already published

Run by hand, never from `publish_release.sh`. Both report only until given
`--apply`.

`prune_mirror.sh`: Deletes directories the allow-list no longer includes from the
mirror's **working tree**. Needed because `publish_release.sh` rsyncs each
platform individually, so its `--delete` can add and update a platform but never
withdraw one. Removes from HEAD only — the content stays in history.

`truncate_remote_history.sh`: Replaces a remote's history with one commit of the
current tree. For (a) content that should never have been published and (b)
clone size. Writes a backup bundle first and refuses on a dirty tree, unpushed
commits, or any fork.

> Read the header before using it for (a). It **reduces** exposure rather than
> ending it: unreachable commits stay fetchable by SHA until GitHub Support
> purges them, forks and PRs survive, and existing clones are unaffected. For a
> leaked credential the only real remedy is rotating it.
>
> Truncating one branch is not enough. Every other branch and tag is a root that
> keeps the whole old history reachable — pass `--drop-branches --drop-tags`, or
> nothing is achieved. LFS objects are not freed either way.

---

## Miscellaneous git helpers

Unrelated to mirroring. Neither is called by anything else.

| Script                          | Scope                                                      |
|---------------------------------|------------------------------------------------------------|
| `gitRemoveBranch.sh`            | Delete a branch locally and on the remote.                 |
| `gitResetHeadToRemoteMaster.sh` | `git fetch`/**hard reset** to `origin/master` Discards local work — no confirmation prompt!!! |
