# Dockerfile for XMOS xcore.ai / FreeRTOS build environment
#
# Installs XMOS XTC Tools (proprietary, free-to-use, NOT redistributable).
#
# *** DO NOT PUSH THIS IMAGE TO A PUBLIC REGISTRY ***
# The built image contains XMOS XTC Tools binaries. Distributing it via
# DockerHub or any public registry would violate the XMOS licence.
# Each developer must build this image locally using:
#   make build_docker_local   (from ert-components with TARGET=xcore_freertos-xcore-base)
#
# *** BEFORE BUILDING — download XTC Tools manually ***
# XTC Tools require an XMOS developer account login and licence acceptance.
# There is no unauthenticated download URL.
#
#   1. Register/log in at https://www.xmos.com/software-tools/
#   2. Download the Linux 64-bit XTC Tools archive for your chosen version.
#      The filename will be similar to: Tools-15---Linux-64_15.3.1.tgz
#   3. Rename the downloaded file to: xtc-tools-linux.tgz
#   4. Place it in this directory:
#        ert-components/target/platform/xcore_freertos-xcore-base/xtc-tools-linux.tgz
#      (it is .gitignored and will not be committed to the repository)
#   5. Run: make build_docker_local
#
# XMOS_TOOL_PATH is baked into the image as a Docker ENV at build time.
# It does NOT need to be in INX_ERTCOMPONENTS_BUILDENV in
# target/envbuildscripts/target_buildenv_run_command.sh.

FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive

# XTC Tools version — update this ARG to match the version you downloaded.
# Also update COMPONENT_BASE_TECHNOLOGIES in config.mk if the major version changes.
ARG XTC_VERSION=15.3.1

# Base directory into which the archive is extracted.
# The XMOS XTC Tools archive has the internal structure:
#   <top-level-dir>/XTC/<version>/bin/xcc
#                                /include/
#                                /lib/
#                                ...
# With --strip-components=1 the top-level directory is removed, so extracting
# into XMOS_INSTALL_BASE produces:
#   ${XMOS_INSTALL_BASE}/XTC/${XTC_VERSION}/bin/xcc   ← correct
ARG XMOS_INSTALL_BASE=/opt/XMOS

# Install host build dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    git \
    git-lfs \
    python3 \
    python3-pip \
    python3-venv \
    curl \
    unzip \
    xz-utils \
    libusb-1.0-0-dev \
    udev \
    ca-certificates \
    xxd \
    && rm -rf /var/lib/apt/lists/*

# Install Python tools needed by XMOS SDK build scripts (xcommon_cmake, fwk_rtos)
RUN pip3 install --no-cache-dir \
    pyyaml \
    pyelftools

# Install XTC Tools from the locally downloaded archive.
# The file must be placed at:
#   target/platform/xcore_freertos-xcore-base/xtc-tools-linux.tgz
# before running 'make build_docker_local'. See instructions at the top of this file.
COPY target/platform/xcore_freertos-xcore-base/xtc-tools-linux.tgz /tmp/xtc-tools.tgz
RUN mkdir -p ${XMOS_INSTALL_BASE} \
    && tar -xf /tmp/xtc-tools.tgz -C ${XMOS_INSTALL_BASE} --strip-components=1 \
    && chmod -R a+rX ${XMOS_INSTALL_BASE} \
    && rm /tmp/xtc-tools.tgz

# Set XMOS_TOOL_PATH so xcc can locate device target files, builtins, and libraries.
# Both variables are baked into the image at build time — they do not need to be
# passed at runtime via INX_ERTCOMPONENTS_BUILDENV.
ENV XMOS_TOOL_PATH=${XMOS_INSTALL_BASE}/XTC/${XTC_VERSION}
ENV PATH="${XMOS_INSTALL_BASE}/XTC/${XTC_VERSION}/bin:${PATH}"

# Create the inxware build user matching the host uid/gid pattern
RUN groupadd --gid 1000 inxware \
    && useradd --uid 1000 --gid 1000 --create-home --shell /bin/bash inxware

USER inxware
WORKDIR /inxware/ert-components
