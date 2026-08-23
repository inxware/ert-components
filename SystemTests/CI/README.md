Continuous Integration — Regression Workflow
============================================

> **GitHub Actions is a separate thing.** This document covers the local
> regression runner. For the public build workflows in the community mirror —
> adding one, and inspecting runs with `gh` — see
> [`docs/github-actions-ci.md`](../../docs/github-actions-ci.md).

## Script inventory

```
SystemTests/CI/
├── run_regression.sh                   ← unified entry point (use this)
├── validate_all.sh                     ← thin wrapper: run_regression.sh --discover
├── regression_test-published-only.sh   ← thin wrapper: run_regression.sh --platform-list
├── display_regression_tests.sh         ← re-display results without rebuilding
├── generate_feature_matrix.sh          ← generate feature matrix without a build
├── platform-lists/
│   ├── community.txt                   ← public / community platforms
│   ├── deployed.txt                    ← full INTERNAL product list (not published)
│   └── smoke.txt                       ← fast gate, one target per os-arch family
└── results/
    └── <platform>/                     ← per-platform pass/fail flags + build.log

scripts/build-function-library/
├── ci_functions.sh     ← shared CI function library (sourced by run_regression.sh)
├── colour.sh           ← terminal colour helpers (sourced by ci_functions.sh)
├── build-utilities.sh  ← repo root detection
├── android-utilities.sh
└── debian-utilities.sh

scripts/software-utilities/
└── fb_platform_matrix.py   ← feature compliance matrix generator
```

---

## Quick guide

All examples are run from the **repo root** (`ert-components/`).

### Everyday builds

```bash
# Build every platform found in target/platform/ (interactive menu if no flag given)
./SystemTests/CI/run_regression.sh --discover

# Build only the published curated list
./SystemTests/CI/run_regression.sh --platform-list deployed

# Build only the community (public) list
./SystemTests/CI/run_regression.sh --platform-list community

# Build a custom list file
./SystemTests/CI/run_regression.sh --platform-list /path/to/mylist.txt

# Skip packaging steps — fast compile-only check
./SystemTests/CI/run_regression.sh --discover --compile-only

# Include the slow Unity-lib target
./SystemTests/CI/run_regression.sh --platform-list deployed --include-slow
```

### Adding runtime smoke tests

```bash
# Build + run app tests on Linux x86 targets
./SystemTests/CI/run_regression.sh --discover --run-apps

# Compile-only first pass, then full run with tests
./SystemTests/CI/run_regression.sh --platform-list deployed --run-apps
```

### Generating release reports (recommended entry point)

```bash
# Full versioned release report — builds all published platforms, generates
# per-platform SBOM and feature matrix, rebuilds DEPENDENTS.md from scratch.
# Output: Releases/SBOM/<version>/<platform>/ and Releases/SBOM/<version>/FEATURES/
./Releases/update_release_report.sh

# Same, but also run app smoke tests on Linux x86 targets
./Releases/update_release_report.sh --run-apps

# Compile-only (skip packaging) for a faster report pass
./Releases/update_release_report.sh --compile-only
```

### Generating reports ad-hoc

```bash
# SBOM per platform during a normal regression run
# → Releases/SBOM/<version>/<platform>/SBOM.spdx + SBOM_SUMMARY.md
./SystemTests/CI/run_regression.sh --discover --generate-sbom

# Feature compliance matrix (markdown) written to CI results dir
./SystemTests/CI/run_regression.sh --discover --generate-matrix

# Feature matrix in all formats (CSV, Markdown, HTML)
./SystemTests/CI/run_regression.sh --discover --generate-matrix all

# Matrix only, without a regression run — output to SystemTests/CI/results/
./SystemTests/CI/generate_feature_matrix.sh        # markdown (default)
./SystemTests/CI/generate_feature_matrix.sh csv
./SystemTests/CI/generate_feature_matrix.sh all    # → results/matrix.{csv,md,html}

# Or invoke fb_platform_matrix.py directly (stdout, from repo root)
python3 scripts/software-utilities/fb_platform_matrix.py --format md --no-always --no-never

# SBOM only for the current configured platform
./configure <platform>
make sbom
# → Releases/SBOM/<version>/<platform>/SBOM.spdx
```

### Claude-assisted failure investigation

```bash
# Non-interactive: Claude reads the build log and applies fixes automatically
./SystemTests/CI/run_regression.sh --discover --claude-autofix

# Interactive: opens a Claude Code session on each failure so you can guide the fix
./SystemTests/CI/run_regression.sh --discover --claude-interactive

# Autofix + full reporting for a CI pipeline
./SystemTests/CI/run_regression.sh --platform-list deployed \
    --generate-sbom --generate-matrix --claude-autofix
```

### Skipping checks / inspecting results

