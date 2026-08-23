# Build and CI fragility — review

What broke while wiring up the new CI targets, why, what is fixed, and what
would catch the next one. Written from the actual failures, not from theory.

Nothing here is a defect in the runtime. Every one is a *build-system* or
*process* fault, and almost all share one shape: **something works locally
because of state the clean environment does not have.**

---

## 1. Ambient state the container does not share

The dominant failure class. Six separate outages, one cause.

| Failure | Ambient thing relied on |
|-------------------------------|--------------------------------------------|
| `docker: command not found` in `targetenv_package` | a docker daemon to re-enter |
| same in `zephyr_prepdeps` / `zephyr_build_docker` | ditto |
| `fakeroot: command not found` building a `.deb` | fakeroot on the host |
| `Could not find … Zephyr-sdk` | `$HOME/.cmake/packages` as root |
| `Minimum Git version … is 2.28` | a newer git than the pinned image had |
| `curl/curl.h` not found | a contrib key present locally, unpublished |

**Improved.** `EHS_IN_CONTAINER` (probing `/.dockerenv`, overridable) now selects
the non-docker packager and Zephyr paths. `dpkg-deb --root-owner-group` removes
the fakeroot dependency outright rather than adding a package to an image.
`zephyr_build.sh` globs `/opt/toolchains/zephyr-sdk-*` when the registry is
unreadable. All verified by reproducing the CI condition locally.

**Still open.** Nothing systematically detects "this recipe assumes a host
facility". A grep for `_docker` make goals reachable from a workflow would find
the remaining ones cheaply.

---

## 2. Published images that cannot be rebuilt

`inxware/esp32s3_ubuntu22.04-build-essential` was `FROM ubuntu:20.04` despite the
name, and its Dockerfile had **five** faults that made it unbuildable:
`libpython2.7` (gone in 22.04), `--only-binary` inside a quoted requirement,
`gdbgui` pinned past Espressif's own win32-only marker, and a `pyparsing`
conflict. Its sibling had three of the same. Both images also diverged from
source in substance — the esp32 one has its venv at `/opt/venv` where the
Dockerfile says `/opt/python_env`, and the `littlefs-python`/`esptool` it pins
were absent.

They worked only as artefacts built years ago and never rebuilt.

**Improved.** Consolidated onto one image, now reproducible from its Dockerfile,
with `esptool`/`littlefs-python` **exact-pinned** to the retired image's versions
— `>=` is what let them drift. Verified byte-identical output before pushing.

**To add.** A weekly job that rebuilds every `target/platform/*/Dockerfile` and
fails on error. Nothing else will catch this: an image can be pullable and
useful for years while its source rots.

---

## 3. Guards that never fire

Three instances, all silent.

- `ifndef HS_UART_SUPPORT` — missing `E`. The name exists nowhere, so the branch
  always fired and overrode a platform's deliberate `EHS_UART_SUPPORT=none`.
- `NEVER_PUBLISH` entry `SystemTests/CI/projects/` — each plan entry is its own
  rsync transfer, so patterns match relative to *that* entry. With
  `SystemTests/` as the root the path is `CI/projects`, and the repo-root
  spelling matched nothing. Customer CI scripts reached the public mirror.
- `prune_mirror.sh` matched every pattern with `find -name`, which cannot match a
  slash or anchored pattern — the same blind spot, so it could not report the
  leak either.

**Improved.** All three fixed, with the rsync pattern rules written into
`publish_set.py` so the next entry is not added blind.

**To add.** A test that asserts each `NEVER_PUBLISH` pattern matches something it
should and nothing it should not. A pattern that matches *nothing at all* is
almost always wrong and is trivially detectable.

---

## 4. Regex over-capture — the allow-list bypass

The worst one. `INC_OSARCH_REL = r"\.\./([A-Za-z0-9_.-]+)/"` captured the second
`..` of `../../platform/`, and `isdir(target/os-arch/..)` is true because that is
`target/`. The plan therefore carried `target/os-arch/../`, which rsync resolves
to `target/` — copying **every** platform and bypassing the 78-platform
allow-list entirely.

Customer platforms were published, and anything `prune_mirror.sh` deleted came
straight back on the next publish. That is what "the same 139 directories keep
reappearing" was.

**Improved.** `_is_osarch_dir()` rejects `.`/`..` and any separator and requires
the resolved path to stay under `target/os-arch`.

