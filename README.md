# eRT Platform build

The eRT-components repository is the hub repository needed to build the inxware eRT runtime. The repo contains a build and test CI for 
validating publically supported hardware and can be extended (pull requests accepted) to extend support for current and future hardware.

This build system currently supports the following hardware architectures and operating systems: 

* arm7/arm64 (bare-bones/freeRTOS/Linux/Android), 
* xtensor (esp32/FreeRTOS)
* x86/x86_64 (linux/windows)

The build system support fine grained target configuration for particular hardware and user configurations, 
including package management and direct installation. To see the full list type ./configure in the root of this repository

# System Requirements

The eRT build system ues docker to create some specific build environment, but needs a few linux packages installed first to get started.

The simplest way to check your system is to use the make prepdeps command. e.g. 
```
./configure linux_x86_64_clang
make prepdeps.
```

This will install git build-essential (GNU make) and docker and two other large inxware repositories containing toolchains and prebuilts:
ert-build-support - contains binary tolchains and uses git lfs.
ert-contrib-middlware - is contains pre-built 3rd party dependencies and builts and scripts for re-creating or updating

These repositories are cloned into adjascent directories to ert-components when make prepdeps os first ran on a target (see below).

40GB of space is required for the 3 repositories. A further few GBs is recommended for built binaries and installable packages.


# MANIFEST


eRT runs application built with the inxware iAB tools, which can be downloaded from https://github.com/inxware/inxware-dev-tools

An open source publication of this eRT runtime is hosted at https://github.com/inxware/ert-components

For further information about inxware IoT sftware development please seapproach read more here: https://www.inx-systems.com/dev-tools/ 

This build system for EHS requires linux (debian usually) and the following repos to be cecked out in to the same directory:

  ert-components         (Contains this readme file)
  ert-build-support      (Contains toolchains and Basic OS dependencies)
  ert-contrib-middleware (Contains optional pre-built components for each suport platform)

It is advisable to also check out the following repos to build production installs:
apps               (Contains applications which maybe installed into production packages)

All the above repos are available at ssh://inx-data@server/home/inx-data/data/Repos/*.git

# BUILDING eRT Locally

You can check eRT out onto a linux machine and run the build and some CI operations on your own hardware.

The command steps needed to build EHS for a specific target:

```make help ```

to get a list of supported targets type
``` ./configure help ```


# Specific Platform Notes

e.g.
```
./configure linux_x86_gtk_gst
make prepdeps
make all
make targetenv 
make targetenv_version
make targetenv_deb 
```
