# Overview to building eRT

Below are some detailed instructions on how to use the eRT Components configuration and build system.

- [Prerequisites](#prerequisites)
- [Smoke test](#smoke-test)
- [Build status](#build-status)
- [Host pre-requisites](#host-pre-requisites)
- [Repository pre-requisites](#repository-pre-requisites)
- [Build workflow overview](#build-workflow-overview)
- [Repository setup](#repository-setup)
- [Docker configuration](#docker-configuration)
- [Target platform matrix](#target-platform-matrix)
- [Standard build commands](#standard-build-commands)
- [Docker-based building](#docker-based-building)
- [Package creation commands](#package-creation-commands)
- [Example builds](#example-builds)
- [Troubleshooting](#troubleshooting)
- [Docker troubleshooting](#docker-troubleshooting)
- [Basic runtime deployment](#basic-runtime-deployment)
- [Running with custom applications](#running-with-custom-applications)
- [Platform-specific examples](#platform-specific-examples)
- [Integration examples](#integration-examples)
- [Performance monitoring](#performance-monitoring)

## Prerequisites

| Requirement | Minimum                             | Recommended                        |
| ----------- | ------------------------- --------- | ---------------------------------- |
| **OS**      | Linux (Ubuntu >= 18.04) or WSL2     | Ubuntu 20.04+ or Debian 11+        |
| **RAM**     | 8GB                                 | 16GB+                              |
| **Storage** | 50GB free                           | 100GB+ SSD                         |
| **CPU**     | 4 cores                             | 8+ cores                           |
| **Network** | Broadband internet                  | High-speed for container downloads |

## First Build

The instructions below will allow you to get a clean build environment set up and then run a single test build to ensure everything is working. Within a Linux bash shell, enter the following commands:

   ```bash
   mkdir inxware && cd inxware

   # Clone the main `ert-components` repository
   # If you prefer HTTPS:
   git clone -depth 1 https://github.com/inxware/ert-components.git
   #
   # If you prefer SSH:
   git clone -depth 1 git@github.com:inxware/ert-components.git
   cd ert-components

   # Configure the build for your chosen target platform
   ./configure  # List all available targets
   ./configure linux_x86_64_clang  # Configure for Linux x64 - test on build machine

   # Install the build dependencies
   make prepdeps                   # Downloads toolchains and dependencies (~40GB) 

   # Build the runtime binary
   make all_docker                 # Build using containerised environment

   # Create a staging directory for later packaging
   make targetenv                  # Assemble the runtime environment in ../TARGET_TREES/ehs-env_linux_x86_64_clang
   ```

**Success!** You now have a working eRT runtime. Try the [Lucid IDE](https://appland.inxware.io/) to create your first no-code application.

## Build status

Per-target badges, and what each one covers, are in the
[README build status table](README.md#github-build-status). Every badge is a
clean-checkout build of that target on each push to `main`; click one for its
log.

Two things worth knowing before reading a red badge as a code defect:

- A target must be listed in `scripts/git-utilities/publish_set.txt` and have
  been published, or `configure` fails on the mirror because its platform
  directory is not there.
- Some targets cannot be built in public CI at all for licensing reasons — the
  Nordic NCS ones most notably. See
  [`docs/ert-porting-guide.md`](docs/ert-porting-guide.md) § *Publishing and
  licensing*.

Adding a workflow: [`docs/github-actions-ci.md`](docs/github-actions-ci.md).

## Host pre-requisites

- Git with LFS support
- GNU Make (included in the `build-essential` Debian package)
- Docker (ideally the latest stable release)
- Python 3 (Optional - used for creating documentation and new components)

## Repository pre-requisites

The `make prepdeps` command will clone the dependency repos for you.

- **ert-build-support** (~20GB): Binary toolchains and build tools
- **ert-contrib-middleware** (~15GB): Pre-built 3rd-party libraries
- **apps** (optional): Demo applications and examples

## Build workflow overview

   ```text
   Configure → Install Deps → Build         → Package         → Deploy
       ↓           ↓             ↓                ↓                ↓
   ./configure  make prepdeps  make all_docker  make targetenv   build-deploy/*.sh
   ```

## Detailed Build Step Explaination

1. **Clone the main eRT repository**
   ```bash
   mkdir inxware && cd inxware

   git clone --depth 1 https://github.com/inxware/ert-components.git
   cd ert-components
   ```
You don't need to clone the repositiry with depth 1, but it saves space.
`ert-compoents` is where the inxware runtime build system and all open source components are located.

2. **Select target and download dependencies**
   ```bash
   ./configure linux_x86_64_clang  # or your preferred target
   make prepdeps                   # Downloads dependencies (may take some time!)
   ```
After this you will find `ert-build-support` (toolchains and ert-kernels) and `ert-contrib-middleware` (pre-built 3rd-party OSS component libraries & source)
3. **Build ert-components from source**
  ```bash
   make all_docker                 # Builds in the current directory, but in a specific docker environment for the toolchain.
   ```
If you have the toolchain for your target platform already installed you can simply use
  ```bash
   make all                        # Builds in the current directory in your host .
   ```


## Docker configuration

For optimal performance, configure Docker as follows:

   ```bash
   # Increase docker resources (if using docker desktop)
   # RAM: 8gb minimum, 16gb recommended
   # CPU: all available cores
   # Disk: 60gb minimum
   
   # For linux: add user to docker group
   sudo usermod -aG docker $USER
   # Logout and login again for changes to take effect
   ```

## Target platform matrix

| Platform Category    | Architecture  | Operating System                  | Status    | Package Format  | Use Cases                  |
|--------------------- |-------------- |---------------------------------- |---------- |---------------- |--------------------------- |
| **Desktop/Server**   | x86_64        | Linux (Debian 9-12, Ubuntu 14-24) | ✅ Stable | DEB, Binary     | Development, Server apps   |
|                      | x86_64        | Windows 7-11                      | ✅ Stable | EXE, MSI        | Desktop applications       |
| **Single Board**     | ARM64         | Linux (Raspberry Pi 3-5)          | ✅ Stable | DEB, Image      | IoT gateways, edge compute |
|                      | ARM64         | Android (Rock Pi, Radxa)          | ✅ Stable | APK             | Media players, kiosks      |
|                      | ARM32         | Linux (Various SBCs)              | ✅ Stable | DEB, Image      | Industrial controllers     |
| **Microcontrollers** | Xtensa        | ESP32/ESP32-S3 (FreeRTOS)         | ✅ Stable | Firmware        | IoT sensors, edge devices  |
|                      | ARM Cortex-M  | NXP Kinetis (FreeRTOS)            | ✅ Stable | Firmware        | Industrial automation      |
|                      | ARM Cortex-M  | Arduino (MBED, Native)            | ✅ Stable | Firmware        | Prototyping, education     |
| **Mobile/Gaming**    | ARM           | Unity Plugin                      | ✅ Stable | Unity Package   | Games, interactive apps    |
|                      | ARM64         | Android NDK                       | ✅ Stable | AAR, SO         | Mobile applications        |

![Static Badge](https://img.shields.io/badge/RaspberryPi-Y-green) 
![Static Badge](https://img.shields.io/badge/Arduino-Y-green) 
![Static Badge](https://img.shields.io/badge/esp32-Y-green) 
![Static Badge](https://img.shields.io/badge/esp32s3-Y-green)
![Static Badge](https://img.shields.io/badge/Windows-Y-green) 
![Static Badge](https://img.shields.io/badge/Android-Y-green) 
![Static Badge](https://img.shields.io/badge/x86_linux-Y-green)
![Static Badge](https://img.shields.io/badge/arm_freeRTOS-Y-green) 
![Static Badge](https://img.shields.io/badge/arm_linux-Y-green) 
![Static Badge](https://img.shields.io/badge/RISCV-Y-green)

# Building the inxware runtime 

## Standard build commands

1. Select a target platform
   ```bash
   ./configure                           # Show all the platform targets configured to build
   ./configure linux_arm_lvgl_myproduct  # Example configuring the build system 
                                         #(Don't worry this is not autotools!)
   ```

2. Useful configuration tricks (optional)
   ```bash
   ./configure -edit                  # Edit target configuration ./target/platform/<...>/config.mk
   ./configure -run                   # Run a linux target on the build host
   ./configure -debug                 # Debug a linux target with GDB on he build host
   make target_buildenv               # starts a prompt in the Docker container environment. (then you can use make all )
   ```

3. Building eRT
   ```bash
   make prepdeps                     # Install dependencies (run once works for all targets)
   make clean                        # Clean any previous build artifacts
   make all_docker                   # Build the current target using Docker
   ```

4. Build maintenance (optional)
   ```bash
   make depend                       # Update source dependencies!!! (deps.mk)
   make static_analysis              # Run code analysis
   make toolsenv_update              # Update Lucid IDE with new components
   # Runs a collection of target build tests (it may try and run the linux ones.)
   ./SystemTests/CI/regression_test-published-only.sh 
   # Redisplay the results of the last build regression test. 
   ./SystemTests/CI/display_regression_tests.sh 
   ```

## Docker-based building

For consistent, reproducible builds most targets have docker option that will run the command in the relevant Container defined by `./target/patform/<..>/Dockerimagename`. This should typically be run in preference to using the host machine's toolchain.

   ```bash
   make all_docker                     # Build in container
   make targetenv_make_deb_docker      # Package Debian in container
   make targetenv_make_apk_docker      # Package Android in container
   ```

Note: some targets that don't use any non-standard Linux commands (like `make targetenv`) don't require a dockerized toolchain.

### Passing environment variables into the Docker container

Docker containers do **not** automatically inherit the host shell or Make environment. Any Make or bash variable that the build needs inside the container must be explicitly listed in the `INX_ERTCOMPONENTS_BUILDENV` string inside:

```
target/envbuildscripts/target_buildenv_run_command.sh
```

Each variable is added as `-e VARIABLE_NAME` in the `docker run` invocation. If a new platform or toolchain requires an additional variable (e.g. a licence path, SDK root, or board selection flag), add it to that list or the build will silently see an empty value inside the container.

Variables set directly in the Dockerfile with `ENV` are baked into the image and are always available without being listed in `INX_ERTCOMPONENTS_BUILDENV`.

## Package creation commands

1. Runtime environment build and packaging
   ```bash
   make targetenv                 # Assemble runtime files
   make targetenv_version         # Create versioned release
   make targetenv_package         # Create target-specific package
   ./SystemTests/CI/run_lucid_apps.sh --suite unit       # Run regression tests
   ```

2. Alternative package formats (optional)
   ```bash
   make targetenv_deb             # Debian package (.deb)
   make targetenv_apk             # Android package (.apk)
   make targetenv_esp32           # ESP32 firmware (.bin)
   make targetenv_make_nsis       # Windows installer (.exe)
   ```

## Example builds

Builds a PC linux runtime with the clang compiler for debian11 systems using GTK for UI elements, gstreamer for media processing and greengrass for IoT and Edge-compute.

### Building for Linux desktop

   ```bash
   ./configure linux_x86_64_clang_gtk_gst_gg_debian11
   make prepdeps
   make all_docker
   make targetenv
   make targetenv_deb              # Create Debian package
   ```

### Building for ESP32 microcontroller

   ```bash
   ./configure esp32s3_freertos-xtensa-community
   make prepdeps
   make all_docker
   make targetenv
   make targetenv_esp32_docker            # Create firmware image
   ```

### Building for Android

   ```bash
   ./configure linux_android_arm64
   make prepdeps
   make all_docker
   make targetenv
   make targetenv_apk_docker              # Create Android APK
   ```

### Building for Arduino

   ```bash
   ./configure arduino_arduino-mbed-nano_community
   make prepdeps
   make all_docker
   make targetenv
   make targetenv_arduino          # Create Arduino library
   ```

## Troubleshooting

### Common issues

**"Permission denied" with Docker**

   ```bash
   sudo usermod -aG docker $USER
   # Logout and login again
   ```

**"No space left on device"**

   ```bash
   # Clean docker cache
   docker system prune -a
   # Check disk space
   df -h
   ```

**Git LFS download failures**

   ```bash
   # Install/update git lfs
   git lfs install
   git lfs pull
   ```

**Build dependencies missing**

   ```bash
   # Reinstall dependencies
   make clean
   rm -rf ../ert-build-support ../ert-contrib-middleware
   make prepdeps
   ```

## Docker troubleshooting

Dockerhub is used to store pre-built containers and distribute them to build environments, where they may be cached on local machines.

Dockerhub images can be created from Dockerfiles in ert-components and published to docker with the following commands:

   ```bash
   # Clean docker state
   make publish_docker_image
   ```

Some direct docker commands that may be is use:

   ```bash
   # Clean docker state
   docker system prune -a
   docker builder prune
   ```

# Usage examples

## Basic runtime deployment

After building, your eRT runtime will be in the staging directory:

   ```bash
   cd ../TARGET_TREES/ehs-env_[your-target]/
   ./bin/ehs.exe                    # Run eRT runtime
   ```

The minimal deployment includes:

   ```make
   bin/ehs.exe                      # eRT runtime executable
   apps/default/t.sdl               # Default Lucid application (SODL format)
   lib/                             # Runtime libraries (if needed)
   ```

## Running with custom applications

1. **Create application in Lucid IDE**
   - Open [Lucid IDE](https://appland.inxware.io/)
   - Design your application visually
   - Export as SODL file (.sdl)

2. **Deploy to eRT runtime**
   ```bash
   # Copy your application
   cp myapp.sdl ../TARGET_TREES/ehs-env_[target]/apps/default/t.sdl
   
   # Run your application
   cd ../TARGET_TREES/ehs-env_[target]/
   ./bin/ehs.exe
   ```

## Platform-specific examples

### Raspberry PI deployment

   ```bash
   # Build for raspberry pi
   ./configure linux_arm64_gtk_gst_gg_debian11
   make prepdeps && make all_docker
   make targetenv_deb

   # Install on target device
   scp ../TARGET_TREES/packages/*.deb pi@raspberrypi:~
   ssh pi@raspberrypi 'sudo dpkg -i *.deb'
   ```

### ESP32 flash and monitor

   ```bash
   # Build esp32 firmware
   ./configure esp32s3_freertos-xtensa-community
   make prepdeps && make all_docker
   make targetenv_esp32

   # Flash to device (requires esptool)
   esptool.py --port /dev/ttyUSB0 write_flash 0x0 firmware.bin

   # Monitor serial output
   screen /dev/ttyUSB0 115200
   ```

### Android installation

   ```bash
   # Build android apk
   ./configure linux_android_arm64
   make prepdeps && make all_docker
   make targetenv_apk_docker

   # Install via adb
   make upload_ehs_via_adb                               # This will install the apk, plus the supervisor 
   adb install ../TARGET_TREES/packages/*.apk            # Or you can go native and run this for just one package.
   adb shell am start -n com.inxware.ert/.MainActivity   # and run the apps remotely,
   ```

## Integration examples

### Unity game engine plugin

   ```bash
   # Build unity plugin
   ./configure linux_android_arm_unity-lib
   make prepdeps && make all_docker

   # Plugin files created in:
   ../target_trees/unity-plugins/
   # Copy to unity project assets/plugins/
   ```

### Web dashboard integration

   ```javascript
   // Connect to eRT via HTTP API
   fetch('http://device-ip:8080/api/status')
     .then(response => response.json())
     .then(data => {
       console.log('Device status:', data);
       updateDashboard(data);
     });
   ```

## Performance monitoring

### Runtime debugging

   ```bash
   # Run with debug logging
   ./configure -debug
   # Or set environment variable
   EHS_LOG_LEVEL=DEBUG ./bin/ehs.exe
   ```

### System resource monitoring

   ```bash
   # Monitor cpu/memory usage
   top -p $(pgrep ehs.exe)

   # Monitor network activity
   netstat -tulpn | grep ehs
   ```

# Further reading

- **[Build System Reference](docs/ert-build-guide.md)**: Detailed build system documentation

## Attribution

*Copyright © 2008–2025 inx Limited. The community **eRT Components** release is open‑source under **LGPLv3**. The **EHS Kernel** is proprietary and licensed separately as described in [LICENSE.md](LICENSE.md).*
