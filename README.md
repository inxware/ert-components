Device Types Supported
-----------------------
![Static Badge](https://img.shields.io/badge/RaspberryPi-Y-green) ![Static Badge](https://img.shields.io/badge/Arduino-Y-green) ![Static Badge](https://img.shields.io/badge/esp32-Y-green) ![Static Badge](https://img.shields.io/badge/esp32s3-Y-green) ![Static Badge](https://img.shields.io/badge/Windows-Y-green) ![Static Badge](https://img.shields.io/badge/Android-Y-green) ![Static Badge](https://img.shields.io/badge/x86_linux-Y-green) ![Static Badge](https://img.shields.io/badge/arm-freeRTOS-Y-green) ![Static Badge](https://img.shields.io/badge/arm_linux-Y-green) ![Static Badge](https://img.shields.io/badge/RISCV-Y-green)

Github Build Status
------------------- 

[![build-linux_arm64_lvgl_gg_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_lvgl_gg_debian11-no-certs.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_lvgl_gg_debian11.yml)

[![build-linux_arm64_gtk_gst_gg_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_gtk_gst_gg_debian11.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_gtk_gst_gg_debian11.yml)

[![build-linux_android_arm64](https://github.com/inxware/ert-components/actions/workflows/build-linux_android_arm64.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_android_arm64.yml)

[![build-esp32s3_freertos-xtensa-community](https://github.com/inxware/ert-components/actions/workflows/build-esp32s3_freertos-xtensa-community.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-esp32s3_freertos-xtensa-community.yml)

[![build-linux_x86_64_clang_lvgl_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64-linux_x86_64_clang_lvgl_debian11-no-certs.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64-linux_x86_64_clang_lvgl_debian11-no-certs.yml)

[![build-linux_x86_64_clang_gtk_gst_gg_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64_clang_gtk_gst_gg_debian11-no-certs.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64_clang_gtk_gst_gg_debian11-no-certs.yml)

[![build-win_x86_32-lucid-win10](https://github.com/inxware/ert-components/actions/workflows/build-win_x86_32-lucid-win10.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-win_x86_32-lucid-win10.yml)

[![build-arduino-mbed-nano-community](https://github.com/inxware/ert-components/actions/workflows/build-arduino-mbed-nano_community.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-arduino-mbed-nano_community.yml)

What is inxware?
============
inxware streamlines embedded software development by separating software integration and application development. 

It comprises the following key parts:
- ert-components: An open source library of pre-built software and IO-drive components.
- [inxware Lucid](https://appland.inxware.io/)): a free graphical IDE for developing real time data-intensive device applicartions.

ert-components is open source C/C++ with a powerful cross-compiling build system, allowing any hardware porting and new components to be user-contributed.   

The lucid graphical IDE allows applications to be developed in a system engineering context and is available for free from [appland.inxware.io](https://appland.inxware.io/)

Device Support
==============
inxware can run on any CPU architecture and use any operating system. This repository includes some example builds for a represeantive set of targets and 100s more are available. New hardware support and software features can be user contributed with guidance on best practicces available from the [porting guide](https://github.com/inxware/ert-components/wiki/inxware-Porting-Guide).

inxware builds can be carried out locally (on linux or Windows+WSL) and Docker containers to ensure repeatable builds with specific SDKs and 3rd-party middleware dependencies. The inxware ert-components build system is simple and ensures all build environments remain are consistent and trusted.

This repository can generate modifiable builds for the following SoCs, SoMs, Evaluation boards, servers and desktop systems:
* ARM7
  - NXP Kenitis (FreeRTOS, MBED)
  - STM32 (FreeRTOS, Arduino)
  - RP2040 (Arduino)
* ARM64
  - RaspberryPi3-5 (Linux, Android)
  - Radxa-Rock2-5 (Linux, Android)
* xtensor (esp32)
  - ESP32, ESP32S3, ESP32C2 (IDF, FreeRTOS)
* x86/x86_64
  - Linux (Debian 7-12, Ubuntu 14-24, Linux-Lite, Yocto, OpenEmbedded)
  - Windows (7-11: VS, MINGW)

Device Certificate provisioning
-------------------------------
Provisioning devices with IoT service certificates requires an additional certificate repository (DevmanSecurity.git), which is not currently provided to community users. A safer version of this repository with public certificates will be released to allow non-client authenticated connections and the capability for contributors to add their own private keys when doing local builds. 


Build Environment Overview
==========================
The ```ert-components``` repository is the core repository needed to be the inxware eRT runtime. 
This build system currently supports the following hardware architectures and operating systems: 

The build system support fine-grained target configuration for particular hardware and user configurations, 
including package management and direct installation. To see the full list type `./configure` in the root of this repository.


# Build Steps
- The eRT build system ues docker to create some specific build environment, but needs a few linux packages installed first to get started.
- The simplest way to check your system is to use the make prepdeps command. e.g. 
```
./configure linux_x86_64_clang
make prepdeps.
```

This will install git build-essential (GNU make) and docker and two other large inxware repositories containing toolchains and prebuilts:
[ert-build-support](https://github.com/inxware/ert-build-support) - contains binary tolchains and uses git lfs (Large File Support).
[ert-contrib-middleware](https://github.com/inxware/ert-contrib-middleware) - contains pre-built 3rd party dependencies and builts and scripts for re-creating or updating. This contains original source and library build scripts for maintenance. 
[inxware/apps](https://github.com/inxware/apps) - contains demo and home apps, which may be pre-integrated defaults for product/serve/application specific.

Typically these repos are cloned into adjascent directories to ert-components with --depth=1 to reduce disk requirements.

The ert-components repository build system creates deployable firmware or applications in the relevant package format. The build requires the inxwae-EHS (Event Handling System) library to be linked to the application to accept Lucid no-code applications. 

Terms of Use
============
This resilting executable ert-component binaries can be copied and deployed under the terms of the [inxware license policy](https://www.inxware.io/pricing/), which are broadly:

- Deployable to 10 devices (commercial & non-cpommercial)
- Free perpetual use of the [Lucid Community IDE](https://appland.inxware.io/)
- Free perpetual access to inxware Community Firmware source code.
- Free perpetual access to inxware graphical component builder (eclipse).
- No restrictions for connecting to any IoT Gateway or 3rd-party cloud services.

Upgrade access can be provided for free to inxware application or components developers by joining the inxware [developer partner](https://www.inxware.io/developer-partners/) scheme.


Licensing
---------

eRT components are licenced under LGPLv3 license (See LICENSE.md) unless otherwise stated within sub modules from 3rd-parties. 

Full firmware builds contain the inxware kernel library which is a 100% portable closed source element of inxware and is licensed for commercial and non-commercial use for up to 10 copies per user or organisation.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE

Source & Dependencies
---------------------

An open source publication of this eRT runtime is hosted at https://github.com/inxware/ert-components

For further information about inxware IoT sftware development please seapproach read more here: https://www.inx-systems.com/dev-tools/ 

This build system for EHS requires linux (debian usually) and the following repos to be cecked out in to the same directory:

  ert-components         (Contains this readme file)
  ert-build-support      (Contains toolchains and Basic OS dependencies)
  ert-contrib-middleware (Contains optional pre-built components for each suport platform)

It is advisable to also check out the following repos to build production installs:
apps               (Contains applications which maybe installed into production packages)

All the above repos are available at ssh://inx-data@server/home/inx-data/data/Repos/*.git

Deployment
----------
Built eRT objects may be of the following format:
1. Linux user-space application
1. Windows application 
1. Android APK
1. Unity .so plugin
1. MCU-specific system image


# BUILDING eRT Locally

You can check eRT out onto a linux machine and run the build and some CI operations on your own hardware.

The command steps needed to build EHS for a specific target:

Speific OS's may have variations with additional assets, including supervisors and other system specific utilities.
These can found in `/target/envtree/` The most general version of this is in `.../Generic-ehs-tree/root-ehs_dir\*`

The command steps needed to build eRT for a specific targets and different deployment/packaging options:
```bash
make help 
```

to get a list of supported targets types
```bash
./configure
```

The `./configure` sets the source tree to bild the specific platform required and all following make commands will be configured to do the right thing for the specific target after this. 

To edit the configuration file for your selected platform you can use

```bash
./configure -edit
```

Build System Requirements
-------------------------
The eRT build system ues `docker` to create some specific build environment, but needs a few linux packages installed first to get started.

The simplest way to check your system is to use the make prepdeps command. e.g. 
```bash
./configure linux_x86_64_clang
make prepdeps.
```

This will install git build-essential (GNU make) and docker and two other large inxware repositories containing toolchains and prebuilts:
`ert-build-support` - contains binary tolchains and uses git lfs.
`ert-contrib-middlware` - is contains pre-built 3rd party dependencies and builts and scripts for re-creating or updating

These repositories are cloned into adjascent directories to ert-components when make prepdeps os first ran on a target (see below).

40GB of space is required for the 3 repositories. A further few GBs is recommended for built binaries and installable packages.

Example Build Sequence
-----------------------
The eRT build system will build, packafge and deploy eRT to many different devices types, operating system package formats and OTA deployment servers.

The steps to build a linux application you can run on a Debian 11 desktop with graphics: 

```bash
./configure linux_x86_64_gtk_gst_debian11-debug  # Choose 
make prepdeps
make all_docker
make targetenv 
make targetenv_version
make targetenv_deb
```

Example of Running a Basic Deployment
-------------------------------------
The `make targetenv` step above will assemble the eRT binary and supporting files in the 
staging directory located in ../TARGET_TREES/ehs-env_<your target platform name>

A minimum install includes just the bianry and the Lucid SODL file: 
```bash
./bin/ehs.exe          #<-- you run this!
./apps/default/t.sdl   #<-- ehs.exe runs this Lucid application by default
```

Alternatively you can run or debug eRT with the following shortcuts:
```
./configure -run
./configure -debug
```

Developing New Components
=========================
TODO - reference documentation and iCB tools & CDF (Component Description Files)

Useful utilities
----------------
To update your development environment's Lucid tools install with new or altered CDF files:

```bash
make toolsenv_update
```

Version Control
===============
eRT 
---
The following version information file is auto integrated by make targetenv_version
```bash
Releases/version_strings 
```
Containing
```
2 - Major version (Manually updated)
2 - Min version (Manually updated)
463 - Release number (Auto incremented and shared in Repo)
```

System Supervisors
------------------

Some platforms can include a system supervisor to perform OTA updates, System monitoring and connection diversity.
 

