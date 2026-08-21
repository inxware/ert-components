# inxware/inx-debian12-clang14-qt
#
# x86_64 build image: Debian 12 + Clang 14 + Qt 6 dev packages.
#
# Native amd64 counterpart of inxware/inx-debian12-clang-arm-qt
# (target/platform/linux_arm64_qt_debian12-no-certs/Dockerfile). Same Debian
# release, same Qt 6 packages; everything specific to cross-compiling to arm64
# or to Raspberry Pi hardware is dropped:
#
#   - no `dpkg --add-architecture arm64` and no :arm64 package suffixes
#   - no binutils-aarch64-linux-gnu / libc6-arm64-cross / libgcc-12-dev:arm64 /
#     libstdc++-12-dev:arm64, and no gcc-multilib / g++-multilib
#   - no wiringPi, no pigpio, no archive.raspberrypi.com apt source — this
#     target sets EHS_PERIPHERALS_GPIO_SUPPORT=stubbed
#   - no HailoRT .deb (arm64-only, and this target does not enable
#     EHS_ML_HARDWARE_ACCELERATION)
#
# Extends inxware/inx-debian12-clang14 (defined in ert-contrib-middleware
# inx_build_scripts/Dockerfile.debian12-clang14), the shared amd64 Debian 12
# baseline that already provides build-essential, cmake, ninja, clang/llvm/lld,
# git/git-lfs, rsync, wget, curl, pkg-config and libssl-dev. Sharing that base
# rather than starting from debian:12 keeps one toolchain image across the
# amd64 targets — see CLAUDE.md "SDKs are shared across targets".
#
# Clang 14 matches EHS_GNU_OS_VERSION=-clang14_debian12 in config.mk, so the
# compiler agrees with the contrib archives under
# target_libs/x86_64-linux-gnu-clang14_debian12_base/.
#
# Build with:  make build_docker_local

FROM inxware/inx-debian12-clang14

########################################################
# Target platform library dependencies (native amd64)
########################################################

RUN apt-get update && apt-get install -y \
    sudo \
    adduser \
    libcurl4-openssl-dev \
    libarchive-dev \
    libc6-dev \
    zlib1g-dev \
    libexpat-dev \
    libexpat1-dev \
    libidn2-dev \
    libxml2-dev \
    libgtk-3-dev \
    libsdl2-dev \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev \
    libopencv-dev \
    libcamera-dev \
    gnupg \
    && rm -rf /var/lib/apt/lists/*

########################################################
# Qt 6
#
# Qt 5 is deliberately not installed — supporting both in one image has caused
# ambiguous builds before (see the arm64 Qt Dockerfile). config.mk sets
# EHS_GUI_SUPPORT_QT6=yes.
########################################################

RUN apt-get update && apt-get install -y \
    qt6-base-dev \
    qt6-base-dev-tools \
    qt6-declarative-dev \
    qt6-declarative-dev-tools \
    qml6-module-qtquick \
    qml6-module-qtquick-timeline \
    && rm -rf /var/lib/apt/lists/*

######################################################################################
# Boilerplate for the build system

RUN adduser docker && mkdir -p /etc/sudoers.d \
    && echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker \
    && chmod 0440 /etc/sudoers.d/docker
USER docker
WORKDIR /home/docker
RUN whoami
RUN sudo whoami
