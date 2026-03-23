# xcore_freertos-xcore — OS/Arch Build Configuration

This directory contains the inxware eRT build system integration for XMOS xcore.ai targets
running FreeRTOS via the XMOS `fwk_rtos` framework.

## Architecture overview

```
Docker container (XTC Tools installed)
    │
    ├── xcc (compiler)     ← compiles inxware C sources
    ├── xcc (linker)       ← links to produce .xe firmware image
    │
    ├── INC_DIRS → ert-contrib-middleware/target_libs/xcore_freertos-xcore-xtc-15.x/build/include/
    └── LIB_DIRS → ert-contrib-middleware/target_libs/xcore_freertos-xcore-xtc-15.x/build/lib/
                        ├── libfreertos.a       (from fwk_rtos)
                        ├── librtos_support.a   (from fwk_rtos)
                        ├── libxcore_math.a     (from lib_xcore_math)
                        ├── libi2c.a            (from lib_i2c)
                        ├── libuart.a           (from lib_uart)
                        └── ...
```

## Key differences from GNU toolchain targets

Unlike ESP32 or Zephyr targets which use `gcc`/`clang`-based cross-compilers, the XMOS `xcc`
compiler has specific requirements:

1. **`XMOS_TOOL_PATH`** — `xcc` is not self-contained. It requires `XMOS_TOOL_PATH` set to the
   XTC Tools installation directory to locate xcore device target files (`.xn`), builtins, and
   toolchain libraries. This is set as a Docker image `ENV` variable in the platform `Dockerfile`
   (baked in at `make build_docker_local` time) so it is always available inside the container
   without needing a runtime `-e` flag.
   If you need to override `XMOS_TOOL_PATH` at runtime (e.g. to test a different XTC version),
   add it to `INX_ERTCOMPONENTS_BUILDENV` in
   `target/envbuildscripts/target_buildenv_run_command.sh` — see `BUILDING.md` for the general
   rule on passing host variables into the Docker container.

2. **Output format** — `xcc` links to `.xe` (XMOS executable) format rather than ELF. The
   `EXE=xe` variable is set accordingly. Use `xflash` to program the device.

3. **No `gnu_ALL/toolchain.mk` include** — `xcc` does not follow GNU toolchain conventions.
   This `toolchain.mk` stands alone rather than delegating to `gnu_ALL`.

4. **Board target file** — Each XMOS platform requires a board `.xn` XML file describing tile
   layout and memory. This is passed to `xcc` via `-target <BOARD_XN_FILE>` or
   `-target <XMOS_BOARD_NAME>`. Platform-specific `config.mk` files set `XMOS_BOARD_TARGET`.

5. **XC source files** — XMOS `.xc` files (channel-based concurrency extensions) may appear in
   `fwk_rtos` glue code. `xcc` compiles both `.c` and `.xc` natively. No special handling is
   needed in the Make rules.

## Build workflow

### Normal build (using Docker)

```bash
# One-time setup: download XTC Tools from https://www.xmos.com/software-tools/
# (requires XMOS developer account). Rename the Linux archive to xtc-tools-linux.tgz
# and place it at target/platform/xcore_freertos-xcore-base/xtc-tools-linux.tgz

./configure xcore_freertos-xcore-base   # or a board-specific platform
make prepdeps                            # clones ert-build-support and ert-contrib-middleware
make build_docker_local                  # builds the Docker image locally (one-time per XTC version)
make all_docker                          # builds inside the platform's Docker container
```

`make all_docker` reads `Dockerimagename` from the platform directory, starts the container
with the inxware workspace mounted at `/inxware`, and runs `make -j 8` inside it. The container
has XTC Tools pre-installed and `XMOS_TOOL_PATH` set.

**Important — do not publish this image to DockerHub.** Because the Docker image contains
XMOS XTC Tools binaries, distributing it via a public registry would violate the XMOS license.
Each developer must build it locally with `make build_docker_local`. This is different from
other inxware targets (e.g. ESP32) where the compiler comes from `ert-build-support` and the
Docker image itself contains no proprietary binaries.

### First-time SDK library build (one-time, per XTC Tools version)

The pre-built `.a` files and headers for `fwk_rtos` and the XMOS peripheral libraries are
checked in to `ert-contrib-middleware/target_libs/xcore_freertos-xcore-xtc-15.x/build/`.
If that directory is empty (new checkout, or XTC Tools version upgrade), the libraries must
be built inside the Docker container and the artifacts committed to `ert-contrib-middleware`.

