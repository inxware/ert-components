# eRT Build Approaches

This document describes how the eRT (event-driven Run-Time) build system
produces firmware for each supported target platform.

## Two Approaches to Building Executables

There are two distinct patterns used across platforms:

### Pattern A — Direct: `make all` does everything

`make all` (or `make all_docker`) compiles and fully links the final
deployable binary in one step.  `make targetenv` only stages files and
gathers assets — no additional linking occurs.

**Platforms:** Linux, Windows

---

### Pattern B — Indirect: `make all` compiles only; a targetenv step links and packages

`make all` compiles eRT source to an intermediate artifact (`.a`, `.elf`,
or `.exe` ELF).  A separate `make targetenv_*_docker` step uses a
target-specific toolchain or SDK to perform the final linking and/or image
generation.  This is necessary when:

- The target SDK owns the final link step (XMOS xcommon_cmake, Arduino CLI)
- Multiple binary components must be merged into a flashable image (ESP32)
- The output format is a loadable plugin rather than a standalone executable
  (Android `.so` / Unity)

**Platforms:** ESP32-S3, Arduino Image, Android/Unity, XMOS xcore

---

## Per-Platform Build Stage Breakdown

| Platform | `make all` output | Final link in `make all`? | `make targetenv_*` role | Final deployable |
|---|---|---|---|---|
| **Linux** (arm64/x86_64) | ELF executable (`.exe`) | Yes — full link | Stage binary + assets only | `ehs.exe` (ELF) |
| **Windows** (x86 MinGW) | PE executable (`.exe`) | Yes — full link | Stage binary; `targetenv_nsis_docker` builds installer | `ehs.exe` or NSIS `.exe` |
| **Android / Unity** | Shared library (`.so`) | Yes — shared link | Stage `.so`; `targetenv_apk_docker` compiles Java wrapper via Gradle | `.apk` |
| **ESP32-S3** | Xtensa ELF (`.exe`) | Yes — to ELF | `targetenv_esp32_docker` merges bootloader + partition + app + LittleFS → flashable image | `ehs.img` |
| **Arduino Library** | Static archive (`.a`) | Yes — archive only | `targetenv_arduino_docker` packages into ZIP for Arduino IDE | `eRT_Package.zip` |
| **Arduino Image** | ARM ELF (`.elf`) | Yes — to ELF | `targetenv_arduino_docker` re-compiles via `arduino-cli` → UF2 flash image | `ehs.uf2` |
| **XMOS xcore** | Static archive (`.a`) | No — archive only | `targetenv_xmos_docker` runs xcommon_cmake to link SDK + eRT `.a` → `.xe` | `ehs.xe` |

> **Note on `.exe` extension:** Linux and ESP32 targets use the `.exe`
> extension for the intermediate ELF binary.  This is a legacy naming
> convention — the file is a native ELF, not a Windows PE executable.

---

## Notes per Platform

### Linux
Straightforward Pattern A.  The compiler (clang or gcc) compiles and links
in `make all`.  `make targetenv` copies the binary and config assets into
`../TARGET_TREES/ehs_env-<target>/`.  Optional `make targetenv_deb` builds
a Debian package.

### Windows
Pattern A using MinGW cross-compiler.  `make all` produces a Windows PE
binary.  `make targetenv_nsis_docker` optionally wraps it in an NSIS
installer.  No additional linking in `targetenv`.

### Android / Unity
Pattern B.  `make all` links eRT as a shared library (`.so`) using
`-shared` and `override EXE=so` set in the android toolchain.  The `.so`
is an eRT plugin, not a standalone executable.  `make targetenv_apk_docker`
invokes Gradle/Android Studio to compile the Java/Kotlin wrapper and package
the `.so` into an `.apk`.  A 64-bit dependency library is also required
alongside the 32-bit primary build.

### ESP32-S3
Pattern B.  `make all` compiles eRT source with the Xtensa toolchain and
links to an ELF.  `make targetenv_esp32_docker` then:
1. Converts ELF → `.bin` with `esptool.py elf2image`
2. Fetches pre-built bootloader and partition table from `ert-contrib-middleware`
3. Builds a LittleFS filesystem image
4. Merges all components into a single flashable `.img` using `esptool.py merge_bin`

The final `.img` is what gets flashed to the device via USB or OTA.

### Arduino Library
Pattern B (archive variant).  `make all` archives eRT objects into a static
`.a` using the ARM Cortex-M0+ toolchain.  `make targetenv_arduino_docker`
packages the `.a` and headers into a ZIP suitable for the Arduino IDE
library manager.  The eRT code is linked later by the end-user's sketch
build — eRT provides the library, not a complete firmware image.

### Arduino Image
Pattern B (re-compile variant).  `make all` compiles and links eRT to an
ELF using clang.  `make targetenv_arduino_docker` then invokes `arduino-cli
compile` to recompile the sketch using the Arduino build system, producing
a `.uf2` UF2 image that can be flashed by drag-and-dropping onto the
RP2040 bootloader mass-storage device.

### XMOS xcore
Pattern B (two-phase SDK link).  This is the most complex indirect build:

**Phase 1 — `make all` / `make all_docker`:**
eRT source files are compiled with `xcc` and archived into a static library
using `xmosar` (the XMOS archiver).  This step only requires the SDK
*headers* (pre-staged in `ert-contrib-middleware/target_libs/`).

**Phase 2 — `make targetenv_xmos_docker`:**
Runs `targetenv_xmos.sh` inside the XMOS Docker container.  This invokes
`cmake` on a minimal xcommon_cmake wrapper app
(`ert-contrib-middleware/contrib/xmos-sdk/xcore-ehs-app/`) that:
- Loads the xcommon_cmake build system (`include(xcommon.cmake)`), which
  sets up `xcc` as the compiler and `XCORE_XS3A` as the target
- Declares `fwk_rtos`, `fwk_io`, and `fwk_core` as module dependencies
  (resolved from `XMOS_SANDBOX_DIR` — no network access needed)
- Links the Phase 1 `.a` against the SDK modules to produce `ehs.xe`
- Copies `ehs.xe` to `../TARGET_TREES/ehs_env-<target>/bin/ehs.xe`

The reason for this two-phase split is that the XMOS SDK (`fwk_rtos`) is
not a standalone CMake target — it is designed to be consumed by an
xcommon_cmake application project that sets up the `XCORE_XS3A` platform.
Trying to drive the SDK build directly (without xcommon_cmake) results in
missing cmake platform files and the FreeRTOS port not being activated.

---

## Build Commands Quick Reference

```bash
# Linux / Windows — Pattern A
make all_docker          # compile + link → final binary
make targetenv           # stage assets (Linux/Windows only, no extra linking)

# ESP32-S3 — Pattern B
make all_docker          # compile + link → intermediate ELF
make targetenv_esp32_docker  # merge → flashable .img

# Arduino Library — Pattern B
make all_docker          # compile + archive → .a
make targetenv_arduino_docker  # package → ZIP

# Arduino Image — Pattern B
make all_docker          # compile + link → ELF
make targetenv_arduino_docker  # arduino-cli recompile → .uf2

# Android / Unity — Pattern B
make all_docker          # compile + link → .so plugin
make targetenv_apk_docker  # Gradle build → .apk

# XMOS xcore — Pattern B (two-phase SDK link)
make all_docker          # compile + archive → ehs_$(TARGET).a
make targetenv_xmos_docker  # xcommon_cmake link → ehs.xe
```
