# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is the **ert-components** repository - the core runtime components for the inxware eRT (event-based RunTime) system. eRT executes no-code applications created in the Lucid IDE on 10+ platforms from bare-metal microcontrollers (ESP32) to Linux servers.

### What is inxware?

inxware is a complete no-code embedded development ecosystem:
- **Lucid IDE** (https://appland.inxware.io/) - Visual programming environment for creating applications
- **eRT Runtime** - Event-driven runtime (this repository) consisting of:
  - EHS Kernel (proprietary binary, free up to 10 devices)
  - Component library (open source LGPLv3, this repo)
  - HAL implementations for each platform
- **Applications** - SODL files (.sdl) created in Lucid, executed by eRT
- **Devman** - Optional IoT device management platform

## Essential Build Commands

The build system uses Make + Docker with a "sticky" target configuration stored in `TARGET.cfg`.

### Initial Setup & Configuration

```bash
./configure                           # List all available targets
./configure linux_x86_64_clang        # Select target (saves to TARGET.cfg)
make prepdeps                         # Clone dependencies (~40GB, run once for all targets)
```

### Building

```bash
make all_docker                       # Build in Docker (recommended)
make all                              # Build on host (requires toolchain)
make clean                            # Clean build artifacts
```

### Creating Runtime Packages

```bash
make targetenv                        # Assemble runtime in ../TARGET_TREES/ehs_env-[TARGET]/
make targetenv_deb                    # Create Debian package
make targetenv_apk_docker             # Create Android APK
make targetenv_esp32_docker           # Create ESP32 firmware
make targetenv_make_nsis              # Create Windows installer
```

### Testing & Development

```bash
make targetenv_run_tests              # Run unit tests on assembled runtime
./SystemTests/CI/regression_test-published-only.sh  # Full regression suite
make static_analysis                  # Static code analysis
make depend                           # Update deps.mk (run after modifying components)
make toolsenv_update                  # Update Lucid IDE with new/modified components
```

### Configuration Utilities

```bash
./configure -edit                     # Edit target/platform/[TARGET]/config.mk
./configure -run                      # Run Linux target on build host
./configure -debug                    # Debug with GDB
./configure -pushd                    # cd to ../TARGET_TREES/ehs_env-[TARGET]/
./configure -pushd-config             # cd to target/platform/[TARGET]/
make target_buildenv                  # Interactive shell in Docker build container
```

### External Dependencies (Auto-managed)

`make prepdeps` clones two large repositories into parent directory:
- **ert-build-support/** (~20GB) - Binary toolchains from https://github.com/inxware/ert-build-support
- **ert-contrib-middleware/** (~15GB) - Pre-built libraries from https://github.com/inxware/ert-contrib-middleware
- **apps/** (optional) - Demo applications from https://github.com/inxware/apps

## Architecture Overview

### How eRT Works

1. **Application Creation**: User creates visual program in Lucid IDE
2. **Export**: Lucid exports application as SODL file (.sdl)
3. **Deployment**: SODL file placed in `apps/default/t.sdl` alongside eRT runtime
4. **Execution**: `ehs.exe` loads SODL, instantiates components, runs event loop
5. **Component Library**: Components (in this repo) provide all functionality

### Directory Structure

```
Common/                              # Core runtime code
├── Components/                      # Component implementations by category
│   ├── core/                        # Operators, buffers, timers, file I/O, type conversion
│   ├── gui/                         # Display drivers, buttons, text boxes, graphics
│   ├── networking/                  # HTTP, MQTT, TCP/UDP, WebSocket, SSL
│   ├── media/                       # Audio/video codecs, GStreamer integration
│   ├── ml/                          # TensorFlow Lite, neural networks
│   ├── mv/                          # Image processing, AprilTag detection
│   ├── peripherals/                 # GPIO, ADC/DAC, UART, I2C, SPI, PWM
│   ├── AV/                          # Audio/Video (legacy, platform-specific)
│   └── deprecated/                  # Backward compatibility components
├── HAL/                             # Hardware abstraction interface definitions
├── KAPI/                            # Kernel API headers
└── Ehs/                             # Event handling system integration code

target/                              # Platform-specific implementations
├── platform/[target_name]/          # Complete target configurations (~40 platforms)
│   ├── config.mk                    # Build configuration variables
│   ├── Dockerfile                   # Container definition for reproducible builds
│   └── Dockerimagename              # DockerHub reference for pre-built image
└── os-arch/[os]-[arch]/             # HAL implementations per OS/architecture
    ├── target.mk                    # Build rules
    ├── toolchain.mk                 # Compiler/linker configuration
    ├── target_main.c                # Application entry point
    ├── target_time.c                # Timer/clock HAL implementation
    ├── target_process.c             # Threading/mutex primitives
    ├── target_file.c                # File system operations
    └── target_gpio.c                # GPIO HAL (if applicable)

scripts/                             # Build utilities and deployment tools
├── build-deploy/                    # Platform-specific deployment scripts
│   ├── esp32/                       # Flash, monitor, debug for ESP32
│   ├── linux-general/               # SSH deployment for Linux devices
│   └── packagers/                   # DEB, RPM, NSIS packaging tools
├── build-function-library/          # Reusable shell functions for builds
└── software-utilities/              # CDF validation, SODL conversion

../TARGET_TREES/ehs_env-[TARGET]/    # Build output (created by make targetenv)
├── bin/ehs.exe                      # Runtime executable
├── apps/default/t.sdl               # Default application (or your app)
└── lib/                             # Runtime libraries (if needed)
```

### Component System

Each component consists of:
- **`.cdf`** - XML descriptor defining inputs/outputs/events for Lucid IDE
- **`.c/.h`** - C/C++ implementation
- **`.bmp`** - 24x24 icon for Lucid IDE
- **`.idf.ini`** - IDE interface settings
- **`tests/`** - Lucid application test cases (SODL files)
- **`help/`** - HTML documentation shown in IDE

To add a component to `Common/Components/[category]/`, add the `.c` file to `components.mk` in that directory, run `make depend`, then `make toolsenv_update` to publish to Lucid IDE.

## Build System Details

### Key Make Variables

These variables are set by `target/platform/[TARGET]/config.mk` and used throughout the build:

- **`TARGET`** - Platform name (from `./configure`, stored in `TARGET.cfg`)
- **`TARGET_NAME`** - Binary name: `ehs_${TARGET}`
- **`EHS_ARCH`** - Architecture: `x86`, `arm64`, `xtensa`, etc.
- **`EHS_OS`** - Operating system: `linux`, `freertos`, `windows`, etc.
- **`EHS_GNU_ARCH`** - GNU triplet architecture: `x86_64`, `arm64`, etc.
- **`EHS_GNU_OS`** - GNU triplet OS: `linux-gnu`, etc.
- **`EHS_ROOT_PATH`** - This repository root
- **`EHS_PLATFORM_PATH`** - Current platform config: `target/platform/${TARGET}/`
- **`EHS_COMPONENT_SUPPORT_BASE`** - Path to `ert-contrib-middleware`
- **`EHS_CORE_SUPPORT_BASE`** - Path to `ert-build-support`

Feature flags configured per platform:
- **`EHS_GUI_SUPPORT`** - `gtk`, `lvgl`, `qt`, `none`
- **`EHS_COMPONENT_NETWORKING_SUPPORT`** - `all`, `minimal`, `none`
- **`EHS_MEDIA_SUPPORT`** - `all`, `gstreamer`, `none`
- **`EHS_PERIPHERALS_GPIO_SUPPORT`** - HAL implementation: `sysfs_linux_arm`, `esp32`, etc.
- **`EHS_DEBUGALL`** - `true` for debug builds

### Docker Workflow

Most targets use Docker for reproducible builds:
- `Dockerfile` in each platform directory defines build environment
- `Dockerimagename` specifies pre-built image from DockerHub
- Commands with `_docker` suffix run in container (e.g., `make all_docker`)
- `make target_buildenv` starts interactive shell in container

### Code Style & Conventions

**C/C++ Naming:**
- Functions: `lowercase_with_underscores`
- Types: `lowercase_t` (suffix)
- Constants: `UPPERCASE_WITH_UNDERSCORES`
- HAL functions: `hal_` prefix (e.g., `hal_gpio_write`, `hal_log_error`)

**Indentation:** 4 spaces, K&R style braces

**Adding New Components:**

1. Create files in `Common/Components/[category]/`:
   - `my_component.cdf` - XML descriptor
   - `my_component.c/.h` - Implementation
   - `my_component.bmp` - 24x24 icon
   - `my_component.idf.ini` - IDE settings
2. Add `.c` file to `components.mk` in that directory
3. Run `make depend` to update dependencies
4. Run `make toolsenv_update` to push to Lucid IDE
5. Test: `make all_docker && make targetenv && make targetenv_run_tests`

**Porting to New Platform:**

1. Create `target/platform/my_new_platform/` with `config.mk`
2. Implement HAL in `target/os-arch/my_os-my_arch/`:
   - `target.mk`, `toolchain.mk` - Build configuration
   - `target_main.c` - Entry point
   - `target_time.c`, `target_process.c`, `target_file.c`, `target_gpio.c` - HAL implementations
3. Configure: `./configure my_new_platform`
4. Test: `make prepdeps && make all && make targetenv_run_tests`

## Important Notes

- **Target selection is "sticky"** - `./configure [target]` saves to `TARGET.cfg`, persists across make invocations
- **Always use `_docker` variants** for reproducible builds (e.g., `make all_docker` not `make all`)
- **Run `make depend` after modifying components** - Updates `deps.mk` files with source dependencies
- **Run `make toolsenv_update` to publish components** - Syncs CDF files to Lucid IDE development environment
- **EHS Kernel is proprietary binary** - Located in `ert-build-support`, linked during build, not source code
- **Version strings** - Managed in `Releases/version_strings`, updated by `make targetenv_version`