**This work is done in the `ert-contrib-middleware` repository**, not here.
See `ert-contrib-middleware/contrib/xmos-sdk/README.md` for the full procedure.

#### Packages required (clone into `ert-contrib-middleware/contrib/xmos-sdk/`)

| Directory | Repository | Purpose |
|-----------|-----------|---------|
| `fwk_rtos/` | github.com/xmos/fwk_rtos | FreeRTOS SMP kernel, lwIP, FatFS, RTOS support layer |
| `xcommon_cmake/` | github.com/xmos/xcommon_cmake | CMake build infrastructure (build-time only) |
| `lib_xcore_math/` | github.com/xmos/lib_xcore_math | VPU-accelerated maths |
| `lib_i2c/` | github.com/xmos/lib_i2c | I2C master/slave |
| `lib_uart/` | github.com/xmos/lib_uart | Async UART TX/RX |
| `lib_i2s/` | github.com/xmos/lib_i2s | I2S audio |

#### What gets built and where it lands

```
ert-contrib-middleware/target_libs/xcore_freertos-xcore-xtc-15.x/build/
├── include/
│   ├── FreeRTOS/          ← FreeRTOS.h, task.h, timers.h, semphr.h, …
│   ├── rtos_support/      ← XMOS RTOS support layer headers
│   └── xcore/             ← xcore intrinsics headers
└── lib/
    ├── libfreertos.a
    ├── librtos_support.a
    ├── libxcore_math.a
    ├── libi2c.a
    ├── libuart.a
    └── libi2s.a
```

#### Quick steps (run from `ert-components` with the xcore target configured)

```bash
# 1. Clone SDK sources into ert-contrib-middleware (host, outside Docker)
cd ../ert-contrib-middleware/contrib/xmos-sdk
git clone https://github.com/xmos/fwk_rtos.git
git clone https://github.com/xmos/xcommon_cmake.git
git clone https://github.com/xmos/lib_xcore_math.git
git clone https://github.com/xmos/lib_i2c.git
git clone https://github.com/xmos/lib_uart.git
git clone https://github.com/xmos/lib_i2s.git

# 2. Start the build container
cd ../../ert-components
make target_buildenv

# 3. Inside the container — build all libraries
cd /inxware/ert-contrib-middleware/contrib/xmos-sdk
./build-xmos-libs.sh

# 4. Exit container; commit built artifacts to ert-contrib-middleware
cd ../ert-contrib-middleware
git add target_libs/xcore_freertos-xcore-xtc-15.x/
git commit -m "feat(xmos): add pre-built SDK libs for XTC 15.x"
```

### Flash firmware

```bash
# TBD: targetenv_xcore.sh (equivalent of targetenv_esp32.sh)
# Then: xflash --target XCORE-AI-EXPLORER ehs_xcore_freertos-xcore-base.xe
```

## File reference

| File | Purpose |
|------|---------|
| `toolchain.mk` | `CC_OVERRIDE`/`LINK_OVERRIDE` binary selection (gnu_ALL pattern); `XMOS_TOOL_PATH` absolute-path prepending; `xcc` flags; INC/CFLAGS/LNKFLAGS expansion |
| `target.mk` | Object files, include paths, library flags for the RTOS and peripheral libs |
| `config.mk` | Default feature flags for all xcore FreeRTOS targets |

## XMOS SDK sources (ert-contrib-middleware)

The MIT-licensed XMOS libraries live in `ert-contrib-middleware/contrib/xmos-sdk/`:

| Library | GitHub | Purpose |
|---------|--------|---------|
| `fwk_rtos` | github.com/xmos/fwk_rtos | FreeRTOS SMP port for xcore.ai |
| `lib_xcore_math` | github.com/xmos/lib_xcore_math | VPU-accelerated maths |
| `lib_i2c` | github.com/xmos/lib_i2c | I2C master/slave |
| `lib_uart` | github.com/xmos/lib_uart | UART TX/RX |
| `lib_i2s` | github.com/xmos/lib_i2s | I2S audio |
| `xcommon_cmake` | github.com/xmos/xcommon_cmake | CMake build infrastructure (build-time only) |

All are MIT licensed. Pre-built `.a` files and headers are committed to
`ert-contrib-middleware/target_libs/xcore_freertos-xcore-xtc-15.x/build/`.

## XTC Tools licensing

XTC Tools (the `xcc` compiler and associated tools) are proprietary XMOS software. They are
**not** redistributed in any inxware repository. The Docker image downloads and installs them
from the official XMOS release at image build time. See the platform `Dockerfile` for details.
