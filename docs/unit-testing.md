# Host unit testing in eRT components

How the host unit tests are put together, what they do and do not pick up from
the eRT configuration, and where the approach is expected to change.

> **This is a first pass, not a settled design.** One suite exists. It was
> written to get a specific piece of code (the printf/scanf format handling in
> `hal_string.c`) under test quickly, and its structure was copied from the EHS
> kernel's `UnitTests/data_string`. It is deliberately minimal. See
> [Expected refactoring](#expected-refactoring) before extending it — several of
> the choices below are provisional and worth changing once there is more than
> one suite to generalise from.

## What exists

| Location | Suite | Status |
|---|---|---|
| `UnitTest/hal_string/` | `hal_string.c` format handling | **Runs.** 119 checks |
| `UnitTest/class_tests/` | C++ widget/console/graphics cases | Orphaned, no build |
| `UnitTest/FunctionLibrary/` | C++ function library cases | Orphaned, no build |
| `../EHS-kernel/UnitTests/data_string/` | kernel string-row accessors | Runs; the pattern this copies |

```bash
make unit_tests        # every suite with a 'run' target
make unit_tests_asan   # the same under AddressSanitizer + UndefinedBehaviorSanitizer
```

`make unit_tests` globs `UnitTest/*/Makefile`, so a new suite is picked up
purely by existing. Nothing is registered centrally.

## How a suite is built

Four rules, all of them a reaction to how the main build works:

1. **Compile the real source, never a copy.** The suite lists the actual
   `.c` file in `SRCS`. A copied fixture drifts and then tests nothing.
2. **No framework and no external dependency.** Plain C, a `CHECK` macro, a
   failure counter, non-zero exit. Nothing to install, so it drops into CI or a
   developer shell identically.
3. **Stub the few symbols the unit needs.** `hal_string.c` needs exactly one
   (`EhsHMem_Alloc`); the kernel's `data_string.c` needs the connection table
   and target config. If a file needs a long stub list it is usually telling
   you the file has too many dependencies to be a unit — that is a signal about
   the source, not about the test.
4. **Name the host compiler explicitly.** See the trap below.

### The `HOSTCC` trap

The top-level build exports `CC` and `CFLAGS` for the **cross** toolchain. A
suite Makefile that uses `$(CC)` builds correctly when you run it by hand in its
own directory, and then cross-compiles — and fails to link — the moment it runs
under `make unit_tests`:

```
xtensa-esp32s3-elf/bin/ld: ... __getreent is not implemented and will always fail
collect2: error: ld returned 1 exit status
```

So a suite must use its own variable names:

```make
HOSTCC     ?= cc
TESTCFLAGS := -std=c99 -Wall -Wextra -g
```

`UnitTest/hal_string/Makefile` does this. `EHS-kernel/UnitTests/data_string/Makefile`
still uses `$(CC)` and would hit the same failure if it were ever invoked from a
parent build; it is only ever run directly today.

## What the tests inherit from the eRT configuration

This is the part worth being precise about, because the tests are **not**
completely detached from eRT config — but they are not driven by it either.

### Picked up

The suite puts the platform's own config directory on the include path, so
`globals.h` → `target.h` → `target_config.h` resolves to a real eRT platform:

```make
PLATFORM ?= linux_x86_64_clang_gtk_debian10
INCLUDES := -I$(ERTCOMP_ROOT)/target/platform/$(PLATFORM) ...
```

That means the size constants and base types under test are the real ones, and
`PLATFORM=` is a meaningful knob. Each platform's `target_config.h` includes
either `base_full/base_config.h` or `base_small/base_config.h`, and that choice
changes the constants that most memory-safety bugs in this tree turn on:

| `PLATFORM=` | base | `EHS_STRING_LENGTH_MAX` | `EHS_DATA_TABLE_STRING_DEFAULT_LENGTH` |
|---|---|---:|---:|
| `linux_x86_64_clang_gtk_debian10` | full | 2048 | 32768 |
| `linux_arm_gtk_gst` | full | 2048 | 32768 |
| `esp32s3_freertos-xtensa-hrdcv2C` | small | 256 | 256 |
| `nxp_arm` | small | 256 | 256 |

So `make PLATFORM=nxp_arm` exercises the MCU size constants on the host, which
is how the format-buffer sizing was checked against both bases without a
device. Do that before assuming a fix is size-independent.

### Not picked up

- **`TARGET.cfg` and `./configure`.** The suite does not read the selected
  target. `PLATFORM` is its own variable and defaults to a Linux platform
  regardless of what the tree is configured for.
- **`config.mk` feature flags.** Module enables, `EHS_DEBUG_*`, logging levels
  and the rest of the per-target `-D` set are not passed. The suite hard-codes
  the one flag it needs (`-DEHS_TARGET_FP_SUPPORT`). Code behind a
  configuration `#ifdef` is therefore **untested unless the suite defines the
  flag itself**.
- **The os-arch layer.** `INCLUDES` always names `linux_ALL`, `gnu_ALL` and
  `base_full`, whatever `PLATFORM` says. `target_types.h` in `gnu_ALL` /
  `linux_ALL` also hard-includes `../base_full/base_types.h` by relative path,
  so the `ehs_*` typedefs always come from `base_full`. This is harmless today
  because `base_full/base_types.h` and `base_small/base_types.h` declare
  identical typedefs — but it is an assumption, not a guarantee, and it would
  break quietly if they ever diverged.
- **The target toolchain and ABI.** Everything is built with the host compiler
  for the host ABI. Pointer size, struct padding and stack frame sizes are host
  figures, not device figures.

In short: **the tests are host-native and Linux-only, but the configuration
constants are real and selectable.** They will catch a bound computed from the
wrong constant. They will not catch anything that depends on the target
toolchain, the target ABI, or a build-flag combination the suite does not set.

## Host requirements

Linux with `cc` (gcc or clang) and GNU make. Nothing else.

macOS is untested and would currently fail: `INCLUDES` names `linux_ALL`
unconditionally. Making it work is a matter of selecting the os-arch directory
rather than hard-coding it — see below.

## Related tooling

| Command | Purpose |
|---|---|
| `make unit_tests` | host unit tests |
| `make unit_tests_asan` | the same under ASan + UBSan |
| `make static_analysis` | cppcheck over the tree, using `suppressions.txt` |
| `make stack_report` | per-function stack frames, largest first |
| `make targetenv_run_tests` | on-target regression tests (needs a built target tree) |

Bounds tests should allocate **exact-sized heap buffers** rather than generous
stack arrays, so ASan traps an overrun that an assertion would miss. The
`hal_string` bounds cases use 4-byte rows for this reason.

`make stack_report` takes `STACK_SRCS` and `STACK_THRESHOLD`; a non-zero
threshold makes it exit non-zero, so it works as a CI gate. Frame sizes are
ABI-dependent, so the host figure is for comparison only — for the number that
matters on a device, build the real target and read the cross-compiler's
`.su` files with
`scripts/software-utilities/stack_report.sh --prebuilt`.

## Expected refactoring

The current arrangement is one suite's worth of structure, generalised no
further than it had to be. Known places it should change:

- **Split common from target-specific.** Everything today is implicitly
  "common code, host-built". There is no place for a test that must run against
  a particular target's os-arch layer, HAL implementation or toolchain. The
  likely shape is `UnitTest/common/` for host-buildable target-independent
  units, and `UnitTest/target/<os-arch>/` for anything that has to be built by
  the cross toolchain and run on device or in an emulator, hooked into
  `targetenv_run_tests` rather than `unit_tests`.
- **Stop hard-coding the os-arch include set.** It should follow `PLATFORM` the
  way `target_config.h` already does, which would also make macOS hosts work.
- **Share the boilerplate.** Every suite currently repeats the same `INCLUDES`
  block, `HOSTCC`/`TESTCFLAGS` and `run`/`asan`/`clean` targets. A single
  included `UnitTest/suite.mk` would leave a suite Makefile as little more than
  its `SRCS` and its stubs, and would fix the `HOSTCC` trap once rather than
  per suite.
- **Feed the real build configuration in.** Passing the selected target's
  `config.mk` `-D` set would let tests cover code behind configuration
  `#ifdef`s. Today that code is simply not compiled by the tests.
- **Decide what to do with the orphaned C++ suites.** `class_tests` and
  `FunctionLibrary` have no build and are not run. Either revive them in this
  pattern or delete them — leaving them in place implies a coverage that does
  not exist.
- **Align with the kernel.** `EHS-kernel/UnitTests/` has the same structure and
  the same orphaned-suite problem. Whatever shape this settles into should be
  the same in both repos, since they are built and tested together.

None of this is required to add a suite now. Copy `UnitTest/hal_string/`, keep
the four rules above, and expect the Makefile to be replaced by shared
infrastructure later.

## Reference

- [`UnitTest/README.md`](../UnitTest/README.md) — running the suites, what each covers
- [`DEVELOPING.md`](../DEVELOPING.md) § Testing guidelines, Static analysis, Stack usage
- [`CLAUDE.md`](../CLAUDE.md) § "Never count `%` to decide how many arguments a format takes" — the invariants the `hal_string` suite exists to protect
