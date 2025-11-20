# About inxware

* [Why use inxware?](#why-use-inxware)
* [What is inxware?](#what-is-inxware)
* [How is inxware licenced?](#how-is-inxware-licenced)
* [Where do I get inxware?](#where-do-i-get-inxware)
* [eRT build quick start guide](#ert-build-quick-start-guide)
* [Supported platforms](#supported-platforms)
* [Further reading](#further-reading)
* [Project statistics](#project-statistics)
* [GitHub build status](#github-build-status)

## Why use inxware?

inxware is a powerful solution helping you to rapidly develop real-time and data-intensive firmware for embedded devices. It is in active use across [many sectors](https://www.inx-systems.com/sectors/), including industrial IoT, smart buildings, smart domestic energy products, edge-AI applications and advanced control systems.

It supports [devices](#supported-platforms) ranging from basic microcontrollers (e.g. NXP Kinetis, STM32, ESP32, etc.) through Arduino, Raspberry Pi, and Android tablets, all the way to full Windows/Linux PCs and containerised environments, allowing you to build products of any complexity.

The inxware development suite includes a powerful no-code IDE called [Lucid](https://appland.inxware.io//), to help you to assemble the exact components you require for your application - potentially without ever needing to write any code. Our related [Appland](https://appland.inxware.io/appLand/index.php/) community gallery helps inxware users to share their applications. It is not essential to use Lucid with inxware, but it can provide significant development acceleration for many typical cases.

## What is inxware?

inxware encompasses an ecosystem of software, including:

* A graphical, no-code IDE called [Lucid](https://appland.inxware.io/) for creating your application(s) by connecting components together and adding control logic.
* A event-driven runtime application called "eRT" (which includes a proprietary runtime kernel, called EHS Kernel) and Hardware Abstraction Layer (HAL).
* A broad range of 'components' providing all of the functionality you would expect in an embedded system.
* Your applications, these run within the eRT environment.
* An IoT administration platform called [Devman](https://www.inxware.io/devman/), which can be used to monitor and manage devices running your deployed inxware apps.

![inxware diagram](docs/inxware.png)

eRT can be run bare-metal on microcontrollers or as an application in more full-featured RTOS- and OS-based systems. It is written in C and can be ported to virtually any platform that has a suitable toolchain (e.g. C/C++ compiler). It has already been ported to [many popular platforms](#supported-platforms).

Here are some of the key features of inxware:

**Cross-Platform Runtime**

- **10+ architectures**: ARM, x86, RISC-V, Xtensa.
- **Multiple OS support**: Linux, Android, Windows, FreeRTOS, Arduino.
- **Unified build system**: includes Docker containerisation.

**Rich Component Library**

- **Core components**: Operators, buffers, timers, file I/O.
- **Networking**: Wi-Fi, HTTP, MQTT, TCP/UDP sockets.
- **Graphics & UI**: Display drivers, controls, imaging.
- **Hardware interfaces**: GPIO, ADC/DAC, PWM, UART.

**Production Ready**

- **OTA updates**: Over-the-air firmware deployment.
- **Package formats**: APK, DEB, firmware images, Unity plugins.
- **CI/CD integration**: Automated testing and deployment.

**No-Code Development**

- **Visual programming**: Drag-and-drop component assembly / app development.
- **Real-time debugging**: Live system monitoring and management.
- **Rapid prototyping**: From initial concept to working prototype in minutes.

---

## How is inxware licenced?

The majority of the inxware runtime stack is published under a permissive Open Source licence ([LGPL v3.0](licenses/eRT_Components.md)) and we welcome [contributions](CONTRIBUTING.md) from the community to expand and improve its capabilities and features.

There is a single closed-source component, the inxware event-handling system EHS Kernel, supplied in binary form in our [`ert-build-support`](https://github.com/inxware/ert-build-support) repository. The EHS Kernel is required and automatically included in inxware firmware builds.

Please refer to our [LICENSE.md](LICENSE.md) document for full information on licensing.

## Where do I get inxware from?

There are two routes to trying out inxware:

1. Top-down: build your applications in the Lucid no-code environment and run them on your chosen hardware platform(s)
   
   - Register with the inx [Appland](https://appland.inxware.io/appLand/) community and try out Lucid for free.
2. Bottom-up: integrate your own software components with inxware and build custom firmware images.
   
   - See the "build quick start guide" below.

The second option is only normally required if you need to develop your own eRT components, or create an eRT port to a new target hardware platform which isn't already supported by the published sources.

## eRT build quick start guide

To create an inxware firmware image for your device, you will just need these three items:

1. A Linux build machine (including [Windows Subsystem for Linux](https://learn.microsoft.com/en-us/windows/wsl/))
2. The ERT (Event RunTime) environment
3. Your Lucid application, which would typically be located in the [inxware Apps](https://github.com/inxware/apps) repository.

**Note:** This guide assumes you have access to a bash shell in a Debian/Ubuntu Linux distribution. If you have a Windows PC, you can install and run Linux *within Windows* using [Windows Subsystem for Linux](https://learn.microsoft.com/en-us/windows/wsl/).

### Build system requirements

| Requirement | Minimum                                       | Recommended                        |
| ----------- | --------------------------------------------- | ---------------------------------- |
| **OS**      | Linux (Ubuntu 18.04+) or Windows 10 with WSL2 | Ubuntu 20.04+ or Debian 11+        |
| **RAM**     | 8GB                                           | 16GB+                              |
| **Storage** | 50GB free                                     | 100GB+ SSD                         |
| **CPU**     | 4 cores                                       | 8+ cores                           |
| **Network** | Broadband internet                            | High-speed for container downloads |

Within a Linux bash shell, enter the following commands to check your build environment is working:

```bash
mkdir inxware && cd inxware

# Clone the main `ert-components` repository
#
# If you prefer HTTPS:
# git clone https://github.com/inxware/ert-components.git
#
# If you prefer SSH:
git clone git@github.com:inxware/ert-components.git

cd ert-components

# Configure the build for your chosen target platform
./configure  # List all available targets
./configure linux_x86_64_clang  # Configure for Linux x64 - test on build machine

# Install the build dependencies
make prepdeps       # Downloads toolchains and dependencies (~40GB) 

# Build the runtime binary
make all_docker     # Build using containerised environment

# Create a staging directory for later packaging
make targetenv      # Assemble the runtime environment
```

**Success!** You now have a working eRT runtime. Try the [Lucid IDE](https://appland.inxware.io/) to create your first no-code application.

### Repository dependencies

- **ert-build-support** (~20GB): Binary toolchains and build tools
- **ert-contrib-middleware** (~15GB): Pre-built 3rd-party libraries
- **apps** (optional): Demo applications and examples

### Docker configuration

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

### Troubleshooting

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

## Supported platforms

| Platform Category    | Architecture | Operating System                  | Status   | Package Format | Use Cases                  |
| -------------------- | ------------ | --------------------------------- | -------- | -------------- | -------------------------- |
| **Desktop/server**   | x86_64       | Linux (Debian 9-12, Ubuntu 14-24) | ✅ Stable | DEB, Binary    | Development, Server apps   |
|                      | x86_64       | Windows 7-11                      | ✅ Stable | EXE, MSI       | Desktop applications       |
| **Single board**     | ARM64        | Linux (Raspberry Pi 3-5)          | ✅ Stable | DEB, Image     | IoT gateways, edge compute |
|                      | ARM64        | Android (Rock Pi, Radxa)          | ✅ Stable | APK            | Media players, kiosks      |
|                      | ARM32        | Linux (Various SBCs)              | ✅ Stable | DEB, Image     | Industrial controllers     |
| **Microcontrollers** | Xtensa       | ESP32/ESP32-S3 (FreeRTOS)         | ✅ Stable | Firmware       | IoT sensors, edge devices  |
|                      | ARM Cortex-M | NXP Kinetis (FreeRTOS)            | ✅ Stable | Firmware       | Industrial automation      |
|                      | ARM Cortex-M | Arduino (MBED, Native)            | ✅ Stable | Firmware       | Prototyping, education     |
| **Mobile/gaming**    | ARM          | Unity Plugin                      | ✅ Stable | Unity Package  | Games, interactive apps    |
|                      | ARM64        | Android NDK                       | ✅ Stable | AAR, SO        | Mobile applications        |

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

### **Industrial & IoT**

- **Raspberry Pi**: All models (3, 4, 5, Zero)
- **Rock Pi**: 4A/4B/4C+, Rock 3C, Rock 5A/5B
- **ESP32 family**: ESP32, ESP32-S3, ESP32-C3
- **NXP Kinetis**: K64F, K66F, RT series
- **STM32**: F4, F7, H7 series (via Arduino)

### **Consumer devices**

- Android tablets and phones
- Set-top boxes and media players
- Digital signage displays
- Interactive kiosks

## Further reading

The following documents contain more information about inxware, eRT Components, and related topics:

- Guidelines on getting involved with [contributing](CONTRIBUTING.md) to this project.
- Our guide to [building](BUILDING.md) eRT Components.
- Our guide to [developing](DEVELOPING.md) within inxware.
- The eRT [licence](LICENSE.md).

## Github build status

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://github.com/inxware/ert-components/actions)

[![build-linux_arm64_lvgl_gg_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_lvgl_gg_debian11.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_lvgl_gg_debian11.yml)

[![build-linux_arm64_gtk_gst_gg_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_gtk_gst_gg_debian11.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_gtk_gst_gg_debian11.yml)

[![build-linux_android_arm64](https://github.com/inxware/ert-components/actions/workflows/build-linux_android_arm64.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_android_arm64.yml)

[![build-esp32s3_freertos-xtensa-community](https://github.com/inxware/ert-components/actions/workflows/build-esp32s3_freertos-xtensa-community.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-esp32s3_freertos-xtensa-community.yml)

[![build-linux_x86_64_clang_lvgl_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64-linux_x86_64_clang_lvgl_debian11-no-certs.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64-linux_x86_64_clang_lvgl_debian11-no-certs.yml)

[![build-linux_x86_64_clang_gtk_gst_gg_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64_clang_gtk_gst_gg_debian11-no-certs.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64_clang_gtk_gst_gg_debian11-no-certs.yml)

[![build-win_x86_32-lucid-win10](https://github.com/inxware/ert-components/actions/workflows/build-win_x86_32-lucid-win10.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-win_x86_32-lucid-win10.yml)

[![build-arduino-mbed-nano-community](https://github.com/inxware/ert-components/actions/workflows/build-arduino-mbed-nano_community.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-arduino-mbed-nano_community.yml)

## Project statistics

![GitHub stars](https://img.shields.io/github/stars/inxware/ert-components?style=social)

![GitHub forks](https://img.shields.io/github/forks/inxware/ert-components?style=social)

![GitHub contributors](https://img.shields.io/github/contributors/inxware/ert-components)

![GitHub last commit](https://img.shields.io/github/last-commit/inxware/ert-components)

![Code size](https://img.shields.io/github/languages/code-size/inxware/ert-components)

[![License](https://img.shields.io/badge/license-LGPLv3-blue)](LICENSE.md)

---

**Ready to get started?** [Try the Quick Start](#ert-build-quick-start-guide), find out more on [YouTube](https://www.youtube.com/@inxiot), or explore the [Lucid IDE](https://appland.inxware.io/) to create your first no-code embedded application!

## Attribution

*Copyright © 2008–2025 inx Limited. The community **eRT Components** release is open‑source under **LGPLv3**. The **EHS Kernel** is proprietary and licensed separately as described in [LICENSE.md](LICENSE.md).*
