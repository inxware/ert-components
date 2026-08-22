# Scripts Directory

This directory contains development utilities, build helpers, and deployment scripts for the eRT runtime system.

---

## Quick guide

All examples are run from the **repo root** (`ert-components/`).

### Regression CI (build reporting)

```bash
# Build every platform, generate SBOM per platform and a feature matrix
./SystemTests/CI/run_regression.sh --discover --generate-sbom --generate-matrix all

# Build the published list, compile only (fast check, no packaging)
./SystemTests/CI/run_regression.sh --platform-list published --compile-only

# Full nightly with smoke tests, SBOM and matrix
./SystemTests/CI/run_regression.sh --platform-list published \
    --run-apps --generate-sbom --generate-matrix all

# See SystemTests/CI/README.md for the full option reference
```

### Feature compliance matrix

```bash
# Markdown table — all components vs all platforms (omit always/never rows)
python3 scripts/software-utilities/fb_platform_matrix.py \
    --format md --no-always --no-never

# Filter to a single component family
python3 scripts/software-utilities/fb_platform_matrix.py \
    --format md --component 'accel_gyro'

# All three formats at once → matrix.{csv,md,html} in cwd
python3 scripts/software-utilities/fb_platform_matrix.py --format all
```

### SBOM — platform dependency tagging

```bash
# Generate SBOM for the currently configured platform
./configure <platform>
make sbom
# Writes: ert-components/sbom/<platform>/SBOM.spdx
#         ert-components/sbom/<platform>/SBOM_SUMMARY.md
#         <dep-root>/DEPENDENTS.md  (appended — builds up a cross-platform matrix)
```

### CDF / component ID validation

```bash
# Check all function-block IDs are unique and match any header #defines
python3 scripts/inxware-id-tool/check_cdf_hashes.py Common/Components

# Generate/update a NameHash_CRC16 for a new class name
python3 scripts/inxware-id-tool/inxtool.py -genHash MyNewClass
```

### Component authoring tools

```bash
# Suggest Block geometry before writing a CDF <Block> section
python3 scripts/software-utilities/cdf_geometry.py suggest \
    --labels "enable" "error id" "done" --last-y 32

# Validate geometry of a written CDF
python3 scripts/software-utilities/cdf_geometry.py validate \
    Common/Components/peripherals/my_block.cdf

# Generate skeleton C + header from a CDF
python3 scripts/software-utilities/cdf_to_c_skel.py \
    Common/Components/peripherals/my_block.cdf

# Generate ASCII diagram + markdown docs from a CDF
python3 scripts/software-utilities/cdf_to_ascii.py \
    Common/Components/peripherals/my_block.cdf
```

### Build & deployment helpers

```bash
# Deploy a Linux build to a remote device via SSH
./scripts/build-deploy/linux-general/deployViaSsh.sh

# Flash an ESP32 (from build-deploy/esp32/ — see its README for setup)
./scripts/build-deploy/esp32/esp32_flash.sh

# Run a command inside the Docker build container for the active platform
./scripts/docker-utilities/run_as_docker.sh make all
```

---

## Directory Overview

### build-function-library/

Reusable shell functions for build automation.  Source these files from other scripts to get consistent build operations across platforms.

- `colour.sh` — Terminal colour/text styling abstraction (ANSI + tput). Exports `TXT_FG_*`, `TXT_BG_*`, style variables, and helper functions: `heading()`, `warn()`, `err()`. Respects `NO_COLOR`. Sourced by `ci_functions.sh` and `regression_test-published-only.sh`.
- `build-utilities.sh` — Common build helpers: `cdBuildTreeRoot()` walks up the directory tree to find the repo root (locates `./configure`).
- `debian-utilities.sh` — Debian package build functions: `BuildDebian()` (clean → all\_docker → targetenv → targetenv\_deb\_docker), `UploadDebian-Devman()`.
- `android-utilities.sh` — Android APK build functions: `BuildAndroid()` (clean → all\_docker → targetenv → targetenv\_apk), `UploadApk-ambifier()`, `InstallToDevice()` (ADB wrapper).
- `ci_functions.sh` — **Shared CI regression library** sourced by `SystemTests/CI/run_regression.sh`. Provides all build, test, and reporting functions used by the regression framework. See *Key CI functions* below.

