![Static Badge](https://img.shields.io/badge/RaspberryPi-OK-green) ![Static Badge](https://img.shields.io/badge/Arduino_nano_connect-OK-green) ![Static Badge](https://img.shields.io/badge/esp32-OK-green) ![Static Badge](https://img.shields.io/badge/esp32s3-OK-green) ![Static Badge](https://img.shields.io/badge/x86_Windows-OK-green) ![Static Badge](https://img.shields.io/badge/Android-OK-green) ![Static Badge](https://img.shields.io/badge/x86_64_linux-OK-green) ![Static Badge](https://img.shields.io/badge/arm64_linux_RDK-TBC-amber) 

[![build-linux_arm64_lvgl_gg_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_lvgl_gg_debian11.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_lvgl_gg_debian11.yml)
[![build-linux_arm64_gtk_gst_gg_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_gtk_gst_gg_debian11.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_arm64_gtk_gst_gg_debian11.yml)
[![build-linux_android_arm64](https://github.com/inxware/ert-components/actions/workflows/build-linux_android_arm64.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_android_arm64.yml)
[![build-esp32s3_freertos-xtensa-community](https://github.com/inxware/ert-components/actions/workflows/build-esp32s3_freertos-xtensa-community.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-esp32s3_freertos-xtensa-community.yml)
[![build-linux_x86_64_clang_gtk_gst_gg_debian11](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64_clang_gtk_gst_gg_debian11.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64_clang_gtk_gst_gg_debian11.yml)
[![build-win_x86_32-lucid-win10](https://github.com/inxware/ert-components/actions/workflows/build-win_x86_32-lucid-win10.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-win_x86_32-lucid-win10.yml)
[![.github/workflows/build-linux_x86_64-lucid-debian11.yml](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64-lucid-debian11.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64-lucid-debian11.yml)
[![.github/workflows/build-linux_x86_64-lucid-debian11.yml](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64-lucid-debian11.yml/badge.svg)](https://github.com/inxware/ert-components/actions/workflows/build-linux_x86_64-lucid-debian11.yml)

What is eRT & eRT Components?
============
eRT is  runtime environment for inxware-lucid applications for embedded systems.
It comprises native C/C++ components, component interface descriptors and an event driven kernel that executes functions within the components.  
The inxware lucid IDE is available from  https://appland.inxware.io/

Device Support
==============
inxware eRT can run on any CPU architecture and use any OS or baremetal SDK.
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


Copying & Terms of Use
======================

eRT components are licenced under LGPLv3 license (See LICENSE.md) unless otherwise stated within sub modules from 3rd-parties. 

Full firmware builds contain the inxware kernel library which is a 100% portable closed source element of inxware and is licensed for commercial and non-commercial use for up to 10 copies per user or organisation.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE

About this Repository
---------------------
The ```ert-components``` repository is the core repository needed to be the inxware eRT runtime. 
This build system currently supports the following hardware architectures and operating systems: 

The build system support fine-grained target configuration for particular hardware and user configurations, 
including package management and direct installation. To see the full list type `./configure` in the root of this repository.

The eRT runtime also contains various options for **System Supervisors**, which may take the form of scripts,deamons, apps or a sparate executable binary, These are typically applicable to multi-tasking operating systems such as Linux, Android, Windows where maintenance of the OS may be required via Devman. The System Supervisors are usually also responsble for providing eRt softare updates and disaster recovery as an independent process eRT binary itself.
# Build System Requirements
- The eRT build system ues docker to create some specific build environment, but needs a few linux packages installed first to get started.
- The simplest way to check your system is to use the make prepdeps command. e.g. 
```
./configure linux_x86_64_clang
make prepdeps.
```

This will install git build-essential (GNU make) and docker and two other large inxware repositories containing toolchains and prebuilts:
ert-build-support - contains binary tolchains and uses git lfs.
ert-contrib-middlware - is contains pre-built 3rd party dependencies and builts and scripts for re-creating or updating

These repositories are cloned into adjascent directories to ert-components when make prepdeps os first ran on a target (see below).

40GB of space is required for the 3 repositories. A further few GBs is recommended for built binaries and installable packages.

eRT requires a proprietory event handling kernel library to be linked to the application. This binary can be copied and deployed for non-commercial applications without prior-agreement with inx limited. Before deploying commercial applications (i.e. copying the eRT kernel in commercial products and services you must first contact inxware@inx-systems.com). 

These repositories are cloned into adjascent directories to ert-components when make prepdeps os first ran on a target (see below).

40GB of space is required for the 3 repositories. A further few GBs is recommended for built binaries and installable packages.

Manifest
========

eRT runs application built with the inxware iAB tools, which can be downloaded from https://github.com/inxware/inxware-dev-tools

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

### Android System Supervisor 
---
```bash
./target/envtree/android-ehs-tree/root-dir/system/etc/ehs_supervisor/version
```

### Gnu Linux 
No version control for cron-based update scripts is currently supported