```bash
# Skip the CDF hash pre-flight (e.g. mid-investigation)
./SystemTests/CI/run_regression.sh --discover --no-preflight

# Re-display the last run's colourised pass/fail results without rebuilding
./SystemTests/CI/display_regression_tests.sh

# Check CDF ID uniqueness and header consistency independently
python3 scripts/inxware-id-tool/check_cdf_hashes.py Common/Components
```

### Legacy entry points (pass all flags through)

```bash
./SystemTests/CI/validate_all.sh [OPTIONS]
# equivalent to: run_regression.sh --discover [OPTIONS]

./SystemTests/CI/regression_test-published-only.sh [OPTIONS]
# equivalent to: run_regression.sh --platform-list deployed [OPTIONS]

./SystemTests/CI/regression_test-published-only.sh --community-only [OPTIONS]
# equivalent to: run_regression.sh --platform-list community [OPTIONS]
```

---

## `run_regression.sh` — all options

### Mode (choose one; interactive menu if omitted)

| Flag | Effect |
|---|---|
| `--discover` | Enumerate every directory under `target/platform/` — always in sync with the repo |
| `--platform-list <name\|file>` | Use a named list from `platform-lists/` (e.g. `community`, `deployed`, `smoke`) or a direct file path |

When stdin is a terminal and no mode flag is given, an interactive menu lets you pick from the available lists or enter a custom file path.  When stdin is not a terminal (CI/cron), `--discover` is assumed.

### Build options

| Flag | Effect |
|---|---|
| `--build-only` | Fastest gate — `make all_docker` / `make -j8` only; skips `targetenv`, APK, Unity, and exe-presence checks. Writes `build-target.skip` (yellow in summary). |
| `--compile-only` | Build + `targetenv` — skips APK, Unity export, and exe-presence checks |
| `--include-slow` | Append `linux_android_arm64_unity-lib` (and any other slow targets) |
| `--no-preflight` | Skip the CDF hash pre-flight check |

### Test options

| Flag | Effect |
|---|---|
| `--run-apps` | Run smoke tests on Linux `x86` targets and `linux_x86_gtk` (off by default) |

### Reporting options

| Flag | Effect |
|---|---|
| `--generate-sbom` | Run `make sbom` for each platform immediately after it is built (while the build tree is still configured). Writes SPDX 2.3 + markdown summary under `ert-components/sbom/<platform>/` and appends a reverse-dependency row to `DEPENDENTS.md` in each dependency root. Skipped (with a warning) if `./configure` failed for a platform. |
| `--generate-matrix [fmt]` | Run `fb_platform_matrix.py` once after all platforms have been processed; `fmt` = `csv` / `md` / `html` / `all` (default `md`). Output goes to `results/feature_matrix.<fmt>`, or for `all` to `results/matrix.{csv,md,html}`. |
| `--no-summary` | Suppress the colourised pass/fail summary at the end |

### Claude-assisted failure options

| Flag | Effect |
|---|---|
| `--claude-autofix` | Launches `claude --print` with the build log to diagnose and fix failures non-interactively. Run continues afterwards. |
| `--claude-interactive` | Opens an interactive Claude Code session so you can guide the fix. Run resumes when you exit (`Ctrl-D` or `/exit`). |

`claude` must be on `PATH`; a warning is printed and the flag silently skipped otherwise.

---

## Platform list files

Files live in `SystemTests/CI/platform-lists/`.  Format:

```
# comment
linux_x86_64_gtk_gst_debian11
linux_android_arm_p64_h6_ambifier
```

One platform directory name per line; blank lines and `#` comments are ignored.
Create additional list files as needed (e.g. `nightly.txt`, `smoke.txt`).

---

## Shared function library: `scripts/build-function-library/ci_functions.sh`

All build, test, and reporting logic lives here so that entry-point scripts
contain only argument parsing and orchestration.

**Why `scripts/build-function-library/` and not `SystemTests/CI/`?**
The `scripts/` tree owns build-execution machinery; `SystemTests/CI/` owns
*what to test* and *how to report results*.  Keeping the shared functions in
`scripts/build-function-library/` means they can also be sourced by ad-hoc
build scripts in `scripts/build-deploy/` without creating circular dependencies.

### Functions provided