#### Key CI functions (`ci_functions.sh`)

| Function | Purpose |
|---|---|
| `ci_setup_paths` | Initialises `ROOT_DIR`, `CI_RESULT_DIR`, `TARGET_TREE_DIR`, `SYSTEM_TESTS_DIR`, `TOOL_TEST_EHS_VARIANT` |
| `ci_skip_platform <name>` | Returns 0 (skip) for `.svn`, `depricated`, `base*` |
| `ci_setup_platform_dir` / `ci_teardown_platform_dir` | Creates fresh results dir; touches/removes `building.flag` |
| `ci_remove_build` | Removes `ehs.exe` and `*.so` from the target tree |
| `ci_test_build` | Checks for `ehs.exe` or `ehs_<platform>.so` |
| `ci_build_target` | Full build: clean → configure → Docker-or-host → targetenv → APK → Unity |
| `ci_display_log_tail` | Prints last 20 lines of a log to the console |
| `ci_on_build_failure <log> <stage>` | Invokes `claude --print` or interactive session if enabled |
| `ci_run_preflight [root]` | Runs `check_cdf_hashes.py`; returns 1 on failure |
| `ci_generate_sbom` | Runs `make sbom` for the current platform (guards on `TARGET.cfg` match); writes `sbom.pass/.fail/.skip` |
| `ci_generate_matrix [fmt]` | Runs `fb_platform_matrix.py`; writes output to `CI_RESULT_DIR` |
| `ci_show_summary` | Colourised `ls` of all pass/fail results |
| `ci_exit_code` | Returns 1 if any `.fail` flag exists in the results tree |

---

### build-deploy/

Platform-specific deployment and packaging scripts organised by target.  Each script wraps a sequence of `make` commands (clean, all\_docker, targetenv, upload).  No shared library integration; they call `cdBuildTreeRoot()` from `build-utilities.sh`.

