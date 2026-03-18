# Dockerfile for linux_arm64_lvgl_jetson_nano_ngc
#
# Cross-compilation environment: NVIDIA NGC JetPack cross-compilation container
# (x86_64 host → arm64 Jetson target).
#
# Base image
# ----------
# nvcr.io/nvidia/jetpack-linux-aarch64-crosscompile-x86:6.1
#   - JetPack 6.1 / L4T r36.4 (also covers JetPack 6.2 — same library stack)
#   - Cross-compiler: /l4t/toolchain/aarch64-glibc-stable-2022.08-1/bin/
#   - Target sysroot: /l4t/targetfs/  (arm64 JetPack rootfs)
#   - CUDA 12.6 headers + libs in sysroot
#   - TensorRT headers + libs in sysroot
#
# Authentication
# --------------
# Pulling this image requires a free NVIDIA NGC account:
#   1. Register at https://ngc.nvidia.com (free)
#   2. Generate an API key under "Setup > API Key"
#   3. docker login nvcr.io --username '$oauthtoken' --password '<your-api-key>'
# This only needs to be done once per host machine.
#
# CUDA and TensorRT
# -----------------
# The NGC base image pre-installs CUDA 12.6 and TensorRT headers and link
# stubs at the standard multiarch paths (/usr/include/aarch64-linux-gnu/,
# /usr/local/cuda-12.6/, /usr/lib/aarch64-linux-gnu/).  No NVIDIA apt
# repositories are needed.  Only a /usr/local/cuda convenience symlink is
# created so that ml_fw_tensorrt.mk include paths resolve correctly.
#
# GStreamer and other arm64 libraries
# ------------------------------------
# These are not part of the JetPack sysroot so are installed via the standard
# Ubuntu 22.04 arm64 multiarch mechanism (ports.ubuntu.com), the same approach
# used by the non-NGC jetson_nano target.
#
# See README.md for full setup instructions.

FROM nvcr.io/nvidia/jetpack-linux-aarch64-crosscompile-x86:6.1

ENV DEBIAN_FRONTEND=noninteractive

RUN echo "NGC JetPack 6.1 arm64 cross-compilation environment"
RUN apt update && apt upgrade -y

#######################################################
# Core build tools (host — x86_64)
#######################################################

RUN apt install -y \
    build-essential \
    cmake \
    git \
    git-lfs \
    clang \
    llvm \
    binutils-aarch64-linux-gnu \
    crossbuild-essential-arm64 \
    libc6-dev-arm64-cross \
    sudo \
    wget \
    gnupg \
    ca-certificates \
    apt-transport-https \
    software-properties-common \
    python3

########################################################
# arm64 multiarch setup
#
# archive.ubuntu.com only serves amd64/i386 — arm64 packages live at
# ports.ubuntu.com.  Replace sources.list entirely with arch-qualified
# entries rather than patching it with sed (sed is fragile if existing
# entries already carry options, producing malformed double-bracket lines
# that cause GPG verification failures for all repos).
########################################################

RUN dpkg --add-architecture arm64

RUN printf '%s\n' \
    'deb [arch=amd64,i386] http://archive.ubuntu.com/ubuntu jammy main restricted universe multiverse' \
    'deb [arch=amd64,i386] http://archive.ubuntu.com/ubuntu jammy-updates main restricted universe multiverse' \
    'deb [arch=amd64,i386] http://archive.ubuntu.com/ubuntu jammy-backports main restricted universe multiverse' \
    'deb [arch=amd64,i386] http://security.ubuntu.com/ubuntu jammy-security main restricted universe multiverse' \
    'deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports jammy main restricted universe multiverse' \
    'deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports jammy-updates main restricted universe multiverse' \
    'deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports jammy-security main restricted universe multiverse' \
    'deb [arch=arm64] http://ports.ubuntu.com/ubuntu-ports jammy-backports main restricted universe multiverse' \
    > /etc/apt/sources.list

RUN apt update

########################################################
# Core arm64 runtime and development libraries
# (from Ubuntu 22.04 multiarch — no Jetson-specific repo needed for these)
########################################################

