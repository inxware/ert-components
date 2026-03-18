# Dockerfile for linux_arm64_lvgl_jetson_nano
#
# Cross-compilation environment: Ubuntu 22.04 x86_64 host → arm64 Jetson target.
#
# CUDA and TensorRT headers
# -------------------------
# Installed from NVIDIA's public Jetson OTA apt repository at
# repo.download.nvidia.com/jetson/ — no NGC account or API key required.
# libnvinfer-dev:arm64 and cuda-cudart-dev-cross-aarch64 install headers to
# /usr/include/aarch64-linux-gnu/, the standard multiarch cross-compilation path.
# Only headers and link stubs are installed; runtime libraries (libcudart.so,
# libnvinfer.so) live on the Jetson and are not needed at compile time.
#
# GStreamer
# ---------
# GStreamer development headers are installed from the Ubuntu 22.04 ports repo.
# The Jetson-specific GStreamer plugins (nvarguscamerasrc, nvvidconv) are only
# available at runtime on the Jetson and are not needed for compilation.
#
# See README.md for full setup instructions.

FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN echo "Jetson Orin Nano arm64 cross-compilation environment"
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
# ports repo (Universe packages are not always built for every arch).
# Building from source is straightforward — Paho MQTT C has no heavy deps.
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
# NVIDIA repositories — two repos, two purposes
#
# Cross-compiling TensorRT code requires headers from two distinct NVIDIA
# sources that cannot be consolidated:
#
# 1. NVIDIA CUDA compute repo (developer.download.nvidia.com/compute/cuda)
#    Provides: cuda-cross-aarch64 — CUDA headers packaged specifically for
#    x86_64 host → arm64 cross-compilation, including crt/host_defines.h.
#    Added below before the Jetson OTA repo.
#
# 2. NVIDIA Jetson OTA repo (repo.download.nvidia.com/jetson)
#    Provides: libnvinfer-dev:arm64 — TensorRT headers as proper arm64
#    multiarch packages that install to /usr/include/aarch64-linux-gnu/.
#    The Jetson OTA repo does NOT have usable CUDA cross-compilation headers
#    for an x86_64 host (its CUDA packages are native Jetson packages).
#
# The alternative that avoids this two-repo complexity is to base FROM an
# NVIDIA cross-compilation container (nvcr.io) which has both pre-configured,
# but that requires a free NGC account for docker login.
########################################################

RUN wget -qO - https://repo.download.nvidia.com/jetson/jetson-ota-public.asc \
    | gpg --dearmor -o /usr/share/keyrings/nvidia-jetson.gpg

RUN echo "deb [arch=arm64 signed-by=/usr/share/keyrings/nvidia-jetson.gpg] https://repo.download.nvidia.com/jetson/common r36.4 main" \
        > /etc/apt/sources.list.d/nvidia-jetson.list

RUN apt update

########################################################
# CUDA and TensorRT arm64 development headers
#
# Installed from the Jetson OTA repo above — arm64 packages that go to the
# correct cross-compilation paths under /usr/include/aarch64-linux-gnu/.
# No x86_64 system headers are touched; no staging/symlink hacks needed.
########################################################

########################################################
# CUDA cross-compilation headers (NVIDIA CUDA compute repo)
#
# The Jetson OTA repo has native arm64 CUDA packages that do not install the
# crt/ subdirectory correctly on an x86_64 host.  NVIDIA's separate CUDA
# compute repo provides cuda-cross-aarch64 which is specifically packaged for
# cross-compilation and puts crt/host_defines.h in the right place.
########################################################

RUN wget -qO /tmp/cuda-keyring.deb \
    https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2204/x86_64/cuda-keyring_1.1-1_all.deb \
    && dpkg -i /tmp/cuda-keyring.deb && rm /tmp/cuda-keyring.deb

RUN apt update && apt install -y cuda-cudart-dev-12-6

########################################################
# TensorRT arm64 headers (Jetson OTA repo)
########################################################

RUN apt install -y \
    libnvinfer-dev:arm64 \
    && apt install -y libnvinfer-headers-dev:arm64 || true \
    && apt install -y libnvinfer-plugin-dev:arm64 || true

########################################################
# CUDA arm64 runtime headers
#
# cuda-cudart-dev-12-6:arm64 cannot be installed alongside the amd64 version
# (both write to /usr/local/cuda-12.6/ and share some top-level files).
# Instead, download the deb and extract only the arm64 targets/ subtree —
# it installs cleanly under targets/aarch64-linux/ with no file conflicts.
# This provides cuda_runtime_api.h and crt/host_defines.h for the aarch64
# cross-compilation target.
# Also create the /usr/local/cuda → /usr/local/cuda-12.6 convenience symlink
# (normally created by the cuda-toolkit meta-package, not cuda-cudart-dev alone).
########################################################

RUN ln -sfn /usr/local/cuda-12.6 /usr/local/cuda \
    && cd /tmp \
    && apt download cuda-cudart-dev-12-6:arm64 \
    && mkdir cuda-arm64 \
    && dpkg -x cuda-cudart-dev-12-6*.deb cuda-arm64 \
    && cp -a cuda-arm64/usr/local/cuda-12.6/targets/aarch64-linux \
             /usr/local/cuda-12.6/targets/ \
    && rm -rf /tmp/cuda-arm64 /tmp/cuda-cudart-dev-12-6*.deb

# temp/TensorRT/include/ fallback: if the Jetson OTA repo is unavailable
# (e.g. offline build), manually download TensorRT from
# https://developer.nvidia.com/tensorrt, extract its include/ directory to
# temp/TensorRT/include/ in the repo root, then rebuild the image.
# The COPY below is a no-op when only the .gitkeep placeholder is present.
COPY temp/TensorRT/include/ /usr/include/aarch64-linux-gnu/

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
