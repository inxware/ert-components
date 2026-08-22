# SystemTests/apps — Lucid application testing

One engine, `SystemTests/CI/run_lucid_apps.sh`, answering two questions:

| Question | How | Applies to |
|-----------------------------|--------------------------------------|--------------------------------|
| Does the app load and stay up? | kernel console records | every app |
| Does it compute the right answer? | `diff` against the checked-in `test_result.txt` (`--assert`) | apps authored as tests |

The first needs nothing from the app. The second needs the app to report a
result and a stored expectation to compare against, so it only covers apps
written as tests — `--assert` is therefore on by default for the `unit` and
`system` suites and off for `external`.

**There are no make targets for this.** App tests run across builds × apps, and
make cannot express that while bound to a single `TARGET.cfg`; passing the build
on the command line is the pattern this repo already rejects for `TARGET=`. make
builds env trees (`make targetenv`); the runner verifies one exists and tells you
what to build if it does not. It never triggers a build itself.

This replaced two earlier mechanisms: `targetenv_run_tests.sh`
(`make targetenv_run_tests`) and `ci_test_run_apps` in `ci_functions.sh`. Both
are deleted. `validate_all.sh --run-apps` now calls this engine.

---

## run_lucid_apps.sh

```bash
./SystemTests/CI/run_lucid_apps.sh --docker                    # ../apps
./SystemTests/CI/run_lucid_apps.sh --suite unit                # component tests
./SystemTests/CI/run_lucid_apps.sh --docker --filter tutorials # a subset
```

`--suite` picks the app tree and the report directory:

| Suite | App tree | Report directory |
|------------|-------------------------|----------------------------------|
| `external` | `../apps` (default) | `SystemTests/reports/external-apps/` |
| `system` | `SystemTests/apps` | `SystemTests/reports/system-apps/` |
| `unit` | `Common/Components/*/*/test[s]/*` | `SystemTests/reports/unit-tests/` |

Each run writes `SystemTests/reports/<suite>/<UTC-timestamp>/` containing `summary.md`,
`results.csv`, `junit.xml` and `logs/<app>.log`. Runs accumulate, so the
directory is the history. JUnit XML is there because Jenkins, GitLab and GitHub
all consume it directly; the markdown is there so a run is readable in an editor
and diffable against the previous one.

### States

| State | Meaning | JUnit |
|--------------|--------------------------------------------------|----------|
| `RUNS` | parsed, still alive at the timeout | pass |
| `EXITED` | parsed, then exited by itself | pass |
| `PARSE-FAIL` | kernel rejected the SODL; error text in report | failure |
| `CRASH` | killed by SIGSEGV/SIGABRT/SIGBUS/SIGFPE/SIGILL | error |
| `NO-RUN` | never reported a completed parse | error |
| `NO-ERT1` | no SODL v1 export — skipped | skipped |

Two ordering constraints, both learned the hard way:

- **Console records are checked before the exit status.** A parse failure leaves
  the kernel alive waiting for a new app, so it hits the timeout exactly like a
  healthy app. Reading the exit status first would classify every parse failure
  as `RUNS`.
- **`ehs.exe` is launched directly, not via `bin/run_ehs.sh`.** That wrapper runs
  `./ehs.exe || echo "Exiting ehs.exe"`, which discards the exit status that
  `CRASH` detection needs. The runner does **not** reproduce the wrapper's
  `INXWAREROOT`/`DEVMANCOREDIR`/`SYSDATA` exports — `ehs.exe` never reads them.

### Why `--docker`

`ehs.exe` for the debian13 targets links OpenCV 4.10 from its build image. An
Ubuntu 24.04 host has 4.06, so every app fails to load with a linker error.
`--docker` runs it in the build image instead. The pre-flight resolves missing
libraries once and names any it cannot satisfy, so a wrong `--image` is obvious
immediately instead of arriving as N identical failures.

Library resolution matches ELF `e_machine`: contrib carries the same
`libtensorflowlite_c.so` filename for every architecture, and taking the first
match by name picks an arm64 object for an x86-64 build.

`SDL_VIDEODRIVER` defaults to `offscreen`, so LVGL/SDL targets need no display
and do not open a window per app. `--sdl-driver x11` to watch them.

### Staging

The env tree is copied to `../TARGET_TREES/ehs-env-test-<target>` and the app
under test is written into that copy's `appdata/default`. The real
`ehs_env-<target>` is never touched, so a default app installed there survives.
The script creates `appdata/`, `userdata/` and `devman/core/` in the copy —
`make targetenv` for the debian13 targets produces only `bin/` and `sysdata/`.

### Known limitation

The runner consumes an **export**, not a `.lpj`. Lucid produces the export, so an
app whose export is not checked in cannot be run here. Of 183 `.lpj` under
`../apps`, 111 have `export-ert1/` and 54 have a v0-only `export/`.

---

## Open decisions

### How an app should report its run state

Today: kernel console records, matched as strings. `messages.h` notes that Lucid
Tools already keys off `Parsing DONE`, so they are a de-facto interface rather
than incidental log text. They only reach stdout when
`EHS_RUNTIME_LOGGER_ENABLED` is set; the runner detects the all-quiet case and
says the results are unreliable rather than reporting them.