**To add.** A parse-time assertion that **no plan entry contains `..`**, and a
publish pre-flight that refuses if the resolved platform count differs from the
allow-list closure by more than the known base-platform count. Both are a few
lines and both would have caught this on day one.

---

## 5. Unconditional code against conditional config

Two instances, identical shape:

- `targetos_init.c` called `EhsTargetWidget_refr_scr_period_lvgl()` with no
  guard, while `target_viewport.c` compiles only with LVGL. Every GUI-less
  esp32s3 target failed to link.
- `graphics.mk` added `lvgl_touch/tp_spi.o` whenever LVGL was on, but that file
  needs `CONFIG_LV_TOUCH_SPI_*`, which only the touch-driver blocks define. A
  display-only target failed on a missing Kconfig symbol rather than on the real
  cause.

Both produce errors that point away from the actual problem.

**Improved.** Guarded on `EHS_GUI_SUPPORT_MODE_B_LVGL` and
`EHS_LVGL_TOUCH_DRIVER` respectively — in each case the exact variable that
governs whether the symbol exists.

**To add.** The rule is simple enough to state: *an object and the config it
dereferences must be gated on the same variable.* Worth a line in the porting
guide, because the failure mode is a misleading error rather than a clear one.

---

## 6. Lists that drift from what they describe

`community.txt` claimed to be the public platform list; 10 of 21 entries were
absent from the mirror, 5 of those referencing platform directories that no
longer exist, one a customer target. `published.txt` was the internal product
list, named as though it were the mirror one — which is what led to
misdiagnosing the wrong file.

**Improved.** `community.txt` pruned and its invariant written into the header
with the command that checks it. `published.txt` → `deployed.txt`.

**To add.** Make the invariant executable, not documentary: the CI pre-flight
should fail if any `community.txt` entry is missing from the resolved plan, or
if any entry names a directory that does not exist. Both are one-liners against
`publish_set.py --plan`.

---

## 7. The working tree is what gets published

`publish_set.py` rsyncs the working tree, so `.gitignore` protects nothing. A
routine build left 676 `.o`/`.d` files that would have been published, invisible
in `git status`.

**Improved.** Caught by running `--diff` before the publish; tree cleaned.

**To add.** `*.o`, `*.d`, `*.su` on `NEVER_PUBLISH`, and a pre-flight that
refuses to publish when the tree contains build output. The current safeguard is
that somebody reads a 700-line diff.

---

## 8. CI cannot run what the regression cannot

`ci_build_target` dispatches on `Dockerimagename` to `make all_docker`, which
Zephyr rejects at parse time — so Zephyr targets could never have been in a
regression list. Separately, `ci_test_run_apps` pointed at a directory absent
from the repository and returned failure on every call, so `--run-apps` had
never done anything.

**Improved.** Zephyr's three-step build is wired into `ci_build_target`;
`ci_test_run_apps` deleted and `--run-apps` repointed at the app-test engine.

**To add.** Nothing structural — but it is worth noting that *a step which always
fails looks identical to a step that is not run* unless someone checks. The
app-test engine now exits non-zero only for genuine defects, which is the
property that makes a green result mean something.

---

## Priorities

Cheapest first, by (catches × cost):

1. **Assert no plan entry contains `..`**, and diff the resolved platform count
   against the allow-list closure. Prevents a repeat of the worst failure here.
2. **Build-output check in the publish pre-flight** plus `*.o`/`*.d` on
   `NEVER_PUBLISH`.
3. **Executable list invariants** — `community.txt` ⊆ resolved plan, every entry
   a real directory.
4. **`NEVER_PUBLISH` self-test** — every pattern must match something.
5. **Weekly Dockerfile rebuild job.** The most expensive, and the only thing that
   catches image rot.

The first four are a morning's work and address the failures that had real
disclosure consequences.

---

## The honest summary

Most of these had been latent for a long time. They surfaced now because new
targets were built in a clean environment for the first time — which is exactly
what CI is for, and an argument for adding targets to it earlier rather than
once they are believed to work.

Two were live disclosures (customer CI scripts, customer platform names), both
from allow-list mechanisms that silently matched nothing. **A guard that fails
open and says nothing is worse than no guard**, because it is credited in
reviews. That is the pattern most worth designing against.