| Function | Purpose |
|---|---|
| `ci_setup_paths` | Sets `ROOT_DIR`, `CI_RESULT_DIR`, `TARGET_TREE_DIR`, `SYSTEM_TESTS_DIR`, `TOOL_TEST_EHS_VARIANT` |
| `ci_skip_platform <name>` | Returns 0 (skip) for `.svn`, `depricated`, `base*` |
| `ci_setup_platform_dir` | Creates fresh `results/<platform>/` and touches `building.flag` |
| `ci_teardown_platform_dir` | Removes `building.flag` |
| `ci_remove_build` | Removes `ehs.exe` **and** `*.so` from the target tree |
| `ci_test_build` | Checks for `ehs.exe` or `ehs_<platform>.so`; writes `exe-built.pass/.fail` |
| `ci_build_target` | Full build: clean → configure → Docker-or-host → targetenv → APK → Unity |
| `ci_display_log_tail` | Prints last 20 lines of a log file to the console |
| `ci_on_build_failure <log> <stage>` | Calls Claude autofix or interactive session if enabled |
| `ci_run_preflight [root]` | Runs `check_cdf_hashes.py`; returns 1 on failure |
| `ci_generate_sbom` | Runs `make sbom` for the current platform (guards on `TARGET.cfg` match); writes `sbom.pass/.fail/.skip` |
| `ci_generate_matrix [fmt]` | Runs `fb_platform_matrix.py`; writes output to `results/` |
| `ci_show_summary` | Colourised `ls` of all results |
| `ci_exit_code` | Returns 1 if any `.fail` flag exists in the results tree |

### `ci_build_target` — build strategy

| Condition | Action |
|---|---|
| `target/platform/<P>/Dockerimagename` exists | `make all_docker` |
| No Dockerimagename | `make -j 8` (host build) |
| After compile succeeds | `make targetenv` |
| `ehs_<platform>.so` present and not `--compile-only` | `make targetenv_apk` |
| Platform name contains `_unity` (except `unity-lib`) | `make targetenv_unity_export` |

`make prepdeps` is always run before the compile step.  On any failure,
`ci_display_log_tail` prints the tail to the console and `ci_on_build_failure`
invokes Claude if requested.

---

## Pre-flight: CDF hash check

Before any build, `run_regression.sh` runs:

```bash
python3 scripts/inxware-id-tool/check_cdf_hashes.py Common/Components
```

Checks:
1. Every function-block `<NameHash_CRC16>` is unique across all CDFs.
2. Any matching `INXWARE_FB_ID_<Class>` / `EHS_FB_ID_<Class>` `#define` in a
   `.h` file agrees with the CDF value.

IDs not equal to the CRC16 of their class name produce **advisory warnings**
(non-fatal).  A uniqueness or consistency failure **aborts the regression run**.

Skip with `--no-preflight` if running mid-investigation.

---

## Feature compliance matrix

Running with `--generate-matrix` calls `scripts/software-utilities/fb_platform_matrix.py`
after the build phase.  Output location depends on the format requested:

| Invocation | Output file(s) |
|---|---|
| `--generate-matrix md` (or `csv` or `html`) | `SystemTests/CI/results/feature_matrix.<fmt>` |
| `--generate-matrix all` | `SystemTests/CI/results/matrix.{csv,md,html}` |

The matrix shows which function blocks are supported on which platforms, derived
from CDF files and `config.mk` support variables — no manual maintenance required.
Cell values: `always` (unconditionally built), a HAL variant name (e.g. `sferalabs`),
`—` (not built on this platform), `?` (support variable present but unset).

Run the tool independently at any time from the repo root:

```bash
python3 scripts/software-utilities/fb_platform_matrix.py --format md
python3 scripts/software-utilities/fb_platform_matrix.py --format all   # → matrix.{csv,md,html} in cwd
python3 scripts/software-utilities/fb_platform_matrix.py --help
```

See `scripts/software-utilities/README.md` for full option reference.

---

## Platform dependency tagging (`make sbom` / `--generate-sbom`)

SBOM artefacts are written under a versioned path:

```
Releases/SBOM/<ert-components-version>/<platform>/
    SBOM.spdx           SPDX 2.3 forward SBOM
    SBOM_SUMMARY.md     markdown dependency table
```

The version string is derived from `git describe --tags --always` of the
`ert-components` repository at build time.

**Reverse-dependency records** are also appended to a `DEPENDENTS.md` file in
each dependency root (`ert-contrib-middleware/target_libs/<arch>/build/` and
the `ert-build-support` equivalent), building up a cross-platform middleware
usage matrix as platforms are built.

**Three ways to generate SBOMs:**

| Method | Use case |
|---|---|
| `./Releases/update_release_report.sh` | Full versioned release: all published platforms, DEPENDENTS.md rebuilt from scratch |
| `run_regression.sh ... --generate-sbom` | Ad-hoc regression run — DEPENDENTS.md appended to |
| `./configure <platform> && make sbom` | Single platform by hand |

The middleware path for a platform is:
```
../ert-contrib-middleware/target_libs/<COMPONENT_BASE_TECHNOLOGIES>/build/
```
`COMPONENT_BASE_TECHNOLOGIES` is defined in `target/platform/<platform>/config.mk`.

See `target/platform/sbom.mk` for the full implementation.

---

## Results layout (per platform)

Under `SystemTests/CI/results/<platform>/`:

