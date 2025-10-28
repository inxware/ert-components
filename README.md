# eRT Components
## Event-based Runtime for Embedded & IoT Systems
**eRT (event-based RunTime)** is an open-source library of software components that are pre-buildable ahead of developing embedded applications with a no-code IDE.
reRT is for eal-time, data-intensive embedded systems built with MCUs/CPUs/SoCs.
 
![Static Badge](https://img.shields.io/badge/RaspberryPi-Y-green) ![Static Badge](https://img.shields.io/badge/Arduino-Y-green) ![Static Badge](https://img.shields.io/badge/esp32-Y-green) ![Static Badge](https://img.shields.io/badge/esp32s3-Y-green) ![Static Badge](https://img.shields.io/badge/Windows-Y-green) ![Static Badge](https://img.shields.io/badge/Android-Y-green) ![Static Badge](https://img.shields.io/badge/x86_linux-Y-green) ![Static Badge](https://img.shields.io/badge/arm_freeRTOS-Y-green) ![Static Badge](https://img.shields.io/badge/arm_linux-Y-green) ![Static Badge](https://img.shields.io/badge/RISCV-Y-green)

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/inxware/ert-components/actions)
[![License](https://img.shields.io/badge/license-LGPLv3-blue)](LICENSE.md)
[![Platform Support](https://img.shields.io/badge/platforms-10+-green)](#supported-platforms)

# Github Build Status 

[![build-linux_arm64_lvgl_gg_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_lvgl_gg_debian11.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_lvgl_gg_debian11.yml)

[![build-linux_arm64_gtk_gst_gg_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_gtk_gst_gg_debian11.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_gtk_gst_gg_debian11.yml)

[![build-linux_android_arm64](https://github.com/inxware/ert-components/actions/workflows/build-linux_android_arm64.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_android_arm64.yml)

[![build-esp32s3_freertos-xtensa-community](https://github.com/inxware/ert-components/actions/workflows/build-esp32s3_freertos-xtensa-community.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-esp32s3_freertos-xtensa-community.yml)

[![build-linux_x86_64_clang_lvgl_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64-linux_x86_64_clang_lvgl_debian11-no-certs.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64-linux_x86_64_clang_lvgl_debian11-no-certs.yml)

[![build-linux_x86_64_clang_gtk_gst_gg_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64_clang_gtk_gst_gg_debian11-no-certs.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64_clang_gtk_gst_gg_debian11-no-certs.yml)

[![build-win_x86_32-lucid-win10](https://github.com/inxware/ert-components/actions/workflows/build-win_x86_32-lucid-win10.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-win_x86_32-lucid-win10.yml)

[![build-arduino-mbed-nano-community](https://github.com/inxware/ert-components/actions/workflows/build-arduino-mbed-nano_community.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-arduino-mbed-nano_community.yml)

### 🚀 Quick Links
- [Get Started in 5 minutes](#quick-start)
- [Browse Supported Platforms](#supported-platforms)
- [Read the Porting Guide](docs/ert-porting-guide.md)
- [Try the Free IDE](https://appland.inxware.io/)

---

## Table of Contents

- [Overview](#overview)
- [Key Features](#key-features)
- [Quick Start](#quick-start)
- [Supported Platforms](#supported-platforms)
- [Installation & Setup](#installation--setup)
- [Building](#building)
- [Usage Examples](#usage-examples)
- [Development](#development)
- [Documentation](#documentation)
- [Contributing](#contributing)
- [Support & Community](#support--community)
- [License](#license)


---

## Overview

The **eRT Components** repository contains the core runtime system for the inxware no-code development platform. It enables developers to create embedded applications using a visual, component-based approach without traditional coding.

### What is eRT?

eRT is a component-based runtime that executes applications created with the [inxware Lucid IDE](https://appland.inxware.io/). It provides:

- **Event-driven architecture** for real-time responsiveness
- **Hardware abstraction layer (HAL)** for cross-platform compatibility
- **Pre-built components** for common embedded tasks (GPIO, networking, UI, etc.)
- **No-code deployment** from visual applications to production firmware

### Architecture

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   Lucid IDE     │    │  eRT Components │    │  Target Device  │
│  (Visual Dev)   │───▶│   (Runtime)     │───▶│   (Hardware)    │
└─────────────────┘    └─────────────────┘    └─────────────────┘
```

The system consists of:
- **EHS Kernel**: Proprietary event handling system
- **Component Library**: Open-source functional components
- **HAL**: Hardware abstraction for target platforms
- **Build System**: Cross-platform compilation and packaging

---

## Key Features

### ✨ Cross-Platform Runtime
- **10+ architectures**: ARM, x86, RISC-V, Xtensa
- **Multiple OS support**: Linux, Android, Windows, FreeRTOS, Arduino
- **Unified build system** with Docker containerization

### 🔧 Rich Component Library
- **Core components**: Operators, buffers, timers, file I/O
- **Networking**: HTTP, MQTT, TCP/UDP sockets
- **Graphics & UI**: Display drivers, controls, imaging
- **Hardware interfaces**: GPIO, ADC/DAC, PWM, UART

### 🚀 Production Ready
- **Industrial deployment**: 10 device free commercial license
- **OTA updates**: Over-the-air firmware deployment
- **Package formats**: APK, DEB, firmware images, Unity plugins
- **CI/CD integration**: Automated testing and deployment

### 📱 No-Code Development
- **Visual programming**: Drag-and-drop component assembly
- **Real-time debugging**: Live system monitoring
- **Rapid prototyping**: Minutes from idea to running prototype

---

## Quick Start

### Prerequisites
- Linux (Debian/Ubuntu recommended) or Windows with WSL2
- Docker (installed automatically by `make prepdeps`)
- 40GB free disk space for build dependencies

### 5-Minute Setup

1. **Clone the repository**
   ```bash
   git clone https://github.com/inxware/ert-components.git
   cd ert-components
   ```

2. **Choose a target platform**
   ```bash
   ./configure  # List all available targets
   ./configure linux_x86_64_clang  # Configure for Linux x64
   ```

3. **Install dependencies**
   ```bash
   make prepdeps  # Downloads toolchains and dependencies (~40GB)
   ```

4. **Build the runtime**
   ```bash
   make all_docker  # Build using containerized environment
   ```

5. **Create deployable package**
   ```bash
   make targetenv          # Assemble runtime environment
   make targetenv_version  # Create versioned release
   ```

6. **Test your build**
   ```bash
   ./configure -run  # Run the built application
   ```

**🎉 Success!** You now have a working eRT runtime. Try the [Lucid IDE](https://appland.inxware.io/) to create your first no-code application.

---

## Supported Platforms

### Build Status
[![build-linux_arm64_lvgl_gg_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_lvgl_gg_debian11.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_lvgl_gg_debian11.yml)
[![build-linux_arm64_gtk_gst_gg_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_gtk_gst_gg_debian11.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_gtk_gst_gg_debian11.yml)
[![build-linux_android_arm64](https://github.com/inxware/ert-components/actions/workflows/build-linux_android_arm64.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_android_arm64.yml)
[![build-esp32s3_freertos-xtensa-community](https://github.com/inxware/ert-components/actions/workflows/build-esp32s3_freertos-xtensa-community.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-esp32s3_freertos-xtensa-community.yml)
[![build-linux_x86_64_clang_gtk_gst_gg_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64_clang_gtk_gst_gg_debian11.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64_clang_gtk_gst_gg_debian11.yml)
[![build-win_x86_32-lucid-win10](https://github.com/inxware/ert-components/actions/workflows/build-win_x86_32-lucid-win10.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-win_x86_32-lucid-win10.yml)
[![build-arduino-mbed-nano-community](https://github.com/inxware/ert-components/actions/workflows/build-arduino-mbed-nano_community.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-arduino-mbed-nano_community.yml)

### Platform Matrix

| Platform Category | Architecture | Operating System | Status | Package Format | Use Cases |
|-------------------|--------------|------------------|--------|----------------|-----------|
| **Desktop/Server** | x86_64 | Linux (Debian 9-12, Ubuntu 14-24) | ✅ Stable | DEB, Binary | Development, Server apps |
| | x86_64 | Windows 7-11 | ✅ Stable | EXE, MSI | Desktop applications |
| **Single Board** | ARM64 | Linux (Raspberry Pi 3-5) | ✅ Stable | DEB, Image | IoT gateways, edge compute |
| | ARM64 | Android (Rock Pi, Radxa) | ✅ Stable | APK | Media players, kiosks |
| | ARM32 | Linux (Various SBCs) | ✅ Stable | DEB, Image | Industrial controllers |
| **Microcontrollers** | Xtensa | ESP32/ESP32-S3 (FreeRTOS) | ✅ Stable | Firmware | IoT sensors, edge devices |
| | ARM Cortex-M | NXP Kinetis (FreeRTOS) | ✅ Stable | Firmware | Industrial automation |
| | ARM Cortex-M | Arduino (MBED, Native) | ✅ Stable | Firmware | Prototyping, education |
| **Mobile/Gaming** | ARM | Unity Plugin | ✅ Stable | Unity Package | Games, interactive apps |
| | ARM64 | Android NDK | ✅ Stable | AAR, SO | Mobile applications |

### Hardware Examples

#### 🖥️ **Development Platforms**
- Any x86_64 Linux/Windows PC
- Virtual machines and containers
- CI/CD build servers

#### 🔧 **Industrial & IoT**
- **Raspberry Pi**: All models (3, 4, 5, Zero)
- **Rock Pi**: 4A/4B/4C+, Rock 3C, Rock 5A/5B
- **ESP32 family**: ESP32, ESP32-S3, ESP32-C3
- **NXP Kinetis**: K64F, K66F, RT series
- **STM32**: F4, F7, H7 series (via Arduino)

#### 📱 **Consumer Devices**
- Android tablets and phones
- Set-top boxes and media players
- Digital signage displays
- Interactive kiosks

> **Need a new platform?** Check our [porting guide](docs/ert-porting-guide.md) or [request support](#support--community).

---

## Installation & Setup

### System Requirements

| Requirement | Minimum | Recommended |
|-------------|---------|-------------|
| **OS** | Linux (Ubuntu 18.04+) or Windows 10 with WSL2 | Ubuntu 20.04+ or Debian 11+ |
| **RAM** | 8GB | 16GB+ |
| **Storage** | 50GB free | 100GB+ SSD |
| **CPU** | 4 cores | 8+ cores |
| **Network** | Broadband internet | High-speed for container downloads |

### Dependency Installation

eRT automatically manages most dependencies through Docker containers. The `make prepdeps` command will install:

#### Host Dependencies
- **Git** with LFS support
- **GNU Make** (build-essential)
- **Docker** (latest stable)
- **Python 3** for build scripts

#### Repository Dependencies
- **ert-build-support** (~20GB): Binary toolchains and build tools
- **ert-contrib-middleware** (~15GB): Pre-built 3rd-party libraries
- **apps** (optional): Demo applications and examples

### Repository Setup

1. **Clone main repository**
   ```bash
   git clone https://github.com/inxware/ert-components.git
   cd ert-components
   ```

2. **Select target and initialize**
   ```bash
   ./configure linux_x86_64_clang  # or your preferred target
   make prepdeps                   # Downloads dependencies (~40GB)
   ```

3. **Verify installation**
   ```bash
   make help                       # Should show available build targets
   docker --version                # Verify Docker is working
   ```

### Docker Configuration

For optimal performance, configure Docker with:
```bash
# Increase Docker resources (if using Docker Desktop)
# RAM: 8GB minimum, 16GB recommended
# CPU: All available cores
# Disk: 60GB minimum

# For Linux: add user to docker group
sudo usermod -aG docker $USER
# Logout and login again for changes to take effect
```

### Troubleshooting Setup

#### Common Issues

**"Permission denied" with Docker**
```bash
sudo usermod -aG docker $USER
# Logout and login again
```

**"No space left on device"**
```bash
# Clean Docker cache
docker system prune -a
# Check disk space
df -h
```

**Git LFS download failures**
```bash
# Install/update Git LFS
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

---

## Building the inxware runtime 

### Build Workflow Overview

```bash
Configure  → Install Deps  →  Build         → Package     → Deploy
    ↓           ↓             ↓                  ↓           ↓
./configure  make prepdeps  make all_docker  make targetenv build-deploy/*.sh
```

### Build Commands

#### Choose a target
```bash
./configure                           # Show all the paltform targets configured to build
./configure linux_arm_lvgl_myproduct  # Example configuring the build system 
                                      #(Don't worry this is not autotools!)
```

#### Building
```bash
make prepdeps                     # Install dependencies (run once works for all targets)
make clean                        # Clean any previous build artifacts
make all_docker                   # Build the current target using Docker
make clean                        # Clean build artifacts
```

#### Quick Configuration Tricks
```bash
./configure -edit                  # Edit target configuration ./target/platform/<...>/config.mk
./configure -run                   # Run a linux target on the build host
./configure -debug                 # Debug a linux target with GDB on he build host
make target_buildenv               # starts a prompt in the Docker container environment. (then you can use make all )
```

#### Build Maintenance
```bash
make depend                       # Update source dependencies!!! (deps.mk)
make static_analysis              # Run code analysis
make toolsenv_update              # Update Lucid IDE with new components
# Runs a bunch of target build tests (it may try and run the linux ones.)
./SystemTests/CI/regression_test-published-only.sh 
# Redisplay the results of the last build regression test. 
./SystemTests/CI/display_regression_tests.sh 

```

### Package Creation

#### Runtime Environment
```bash
make targetenv                  # Assemble runtime files
make targetenv_version         # Create versioned release
make targetenv_package         # Create target-specific package
make targetenv_run_tests       # Run regression tests
```

#### Package Formats
```bash
make targetenv_deb             # Debian package (.deb)
make targetenv_apk             # Android package (.apk)
make targetenv_esp32           # ESP32 firmware (.bin)
make targetenv_make_nsis       # Windows installer (.exe)
```

### Docker-based Building

For consistent, reproducible builds most targets have _docker option that will run the command in the relevant Container defined by `./target/patform/<..>/Dockerimagename`:
```bash
make all_docker                     # Build in container
make targetenv_make_deb_docker      # Package Debian in container
make targetenv_make_apk_docker      # Package Android in container
```
Note some targets that don't use any non-standard linux commands (like `make targetenv`) and don't require a _docker version.


### Example Builds

Builds a PC linux runtime with the clang compiler for debian11 systems using GTK for UI elements, gstreamer for media processing and greengrass for IoT and Edge-compute.
#### Linux Desktop
```bash
./configure linux_x86_64_clang_gtk_gst_gg_debian11
make prepdeps
make all_docker
make targetenv
make targetenv_deb              # Create Debian package
```

#### ESP32 Microcontroller
```bash
./configure esp32s3_freertos-xtensa-community
make prepdeps
make all_docker
make targetenv
make targetenv_esp32_docker            # Create firmware image
```

#### Android
```bash
./configure linux_android_arm64
make prepdeps
make all_docker
make targetenv
make targetenv_apk_docker              # Create Android APK
```

#### Arduino
```bash
./configure arduino_arduino-mbed-nano_community
make prepdeps
make all_docker
make targetenv
make targetenv_arduino          # Create Arduino library
```

### Build Troubleshooting

#### Docker Container Management
Dockerhub is used to store pre-built containers and distribute them to build environments, where they may be cached on local machines.

Dockerhub images can be created from Dockerfiles in ert-components and published to docker with the following commands:
```bash
# Clean Docker state
make publish_docker_image
```
Some direct docker commands that may be is use:
```bash
# Clean Docker state
docker system prune -a
docker builder prune
```

---

## Usage Examples

### Basic Runtime Deployment

After building, your eRT runtime will be in the staging directory:
```bash
cd ../TARGET_TREES/ehs-env_[your-target]/
./bin/ehs.exe                    # Run eRT runtime
```

The minimal deployment includes:
```
bin/ehs.exe                      # eRT runtime executable
apps/default/t.sdl               # Default Lucid application (SODL format)
lib/                             # Runtime libraries (if needed)
```

### Running with Custom Applications

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

### Platform-Specific Examples

#### Raspberry Pi Deployment
```bash
# Build for Raspberry Pi
./configure linux_arm64_gtk_gst_gg_debian11
make prepdeps && make all_docker
make targetenv_deb

# Install on target device
scp ../TARGET_TREES/packages/*.deb pi@raspberrypi:~
ssh pi@raspberrypi 'sudo dpkg -i *.deb'
```

#### ESP32 Flash and Monitor
```bash
# Build ESP32 firmware
./configure esp32s3_freertos-xtensa-community
make prepdeps && make all_docker
make targetenv_esp32

# Flash to device (requires esptool)
esptool.py --port /dev/ttyUSB0 write_flash 0x0 firmware.bin

# Monitor serial output
screen /dev/ttyUSB0 115200
```

#### Android Installation
```bash
# Build Android APK
./configure linux_android_arm64
make prepdeps && make all_docker
make targetenv_apk_docker

# Install via ADB
make upload_ehs_via_adb                      # This will install the apk, plus the supervisor 
adb install ../TARGET_TREES/packages/*.apk   # Or you can go native and run this for just one package.
adb shell am start -n com.inxware.ert/.MainActivity #and run the apps remotely,
```

### Development Examples

#### Creating a Custom Component

1. **Define component interface (CDF file)**
   ```xml
   <!-- Common/Components/custom/my_sensor.cdf -->
   <component name="my_sensor" category="sensors">
     <outputs>
       <pin name="temperature" type="real"/>
       <pin name="humidity" type="real"/>
     </outputs>
     <events>
       <event name="data_ready"/>
     </events>
   </component>
   ```

2. **Implement component logic**
   ```c
   // Common/Components/custom/my_sensor.c
   #include "my_sensor.h"
   
   void my_sensor_init(my_sensor_t* component) {
       // Initialize sensor hardware
   }
   
   void my_sensor_update(my_sensor_t* component) {
       // Read sensor data
       component->temperature = read_temperature();
       component->humidity = read_humidity();
       // Trigger data ready event
       ehs_trigger_event(component, EVENT_DATA_READY);
   }
   ```

3. **Register and build**
   ```bash
   # Add to component makefile
   echo "my_sensor.c" >> Common/Components/custom/components.mk
   
   # Update development tools
   make toolsenv_update
   
   # Component now available in Lucid IDE
   ```

#### Hardware Abstraction Example

```c
// target/os-arch/my_platform/target_gpio.c
#include "hal_gpio.h"

ehs_result_t hal_gpio_write(int pin, int value) {
    // Platform-specific GPIO implementation
    platform_gpio_set(pin, value);
    return EHS_SUCCESS;
}

int hal_gpio_read(int pin) {
    return platform_gpio_get(pin);
}
```

### Integration Examples

#### Unity Game Engine Plugin
```bash
# Build Unity plugin
./configure linux_android_arm_unity-lib
make prepdeps && make all_docker

# Plugin files created in:
# ../TARGET_TREES/unity-plugins/
# Copy to Unity project Assets/Plugins/
```

#### Web Dashboard Integration
```javascript
// Connect to eRT via HTTP API
fetch('http://device-ip:8080/api/status')
  .then(response => response.json())
  .then(data => {
    console.log('Device status:', data);
    updateDashboard(data);
  });
```

### Performance Monitoring

#### Runtime Debugging
```bash
# Run with debug logging
./configure -debug
# Or set environment variable
EHS_LOG_LEVEL=DEBUG ./bin/ehs.exe
```

#### System Resource Monitoring
```bash
# Monitor CPU/memory usage
top -p $(pgrep ehs.exe)

# Monitor network activity
netstat -tulpn | grep ehs
```

---

## Development

### Component Development

#### Component Architecture
Each eRT component consists of:
- **CDF file** (.cdf): XML component description
- **Implementation** (.c/.h): C/C++ source code  
- **Bitmap** (.bmp): Visual icon for IDE
- **Interface file** (.idf.ini): IDE integration settings

#### Creating New Components

1. **Choose component category**
   ```bash
   ls Common/Components/
   # core/ gui/ networking/ media/ ml/ mv/
   ```

2. **Create component files**
   ```bash
   cd Common/Components/core/
   
   # Create component description
   nano my_component.cdf
   
   # Create implementation
   nano my_component.c
   nano my_component.h
   
   # Create icon (24x24 bitmap)
   # my_component.bmp
   
   # Create IDE settings
   nano my_component.idf.ini
   ```

3. **Register component**
   ```bash
   # Add to category makefile
   echo "my_component.c" >> components.mk
   
   # Update dependencies
   make depend
   
   # Update IDE tools
   make toolsenv_update
   ```

#### Component Categories

| Category | Purpose | Examples |
|----------|---------|----------|
| **core** | Basic operations | Operators, buffers, timers, file I/O |
| **gui** | User interface | Displays, buttons, text boxes, graphics |
| **networking** | Communication | HTTP, MQTT, TCP/UDP, WebSocket |
| **media** | Audio/video | Codecs, players, streaming |
| **ml** | Machine learning | TensorFlow Lite, inference engines |
| **mv** | Machine vision | Image processing, computer vision |

### Platform Porting

#### Porting New Hardware

1. **Create platform configuration**
   ```bash
   mkdir target/platform/my_new_platform/
   cd target/platform/my_new_platform/
   
   # Copy from similar platform
   cp ../linux_x86_64_clang/config.mk .
   cp ../linux_x86_64_clang/target_config.h .
   
   # Edit for your platform
   nano config.mk
   nano target_config.h
   ```

2. **Implement HAL layer**
   ```bash
   mkdir target/os-arch/my_os-my_arch/
   cd target/os-arch/my_os-my_arch/
   
   # Required files:
   nano target.mk           # Build configuration
   nano toolchain.mk        # Compiler settings
   nano target_main.c       # Application entry point
   nano target_time.c       # Timer implementation
   nano target_process.c    # Threading/mutex
   nano target_file.c       # File system
   # ... other HAL implementations
   ```

3. **Test and validate**
   ```bash
   ./configure my_new_platform
   make prepdeps
   make all
   make targetenv_run_tests
   ```

For detailed porting instructions, see [Porting Guide](docs/ert-porting-guide.md).

### Testing Guidelines

#### Unit Testing
```bash
# Run all unit tests
make targetenv_run_tests

# Run specific test suites
cd UnitTest/
./run_function_library_tests.sh
./run_component_tests.sh
```

#### Integration Testing
```bash
# CI regression tests
./SystemTests/CI/regression_test-published-only.sh

# Platform-specific tests
./SystemTests/CI/projects/[platform]/test_*.sh
```

#### Manual Testing
```bash
# Test basic functionality
./configure -run
# Should start without errors
# Check basic I/O, networking, etc.

# Test debugging
./configure -debug
# Should provide detailed logging
```

### Debugging Tools
eRT builds can be debugged most easily on linux targets using GDB, but can also be debugged on MCUs that support debugger connections (e.g. JTAG)
#### GDB on Linux targets
You will need to install gdb on the linux target first.
```bash
# install gdb (Debian/Raspbien/Ubuntu)
apt install gdb
# Debug with GDB
./configure -debug
# Automatically starts GDB session

# For embedded targets
For embedded target supporting debuggers the following host scripts are available.

./scripts/build-deploy/[platform]/debug_*.sh
```

#### Logging System
The logging verbosity of eRT can be configured in the build at the module level for verbosity 
The following logging functions are used within the source code:
```c
// In component code
#include "hal_logger.h"

hal_log_info("Component initialized");
hal_log_error("Failed to read sensor: %d", error_code);
hal_log_debug("Processing data: %f", sensor_value);
```


**todo** The following runtime log level selection may not actually work currently!
```bash
# Control log levels
EHS_LOG_LEVEL=DEBUG
EHS_LOG_CATEGORIES="network,gpio"
./bin/ehs.exe
```

#### Static Analysis
```bash
# Run static analysis
make static_analysis

# Results in build/analysis/
# Includes: cppcheck, clang-analyzer, etc.
```

### Code Style Guidelines

#### Naming Conventions
```c
// Functions: lowercase with underscores
int component_initialize(component_t* comp);

// Types: lowercase with _t suffix
typedef struct {
    int value;
    char name[32];
} my_component_t;

// Constants: uppercase with underscores
#define MAX_BUFFER_SIZE 1024
#define DEFAULT_TIMEOUT_MS 5000

// HAL functions: hal_ prefix
ehs_result_t hal_gpio_write(int pin, int value);
```

#### Component Source File Organization
```
Common/Components/<category>/
├── <component_name>.c           # Implementation
├── <component_name>.h           # Public object interface
├── <component_name>.cdf         # Component XML descriptor used in Lucid IDE
├── <component_name>/tests/      # Lucid Applications used for unit testing the component
└── <component_name>/help/       # HTML documentation (used in Lucid)
```

---

## Documentation

### 📚 Core Documentation
- **[Porting Guide](docs/ert-porting-guide.md)**: Comprehensive guide for adding new platforms and creating components
- **[Build System Reference](docs/build-system.md)**: Detailed build system documentation
- **[Component API Reference](docs/component-api.md)**: Component development guide
- **[HAL Interface Specification](docs/hal-interface.md)**: Hardware abstraction layer documentation

### 🎯 Quick References
- **[Supported Platforms](docs/platforms.md)**: Complete platform compatibility matrix
- **[Configuration Options](docs/configuration.md)**: Build and runtime configuration reference
- **[Troubleshooting Guide](docs/troubleshooting.md)**: Common issues and solutions
- **[Performance Tuning](docs/performance.md)**: Optimization guidelines

### 🎥 Video Tutorials
- [Getting Started with eRT](https://video.inxware.io/getting-started) (15 min)
- [Creating Your First Component](https://video.inxware.io/components) (20 min)
- [Porting to New Hardware](https://video.inxware.io/porting) (30 min)
- [Advanced Debugging Techniques](https://video.inxware.io/debugging) (25 min)

### 🔗 External Resources
- **[inxware Developer Portal](https://dev.inxware.io/)**: Complete development ecosystem
- **[Lucid IDE Documentation](https://docs.inxware.io/lucid/)**: Visual programming environment
- **[Component Marketplace](https://marketplace.inxware.io/)**: Community components
- **[Architecture Overview](https://docs.google.com/document/d/1pD4cbTBuBk3TttwCfbbIlChIIP5dVxm01D9qGm6vL4w/)**: System design documentation

### 📖 Academic Papers
- [Event-Driven Architectures for Embedded Systems](https://research.inxware.io/event-driven)
- [Cross-Platform HAL Design Patterns](https://research.inxware.io/hal-patterns)
- [No-Code Development for IoT](https://research.inxware.io/nocode-iot)

---

## Contributing

We welcome contributions from the community! Whether you're fixing bugs, adding features, porting to new platforms, or improving documentation, your help makes eRT better for everyone.

### 🚀 Quick Contributing Guide

1. **Fork the repository**
2. **Create a feature branch**: `git checkout -b feature/amazing-feature`
3. **Make your changes** and test thoroughly
4. **Commit**: `git commit -m 'Add amazing feature'`
5. **Push**: `git push origin feature/amazing-feature`
6. **Open a Pull Request**

### 📋 Contribution Areas

#### 🐛 Bug Reports
Found a bug? Help us fix it:
- Use the [bug report template](.github/ISSUE_TEMPLATE/bug_report.md)
- Include system information, build logs, and reproduction steps
- Test with the latest version before reporting

#### ✨ Feature Requests  
Have an idea for improvement?
- Use the [feature request template](.github/ISSUE_TEMPLATE/feature_request.md)
- Describe the use case and expected behavior
- Consider contributing the implementation yourself!

#### 🔧 Platform Ports
Adding support for new hardware:
- Follow the [porting guide](docs/ert-porting-guide.md)
- Ensure all tests pass on your platform
- Document hardware requirements and limitations
- Include example applications

#### 📦 Component Development
Creating new components:
- Follow the [component development guide](#component-development)
- Include comprehensive tests
- Provide clear documentation and examples
- Design for reusability across platforms

#### 📚 Documentation
Improving documentation:
- Fix typos, clarify instructions
- Add examples and tutorials
- Translate to other languages
- Create video content

### 🔍 Code Review Process

All contributions go through code review:

1. **Automated Checks**: CI runs tests on all supported platforms
2. **Maintainer Review**: Core team reviews code quality and design
3. **Community Review**: Other contributors may provide feedback
4. **Testing**: Verify functionality on target platforms
5. **Merge**: Approved changes are merged to main branch

### 📏 Development Standards

#### Code Quality
- **Follow existing style**: Use consistent naming and formatting
- **Write tests**: Include unit tests for new functionality
- **Document APIs**: All public functions need documentation
- **Handle errors**: Proper error checking and reporting

#### Platform Compatibility
- **Test on multiple platforms**: Verify cross-platform compatibility
- **Use HAL abstractions**: Don't break platform abstraction
- **Consider resource constraints**: MCUs have limited memory/storage
- **Maintain backwards compatibility**: Don't break existing APIs

#### Security
- **No secrets in code**: Use configuration for credentials
- **Validate inputs**: Check all external inputs
- **Follow secure coding practices**: Prevent buffer overflows, etc.
- **Review dependencies**: Ensure third-party code is secure

### 🏷️ Commit Guidelines

#### Commit Message Format
```
type(scope): description

[optional body]

[optional footer]
```

#### Types
- `feat`: New feature
- `fix`: Bug fix
- `docs`: Documentation changes
- `style`: Code style changes (formatting, etc.)
- `refactor`: Code refactoring
- `test`: Adding or updating tests
- `build`: Build system changes

#### Examples
```bash
feat(components): add temperature sensor component

Implements I2C temperature sensor component with calibration
support. Includes unit tests and documentation.

Closes #123

fix(hal): resolve GPIO race condition on ESP32

The GPIO write function had a race condition that could cause
incorrect pin states during rapid switching.

test(build): add regression test for android build

Ensures Android APK builds correctly with all required
dependencies and proper signing.
```

### 🎯 Getting Help

#### Before Contributing
- Read this README and the [porting guide](docs/ert-porting-guide.md)
- Check existing [issues](https://github.com/inxware/ert-components/issues) and [discussions](https://github.com/inxware/ert-components/discussions)
- Join our [community channels](#support--community)

#### During Development
- Ask questions in [GitHub Discussions](https://github.com/inxware/ert-components/discussions)
- Join our [Discord server](https://discord.gg/inxware) for real-time help
- Attend our monthly [contributor meetups](https://meetup.inxware.io/)

### 🏆 Recognition

Contributors are recognized through:
- **Hall of Fame**: Featured on our website and documentation
- **Contributor Badge**: Special recognition in the community
- **Early Access**: Beta access to new features and tools
- **Mentorship**: Direct support from core team members
- **Conference Opportunities**: Speaking opportunities at events

### 📄 Legal

By contributing, you agree that:
- Your contributions will be licensed under LGPLv3
- You have the right to contribute the code
- Your contributions may be used commercially under the inxware license

---

## Support & Community

### 💬 Community Channels

#### Primary Support
- **[GitHub Discussions](https://github.com/inxware/ert-components/discussions)**: Questions, ideas, and general discussion
- **[Discord Server](https://discord.gg/inxware)**: Real-time chat and support
- **[Stack Overflow](https://stackoverflow.com/questions/tagged/inxware-ert)**: Technical Q&A with the `inxware-ert` tag

#### Social & Updates
- **[Twitter @inxware](https://twitter.com/inxware)**: News and updates
- **[LinkedIn Company Page](https://linkedin.com/company/inxware)**: Professional updates
- **[YouTube Channel](https://youtube.com/inxware)**: Tutorials and demos

### 📅 Regular Events

#### Weekly
- **Community Call**: Wednesdays 3PM UTC - General discussion and Q&A
- **Office Hours**: Fridays 2PM UTC - Direct support from core team

#### Monthly  
- **Contributor Meetup**: First Thursday of each month
- **Platform Spotlight**: Third Tuesday - Focus on specific platforms
- **Component Showcase**: Last Friday - Community component demos

#### Annual
- **inxware DevCon**: Annual developer conference
- **Hackathon**: 48-hour community coding event
- **Awards Ceremony**: Recognizing top contributors

### 🆘 Getting Help

#### Self-Service Resources
1. **[FAQ](docs/faq.md)**: Common questions and answers
2. **[Troubleshooting Guide](docs/troubleshooting.md)**: Step-by-step problem solving
3. **[Video Tutorials](#documentation)**: Visual learning resources
4. **[Knowledge Base](https://kb.inxware.io/)**: Searchable documentation

#### Community Support
1. **GitHub Discussions**: Best for detailed technical questions
2. **Discord**: Great for quick questions and real-time help
3. **Stack Overflow**: Ideal for programming-specific questions

#### Professional Support
For commercial users and professional support:
- **[Enterprise Support](https://inxware.io/enterprise)**: Priority support with SLA
- **[Consulting Services](https://inxware.io/consulting)**: Custom development and integration
- **[Training Programs](https://inxware.io/training)**: On-site and remote training

### 🐛 Issue Reporting

#### Bug Reports
When reporting bugs, please include:
- **System information**: OS, architecture, build environment
- **eRT version**: `git describe --tags`
- **Reproduction steps**: Clear, step-by-step instructions
- **Expected vs actual behavior**: What should happen vs what happens
- **Logs and output**: Build logs, runtime logs, error messages
- **Screenshots/videos**: If applicable

Use our [bug report template](.github/ISSUE_TEMPLATE/bug_report.md) for best results.

#### Feature Requests
For feature requests, include:
- **Use case**: Why is this feature needed?
- **Proposed solution**: How should it work?
- **Alternatives considered**: Other approaches you've tried
- **Additional context**: Screenshots, mockups, examples

### 📊 Project Status

#### Release Cycle
- **Major releases**: Every 6 months (January, July)
- **Minor releases**: Monthly on the 15th
- **Patch releases**: As needed for critical fixes
- **LTS releases**: Every 2 years with 3-year support

#### Current Roadmap
- **Q4 2024**: RISC-V platform support, WebAssembly runtime
- **Q1 2025**: Enhanced ML components, edge AI optimization
- **Q2 2025**: Cloud-native deployment, Kubernetes support
- **Q3 2025**: Real-time safety certification (ISO 26262)

#### Platform Status
| Platform | Status | Maintainer | Last Updated |
|----------|--------|------------|--------------|
| Linux x86_64 | ✅ Stable | Core Team | Current |
| ESP32/ESP32-S3 | ✅ Stable | @esp32-maintainer | Current |
| Android ARM64 | ✅ Stable | @android-team | Current |
| Raspberry Pi | ✅ Stable | @rpi-community | Current |
| Arduino | ✅ Stable | @arduino-maintainer | Current |
| Windows x86 | ⚠️ Beta | @windows-team | -1 release |
| NXP Kinetis | 🔄 Active | @nxp-maintainer | Current |
| RISC-V | 🚧 In Progress | @riscv-team | Development |

### 🤝 Partnership Opportunities

#### Academic Partnerships
- **Research Collaboration**: Joint research projects
- **Student Programs**: Internships and thesis projects
- **Educational Licensing**: Free licenses for educational use
- **Curriculum Development**: Course materials and labs

#### Commercial Partnerships
- **System Integrators**: Partnership program for solution providers
- **Hardware Vendors**: Porting and optimization services
- **Tool Vendors**: Integration with development tools
- **Cloud Providers**: Hosted deployment solutions

#### Open Source Partnerships
- **Foundation Membership**: Linux Foundation, Eclipse Foundation
- **Cross-Project Collaboration**: Integration with other OSS projects
- **Standardization**: Contributing to industry standards
- **Event Sponsorship**: Supporting community events

---

## License

### 🔐 License Overview

eRT Components is dual-licensed to support both open-source development and commercial deployment:

#### Open Source License (LGPLv3)
The **eRT Components library** is licensed under the [GNU Lesser General Public License v3.0](LICENSE.md). This means:

✅ **You can:**
- Use eRT Components in both open-source and commercial applications
- Modify the eRT Components source code
- Distribute applications built with eRT Components
- Sell products that include eRT Components

📋 **You must:**
- Provide source code for any modifications to eRT Components itself
- Include the LGPLv3 license text in distributions
- Allow users to replace the eRT Components library with their own version

🚫 **You don't need to:**
- Open-source your entire application
- Share the source code of applications that use eRT Components
- Pay royalties for using eRT Components

#### Commercial Runtime License
The **EHS Kernel** (Event Handling System) includes a proprietary runtime licensed for:
- **Free use**: Up to 10 device deployments per user/organization
- **Commercial licensing**: Available for larger deployments
- **Enterprise support**: Professional support and consulting services

### 📜 Third-Party Licenses

eRT Components includes and links to various third-party libraries:

| Component | License | Usage |
|-----------|---------|-------|
| **FreeRTOS** | MIT | Real-time operating system |
| **lwIP** | BSD | Lightweight TCP/IP stack |
| **cJSON** | MIT | JSON parsing library |
| **Unity** | MIT | Unit testing framework |
| **Docker base images** | Various | Build environment containers |

See [THIRD_PARTY_LICENSES.md](THIRD_PARTY_LICENSES.md) for complete details.

### 🏢 Commercial Use

#### Free Commercial Use
Under the LGPLv3 license, you can freely:
- Build and sell products using eRT Components
- Deploy up to 10 devices with the EHS Kernel runtime
- Use the free [Lucid Community IDE](https://appland.inxware.io/)
- Access community support and documentation

#### Extended Commercial Licensing
For larger deployments or additional services:
- **[Enterprise License](https://inxware.io/enterprise)**: Unlimited device deployments
- **[Professional Support](https://inxware.io/support)**: Priority support with SLA
- **[Custom Development](https://inxware.io/consulting)**: Porting and integration services
- **[OEM Licensing](https://inxware.io/oem)**: White-label and embedded licensing

### 🤝 Developer Partner Program

Join the **[inxware Developer Partner Program](https://inxware.io/developer-partners/)** for:
- Extended free licensing (up to 100 devices)
- Early access to new features and platforms
- Technical support and training
- Co-marketing opportunities
- Revenue sharing for marketplace components

### ⚖️ Patent Policy

inxware has a policy of not asserting patents against open-source implementations of eRT Components. We encourage innovation and collaborative development while protecting our commercial interests.

### 📞 Licensing Questions

For licensing questions or commercial inquiries:
- **Email**: [licensing@inxware.io](mailto:licensing@inxware.io)
- **Sales**: [sales@inxware.io](mailto:sales@inxware.io)
- **Legal**: [legal@inxware.io](mailto:legal@inxware.io)

---

### 📊 Project Statistics

![GitHub stars](https://img.shields.io/github/stars/inxware/ert-components?style=social)
![GitHub forks](https://img.shields.io/github/forks/inxware/ert-components?style=social)
![GitHub contributors](https://img.shields.io/github/contributors/inxware/ert-components)
![GitHub last commit](https://img.shields.io/github/last-commit/inxware/ert-components)
![Lines of code](https://img.shields.io/tokei/lines/github/inxware/ert-components)

---

**Ready to get started?** [Try the Quick Start](#quick-start) or explore the [Lucid IDE](https://appland.inxware.io/) to create your first no-code embedded application!

---
#LICENSING

*© 2024 inx limited. The COmmunity eRT Components release is open-source software licensed under LGPLv3. The EHS Kernel is proprietary software with free and commercial licensing options.*