**android/** — ADB install + Ambifier server upload (2 scripts)  
**appland/** — `build_upload_all.sh`  
**arduino/** — `flash-rp2040-pico.sh`  
**backer-hrdx/** — 20 device-specific scripts (HeatRod, inx displays, NXP IoT; JTAG flashing)  
**esp32/** — Build, flash, monitor, OTA upload, exception parsing (10 scripts + venv setup). See `build-deploy/esp32/README.md`.  
**heatrodCommissioning/** — Build, flash, monitor, label printing, JTAG  
**linux-general/** — `deployViaSsh.sh` — SSH/SCP deployment to a remote Linux device  
**moodsonic-tsa/** — Project-specific build + upload (7 scripts)  
**tellisign/** — Platform variants (2 scripts)  
**unity/** — Unity license setup + host installation (2 scripts)  

---

### docker-utilities/

- `run_as_docker.sh` — Execute commands inside the Docker build containers used for cross-compilation.

---

### software-utilities/

Development and validation tools for working with CDF files, component identifiers, and binary data. Full usage documentation is in `scripts/software-utilities/README.md`.

**Key reporting tools:**

- **`fb_platform_matrix.py`** — Generates a function-block × platform support matrix showing which HAL variant (or `—`) each component has on each target platform. Output format: `csv` (default, stdout), `md`, `html`, or `all` (writes `matrix.{csv,md,html}` to cwd). Used by `ci_generate_matrix` in the regression framework; also useful for manual auditing of HAL coverage. See *Feature compliance matrix* below.
- **`cdf_geometry.py`** — Block geometry calculator/validator for CDF files. Called by Claude Code and developers before and after writing a CDF `<Block>` section.
- **`cdf_to_ascii.py`** — Generates ASCII art diagrams and markdown docs from CDF files. Integrated into `make components_gendocs`.
- **`cdf_to_c_skel.py`** — Generates skeleton C/header files from a CDF definition. Offers to add new components to `components.mk`.
- **`cdf_validate.py`** — Validates consistency between CDF files and C implementation code (function ID matching).
- **`sodl_bin_to_c_file.py`** — Converts binary files to C byte-array source files for embedding.
- **`move_description_column_final.py`** — Reformats the parameter table layout in component `help.html` files.

#### Feature compliance matrix (`fb_platform_matrix.py`)

```bash
# All components vs all platforms — CSV to stdout:
python3 scripts/software-utilities/fb_platform_matrix.py > matrix.csv

# Markdown, omitting always-built and never-built rows:
python3 scripts/software-utilities/fb_platform_matrix.py --format md --no-always --no-never

# Single component family across all platforms:
python3 scripts/software-utilities/fb_platform_matrix.py --format md --component 'accel_gyro'

# All three formats at once (writes matrix.csv / matrix.md / matrix.html in cwd):
python3 scripts/software-utilities/fb_platform_matrix.py --format all

# Via the CI regression framework (output goes to SystemTests/CI/results/):
./SystemTests/CI/run_regression.sh --discover --generate-matrix md
```

Cell values: `always` (unconditionally built), a HAL variant name (e.g. `sferalabs`, `stubbed`), `—` (not built), `?` (support variable present but not set).

---

### ai-utilities/

Machine learning model preparation tools.

**apriltag-model-trainer/** — AprilTag detection model training scripts  
**hailo-utils/** — Hailo AI accelerator model conversion and deployment  
**yolo-model-utils/** — YOLO object detection model preparation and conversion  

---

### inxware-id-tool/

Tools for generating and validating hash-based identifiers for eRT function blocks.  See `inxware-id-tool/README.md` for full usage.

- `inxtool.py` — Main entry point: generate `NameHash_CRC16` for a class name (`-genHash`) or batch-update ID defines in `.c`/`.h` files.
- `hash.py` — CRC-16/MCRF4XX and CRC-32 hash algorithm implementations.
- `check_cdf_hashes.py` — Regression checker: verifies every CDF's `NameHash_CRC16` is unique across all CDFs, and that any matching `INXWARE_FB_ID_<Class>` / `EHS_FB_ID_<Class>` `#define` in a `.h` file agrees. Exits non-zero on mismatch. Run before any build or automatically by the CI pre-flight step.
- `cdf_validate.py` — Validates that function IDs in `.cdf` files match `EHS_FB_FUNCTION_ENTRY` entries in C source.

---

## Platform dependency tagging (`make sbom`)

The Makefile provides a `make sbom` target (implemented in `target/platform/sbom.mk`) that records exactly which `ert-contrib-middleware` and `ert-build-support` paths a platform consumes:

```bash
./configure <platform>
make sbom
```

**Output — three files per platform:**

| File | Location | Content |
|---|---|---|
| `SBOM.spdx` | `ert-components/sbom/<platform>/` | SPDX 2.3 tag-value forward SBOM |
| `SBOM_SUMMARY.md` | `ert-components/sbom/<platform>/` | Human-readable markdown dependency table |
| `DEPENDENTS.md` | `<dep-root>/DEPENDENTS.md` (appended) | Reverse record: which platforms use this dep root |

The `DEPENDENTS.md` files are written into the `ert-contrib-middleware` and `ert-build-support` trees (and the toolchain directory when applicable).  Running `make sbom` for every platform builds up a complete cross-platform usage matrix inside those sibling repos.

The `ert-contrib-middleware` path for a given platform is:
```
../ert-contrib-middleware/target_libs/<COMPONENT_BASE_TECHNOLOGIES>/build/
```
where `COMPONENT_BASE_TECHNOLOGIES` is set in `target/platform/<platform>/config.mk`.

---

## Notes for developers

- Most scripts expect to be run from specific working directories. Check the script header comments.
- Build function libraries use `cdBuildTreeRoot()` to locate the repository root automatically.
- ESP32 scripts require a Python virtual environment with esptool installed (see `build-deploy/esp32/README.md`).
- Platform-specific packagers are typically invoked via Makefile targets (e.g. `make targetenv_deb_docker`, `make targetenv_apk`).
- The regression entry point is `SystemTests/CI/run_regression.sh`; see `SystemTests/CI/README.md` for full workflow documentation.