| File | Meaning | Written by |
|---|---|---|
| `build.log` | Full stdout+stderr of all make commands | all |
| `build.pass` / `build.fail` | Docker/host compile outcome | all |
| `build-target.pass` / `.fail` | `targetenv` outcome | all |
| `build-target.notrun` | `targetenv` skipped (compile failed) | all |
| `build-target.skip` | `targetenv` intentionally skipped (`--build-only`) | `--build-only` |
| `build-apk.pass` / `.fail` | `targetenv_apk` outcome | Android targets |
| `build-unity.pass` / `.fail` | `targetenv_unity_export` outcome | Unity targets |
| `exe-built.pass` / `.fail` | Binary presence check (`ehs.exe` or `.so`) | all |
| `exe-host-run-app.pass` / `.fail` / `.timedout` | App smoke-test outcome | Linux x86 targets with `--run-apps` |
| `sbom.pass` / `sbom.fail` | SBOM generation outcome | `--generate-sbom` |
| `sbom.skip` | SBOM skipped (configure failed — `TARGET.cfg` mismatch) | `--generate-sbom` |
| `building.flag` | In-progress marker; removed on completion | all |
| `feature_matrix.<fmt>` | Feature compliance matrix (top-level results dir) | `--generate-matrix` |
| `matrix.{csv,md,html}` | All three matrix formats (top-level results dir) | `--generate-matrix all` |

Re-display results without rebuilding:

```bash
./SystemTests/CI/display_regression_tests.sh
```

---

## How the old scripts relate to the new structure

### Before

```
validate_all.sh               — self-contained: discovery, build, test, Claude hooks
regression_test-published-only.sh — self-contained: curated list, build, test, colour output
```

Both scripts duplicated: path setup, skip logic, `remove_build`, `test_build`,
per-platform directory lifecycle.  Their implementations of each had diverged:

- `test_build` / `remove_build` in `validate_all.sh` missed Android `.so` files
- `validate_all.sh` had no Docker-vs-host fallback
- `regression_test-published-only.sh` had no CDF pre-flight or Claude integration

### After

```
run_regression.sh          — orchestration only (arg parsing, loop)
  └── sources ci_functions.sh  — all build/test/report logic
validate_all.sh            — exec run_regression.sh --discover "$@"
regression_test-published-only.sh — exec run_regression.sh --platform-list "$@"
```

The divergences are resolved in `ci_functions.sh`:
- `ci_test_build` / `ci_remove_build` handle both `ehs.exe` and `.so`
- `ci_build_target` checks for `Dockerimagename` before choosing Docker vs host
- All entry points get the CDF pre-flight check and Claude integration

---

## Other scripts in this directory

| Script | Status | Notes |
|---|---|---|
| `display_regression_tests.sh` | Current | Colourised `ls` of results; no changes needed |
| ~~`build_linux_tools_installer.sh`~~ | **Deleted** | Hardcoded `linux_x86_gtk_gst_withtools` — a platform name that no longer exists (the real platform is `linux_x86_gtk_gst_deb_withtools`). Would have failed at `./configure` on any current checkout. `test_build`/`test_run_apps` were commented out. Did not reference `../inxware-tools`, `../dis`, or `../inxware-installer`; only built a `.deb` that bundled the pre-built Windows IDE installer as a Wine postinst step. Fully covered by `run_regression.sh --platform-list deployed`. |
| `deploy_files.sh` | Consider moving | SCP wrapper for copying binaries to test machines. Candidate for `scripts/build-deploy/linux-general/`. |
| `start_remote_ehs.sh` | Consider moving | SSH wrapper to launch EHS on a remote device. Same candidate path. |
| `send_schedule.sh` | Consider deprecating | 4-line curl wrapper for sending a playlist over HTTP. |
| `install_inx_CI.sh` | Outdated | Incomplete CI environment setup (MinGW). Contains commented-out svn commands. |

---

## TODO

1. Add `--platform-list` / `--run-apps` as a named list entries or CI presets to cover
   "quick smoke build" vs "full nightly with matrix" as a selectable menu option.
2. Audit `devman.py` duplication: the file exists independently in
   `projects/ambifier/`, `projects/heatrod/`, and `projects/player/` — extract
   to a shared location under `scripts/` or a Python package.
3. Same for `microtik.py` (appears in both `ambifier` and `heatrod`).
4. Add CDF pre-flight and `--claude-autofix` / `--generate-matrix` as optional
   Jenkins/GitHub Actions pipeline steps calling `run_regression.sh` directly.
5. Re-instate unit test code as an additional regression element, gated behind a
   `--unit-tests` flag in `run_regression.sh`.
6. Decide whether `deploy_files.sh` / `start_remote_ehs.sh` belong here or under
   `scripts/build-deploy/linux-general/` (they are build-deploy concerns, not
   regression-test concerns).