RUN apt install -y \
    libc6:arm64 \
    libc6-dev:arm64 \
    libz-dev:arm64 \
    zlib1g-dev:arm64 \
    libssl-dev:arm64 \
    libcurl4-openssl-dev:arm64 \
    libarchive-dev:arm64 \
    libexpat-dev:arm64 \
    libidn2-dev:arm64 \
    libxml2-dev:arm64

########################################################
# GStreamer arm64 development headers
# The Jetson-specific plugins (nvarguscamerasrc, nvvidconv)
# are runtime-only — not needed at compile time.
########################################################

RUN apt install -y \
    libgstreamer1.0-dev:arm64 \
    libgstreamer-plugins-base1.0-dev:arm64 \
    gstreamer1.0-tools

########################################################
# LVGL display dependencies (SDL2 for framebuffer output)
########################################################

RUN apt install -y \
    libsdl2-dev:arm64 \
    libdrm-dev:arm64 \
    libgbm-dev:arm64

########################################################
# OpenGL / EGL stubs (LVGL uses EGL on Jetson via the L4T OpenGL driver)
########################################################

RUN apt install -y --no-install-recommends \
    libglvnd-dev:arm64 \
    libegl1-mesa-dev:arm64 \
    libgles2-mesa-dev:arm64 \
    libgl1-mesa-dev:arm64

########################################################
# Networking and middleware dependencies
########################################################

RUN apt install -y \
    libglib2.0-dev:arm64

########################################################
# Paho MQTT C — cross-compiled from source for arm64
#
# libpaho-mqtt-c-dev:arm64 is not reliably available in the Ubuntu 22.04
# ports repo.  Building from source is straightforward.
#
# Installs to /usr with libs under lib/aarch64-linux-gnu/ so the cross
# linker finds them the same way as any other arm64 apt package.
########################################################

ARG PAHO_MQTT_C_VERSION=1.3.13
RUN git clone --depth 1 --branch v${PAHO_MQTT_C_VERSION} \
        https://github.com/eclipse/paho.mqtt.c.git /tmp/paho-mqtt-c \
    && cd /tmp/paho-mqtt-c && mkdir build && cd build \
    && cmake .. \
        -DCMAKE_SYSTEM_NAME=Linux \
        -DCMAKE_SYSTEM_PROCESSOR=aarch64 \
        -DCMAKE_C_COMPILER=aarch64-linux-gnu-gcc \
        -DCMAKE_INSTALL_PREFIX=/usr \
        -DCMAKE_INSTALL_LIBDIR=lib/aarch64-linux-gnu \
        -DPAHO_WITH_SSL=ON \
        -DPAHO_BUILD_STATIC=OFF \
        -DOPENSSL_SSL_LIBRARY=/usr/lib/aarch64-linux-gnu/libssl.so \
        -DOPENSSL_CRYPTO_LIBRARY=/usr/lib/aarch64-linux-gnu/libcrypto.so \
        -DOPENSSL_INCLUDE_DIR=/usr/include \
    && make -j$(nproc) \
    && make install \
    && rm -rf /tmp/paho-mqtt-c

########################################################
# CUDA and TensorRT — pre-installed by the NGC base image
#
# The NGC base image already has everything at the standard multiarch paths:
#   TRT headers:  /usr/include/aarch64-linux-gnu/NvInfer.h etc.
#   CUDA headers: /usr/local/cuda-12.6/targets/aarch64-linux/include/
#   TRT libs:     /usr/lib/aarch64-linux-gnu/libnvinfer.so etc.
#
# No NVIDIA apt repositories or package extraction needed.
# Only the /usr/local/cuda convenience symlink is missing — create it so
# ml_fw_tensorrt.mk's include path (/usr/local/cuda/targets/aarch64-linux/include)
# resolves correctly.
########################################################

RUN ln -sfn /usr/local/cuda-12.6 /usr/local/cuda

########################################################
# Boilerplate for the build system
########################################################

RUN adduser --disabled-password --gecos "" docker \
    && mkdir -p /etc/sudoers.d \
    && echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker \
    && chmod 0440 /etc/sudoers.d/docker
USER docker
WORKDIR /home/docker
RUN whoami
RUN sudo whoami