The state machine the runner is reconstructing **already exists in the kernel**:
`EhsHAppLoadStatusNotify()` fires `EHS_APP_LOAD_{STARTED,SUCCESFULL,FAILED,
RESTARTING}` (`Common/HAL/include/hal.h`), and `EhsHSetAppLoadStatusCallback()`
delivers them to the platform. Only the surfacing is missing, and it is
registered solely under `EHS_MAIN_LOOP_ITERATIVE` (the Qt path) in
`target/os-arch/gnu_ALL/target_main.c`.

So the cheap, honest change is to register that callback unconditionally on
`gnu_ALL` and have it write the status where a test harness can read it when an
environment variable asks for it. Roughly 20 lines in one target file: no kernel
change, no CDF, no new build option.

Argued against a dedicated build option: shared HAL and target code has to work
on every target, and a test-only build produces a binary that is not the one
shipped — the harness would then be validating something else. An environment
variable rather than an `argv` switch because `EhsHStoreArgInfo()` already
consumes `argv`.

### How an app should report pass/fail

A file-write FB writes `<userdir>/inx-tests/results/{test_result.txt,test_done}`,
where `<userdir>` derives from `$HOME` (`hal_file.c`,
`EhsHUpdateFilePathEnvironment`). The runner points `HOME` into its staging copy,
so results are per-run and the real home is untouched — the old runner did
`rm -rf ${HOME}/inxware/inx-tests` on every invocation.

**The pre-created directory depth must be exact.** The FB creates `results/`
itself and needs its parent present, and it fails if `results/` already exists:

| Pre-created | Result |
|--------------------------------|-----------------|
| `inxware/` | nothing written |
| `inxware/inx-tests/` | **writes** |
| `inxware/inx-tests/results/` | nothing written |

One level too few or too many and the app silently produces no result, which
looks like a failing test rather than a broken harness. The old script got this
right with a bare `mkdir -p …/inx-tests` and no comment explaining why.

`INXWAREROOT`/`DEVMANCOREDIR`/`SYSDATA` are **not** set by the runner, but not
for path-resolution reasons: `ehs.exe` never reads them. They exist for the env
tree's own shell scripts (`run_ehs.sh`, `runOsInit.sh`, `stop_ehs.sh`,
`devman/core/*`). The runtime locates `sysdata/` from its install path.

It works, but `$HOME` does not exist on MCU targets and the format carries no
structure beyond whole-file equality.

A dedicated reporting FB writing structured records into the env tree's
`userdata/` would fix both — the harness already owns that directory, and it
exists on every target that has a filesystem. Reporting to a server is the wrong
primary channel, since networking is not available on all targets; and targets
with no filesystem at all leave the console as the only channel, which is a
further argument for the console path staying primary.

### Consolidating the test apps — done

`tests/root/` is gone. Its 17 apps split by whether they have an owning function
block:

- **8 single-subject** → `Common/Components/<cat>/<fb>/tests/<name>/`, beside the
  block's C source, so block and test change in one diff.
- **9 family or cross-cutting** → `SystemTests/apps/`. These had no owning block:
  `math_int` exercises 12 `operator_*ix` directories and `string_functions` up to
  40 `stringfn_*` ones.

Discovery is a glob, not a list, so a test added beside a new block is picked up
automatically. Both `tests/` and `test/` are matched — 34 components use the
former, 8 the latter, and that inconsistency is worth resolving.

Reports moved to `SystemTests/reports/` so that `SystemTests/apps/` holds only
apps.

### Overlaying with the build/feature matrix

`SystemTests/CI/generate_feature_matrix.sh` reports which function blocks are
buildable per platform. Overlaying "buildable" with "tested" needs a shared key,
and there is not one yet: the matrix keys on (function block × platform), the app
runner on (app × platform). Joining them means deriving the function blocks each
app uses from its SODL. That derivation is the actual work item — worth costing
before promising a combined view.

---

## What was removed, and why

`ci_test_run_apps` (`ci_functions.sh`) was a third mechanism keyed on
`userdata/test.pass`. It could never have worked: `SYSTEM_TESTS_DIR` pointed at
`SystemTests/LucidTestApps/DefaultTestApp/export/`, which is not in the
repository, so its first `cp -f` failed and it returned 1 every time. Deleted;
`run_regression.sh --run-apps` now calls the engine.

`targetenv_run_tests.sh` did work. It was replaced only after the engine matched
its verdicts on all 17 apps then in `tests/root`, with two differences, both
improvements:

- Three tests it reported as `TIMEOUT` are actually `PARSE-FAIL` — they need
  `scheduler`, `calibrate` and `PID`, which exist in `Common/Components/user/`
  but are not enabled on the target. Checking the console records before the exit
  status is what surfaces the real cause.
- One more test passes, because its result arrived just after the old script's
  fixed 10 s ceiling.

It also had two hazards the engine drops: `pkill ehs.exe` (kills every eRT on the
machine, not just its own) and `rm -rf ${HOME}/inxware/inx-tests`.
