# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is the **ert-components** repository, which contains the core components needed for the inxware eRT (event-based RunTime) system. eRT is designed to run no-code applications on embedded devices and various computing systems including servers, edge compute, and desktop platforms.

## Build System

The project uses a sophisticated Make-based build system with Docker support for cross-platform compilation.

### Key Commands

**Configuration:**
- `./configure` - List available build targets
- `./configure [target]` - Configure for specific platform (e.g., `linux_x86_64_clang`)
- `./configure -edit` - Edit current target configuration
- `./configure -run` - Run the target on current host
- `./configure -debug` - Debug the target with GDB

**Building:**
- `make help` - Show all available build targets and options
- `make prepdeps` - Install dependencies and checkout required repositories
- `make all` - Build the eRT binary (ehs_[TARGET].exe)
- `make all_docker` - Build using Docker environment
- `make clean` - Clean build artifacts

**Runtime Environment:**
- `make targetenv` - Create runtime file structure in staging directory
- `make targetenv_version` - Create new version and tag commit
- `make targetenv_package` - Create target-specific package
- `make targetenv_deb` - Create Debian package (Linux targets)
- `make targetenv_apk` - Create Android APK
- `make targetenv_esp32` - Build ESP32 firmware image

**Testing:**
- `make targetenv_run_tests` - Run regression tests
- `make static_analysis` - Run static analysis suite

**Development:**
- `make depend` - Update source dependencies (WARNING: modifies deps.mk files)
- `make toolsenv_update` - Update development tools with new component descriptions

### Build Dependencies

The build system requires several adjacent repositories:
- `ert-build-support` - Binary toolchains and build dependencies
- `ert-contrib-middleware` - Pre-built 3rd party dependencies

These are automatically cloned by `make prepdeps` and require ~40GB of disk space.

## Architecture

### Directory Structure

- **Common/** - Core eRT components and libraries
  - **Components/** - Reusable functional components organized by category
    - **core/** - Essential components (operators, buffers, converters, etc.)
    - **gui/** - Graphics and UI components  
    - **networking/** - Network communication components
    - **media/** - Audio/video processing components
    - **ml/** - Machine learning components
    - **mv/** - Machine vision components
  - **HAL/** - Hardware Abstraction Layer
  - **KAPI/** - Kernel API headers and interfaces
  - **Ehs/** - Event handling system core

- **target/** - Platform-specific build configurations
  - **platform/** - Target configuration files (config.mk per platform)
  - **os-arch/** - OS and architecture specific code
  - **envtree/** - Runtime environment templates
  - **Component-HAL/** - Hardware abstraction components

### Component System

eRT uses a component-based architecture where functionality is provided through:
- **CDF files** (Component Description Files) - Define component interfaces
- **C/C++ implementations** - Actual component logic
- **Bitmap files** - Visual representation for IDE tools

Components are categorized into functional groups (core, networking, gui, etc.) and can be combined to create applications.

### Build Targets

The system supports numerous build targets including:
- Linux (x86, ARM, x86_64) with various GUI toolkits
- Android (ARM, ARM64) 
- Windows (x86)
- ESP32/ESP32-S3 (FreeRTOS)
- Arduino platforms
- Bare metal embedded systems

Each target has its own configuration in `target/platform/[target_name]/`.

## Development Workflow

1. **Setup:** Run `./configure [target]` and `make prepdeps`
2. **Build:** Use `make all` or `make all_docker` 
3. **Test:** Create runtime with `make targetenv`, then `make targetenv_run_tests`
4. **Package:** Use target-specific packaging commands (`make targetenv_deb`, etc.)

## Important Notes

- The project requires a proprietary event handling kernel library
- Docker is used extensively for reproducible cross-platform builds
- Version information is managed in `Releases/version_strings`
- Component definitions can be updated in development tools with `make toolsenv_update`
- Always run `make prepdeps` when switching to a new target platform