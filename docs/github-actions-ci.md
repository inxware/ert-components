# GitHub Actions CI

The public build workflows in the community mirror: what they are, how to add
one, and how to inspect them with `gh` instead of the web UI.

The local regression runner is a different thing —
[`SystemTests/CI/README.md`](../SystemTests/CI/README.md) covers that. This is
about what runs on GitHub after a publish.

**The workflows live only in the mirror.** There is no `.github/` in the internal
repository and `.github` is not in the published set, so these files are edited
directly in `../../inxware-github/ert-components/.github/workflows/`. Nothing in
the internal repo points at them, which is worth remembering: a change to how
dependencies are laid out will not show up in any internal grep.

---

## Inspecting a run with `gh`

Reading the log beats guessing from a failure message.

```bash
cd ../../inxware-github/ert-components

gh run list --limit 10                    # recent runs and their conclusions
gh run view <id>                          # one run, job by job
gh run view <id> --log-failed             # ONLY the failed steps — start here
gh run rerun <id>                         # re-run after a fix
gh run rerun <id> --failed                # re-run just the failed jobs
gh workflow run <name>.yml                # trigger on demand
gh run watch <id>                         # follow a run to completion
```

Filtering the failed log is usually enough to place the fault:

```bash
gh run view <id> --log-failed | grep -iE 'error|fatal|cannot find|not found'
```

`gh auth status` should show the `repo` scope. Read-only inspection needs no
more than that.

### Which step failed tells you where to look

| Fails at | Look at |
|-----------------|-------------------------------------------------------------|
| `configure` | something missing from the mirror — a platform or os-arch directory the prune removed, or a target not in the published set |
| `make` | a checkout is missing or has the wrong key: kernel, contrib, or build-support |
| `make targetenv` | packaging, the app checkout, or a certs/devman setting |

---

## Adding a workflow

Copy an existing one and change four things. Every build needs **five**
checkouts, and each dependency is sparse-checked out to the one key that build
resolves — that is what keeps the LFS traffic down.

```yaml
- uses: actions/checkout@v4
  with:
    path: ert-components

- name: Checkout ert build support repo
  uses: actions/checkout@v4
  with:
    repository: inxware/ert-build-support
    fetch-depth: 1
    path: ert-build-support
    sparse-checkout: |
        toolchains/x86_64/<TOOLCHAIN_PATH>
    ref: refs/heads/main
    lfs: true

- name: Checkout ert-kernels
  uses: actions/checkout@v4
  with:
    repository: inxware/ert-kernels
    fetch-depth: 1
    path: ert-kernels
    sparse-checkout: <EHS_GNU_OS_ARCH>/kernel
    ref: refs/heads/main

- name: Checkout ert-contrib-middleware
  uses: actions/checkout@v4
  with:
    repository: inxware/ert-contrib-middleware
    fetch-depth: 1
    path: ert-contrib-middleware
    sparse-checkout: target_libs/<COMPONENT_BASE_TECHNOLOGIES>
    ref: refs/heads/main
    lfs: true

- name: Checkout apps
  uses: actions/checkout@v4
  with:
    repository: inxware/apps
    fetch-depth: 1
    path: apps
    ref: refs/heads/RELEASE-PRODUCTION
```

### Get the keys from the build, do not guess them

```bash
cd ert-components
./configure <target>
make chkconfig
```

`chkconfig` prints the resolved `EHS KERNEL`, `CONTRIB INC`, `BUILD SUPP` and
`TOOLCHAIN PATH`. The directory name in each is the sparse-checkout path. The
three keys are composed differently — the contrib key carries the component
variant and toolchain, the kernel key does not — so they are frequently not the
same string.

### Checklist

- [ ] target is in `publish_set.txt`, or reached through another platform's
      `include` closure — otherwise the prune removes its directory and
      `configure` fails
- [ ] the target is a `-no-certs` variant, or otherwise sets no
      `DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE`; the bundle lives in the private
      DevmanSecurity repo and is never present on a public runner
- [ ] all four dependency keys taken from `make chkconfig`
- [ ] `lfs: true` on build-support and contrib; **not** on ert-kernels, which
      deliberately has no LFS
- [ ] a badge added to `README.md`

---

## Gotchas that have actually bitten

**A missing checkout looks like a compile error.** When the kernels moved to
their own repository, the workflows still cloned only build-support, contrib and
apps. The link failed with `cannot find -l:libehs_ehrt1.a` against a `-L` path
nothing had checked out. It built locally because `ert-kernels` is a sibling
checkout there. If a `-L` or `-I` path in the log looks right but the directory
is reported missing, check what the workflow clones before suspecting the code.

**Push dependencies before ert-components.** Pushing ert-components starts the
build, and the build checks out the dependency repos as they stand. Publish it
first and CI compiles against libraries that have not landed. `publish_release.sh`
step 7 pushes in dependency order for this reason.

**A stale dependency reads as a missing header.** A renamed contrib directory
that had not been pushed produced `curl/curl.h not found`, because the
sparse-checkout matched nothing. The workflow was correct; the mirror was behind.

**`-no-certs` in the name does not guarantee no certs.** Several of those
variants include a parent that sets the CA bundle and did not override it.
Check with:

```bash
./configure <target>
make --eval='p: ; @echo "[$(DEVMAN_SERVER_CERTS_FULL_CA_BUNDLE)]"' p
```

**Workflow step names are not consistent.** Some call the build-support step
"Checkout ert build support repo", others "Checkout tools repo". Match on the
repository name rather than the step name when scripting across them.
