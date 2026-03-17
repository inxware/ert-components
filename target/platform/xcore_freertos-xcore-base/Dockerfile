# Dockerfile for XMOS xcore.ai / FreeRTOS build environment
#
# Installs XMOS XTC Tools (proprietary, free-to-use, NOT redistributable).
# XTC Tools are downloaded at image build time from the official XMOS release.
#
# *** DO NOT PUSH THIS IMAGE TO A PUBLIC REGISTRY ***
# Because the built image contains XMOS XTC Tools binaries, distributing it
# via DockerHub or any public registry would violate the XMOS license.
# This image must be built locally by each developer using:
#   make build_docker_local   (from ert-components with TARGET=xcore_freertos-xcore-base)
#
# XTC Tools download:
#   The download URL below follows the pattern used by the xmos/install-xtc-tools
#   GitHub Action. Verify the current URL at:
#     https://github.com/xmos/install-xtc-tools
#   or download manually from:
#     https://www.xmos.com/software/tools/
#
# XMOS_TOOL_PATH is set as an ENV so it is available to xcc at build time.
# It is also passed into the running container via INX_ERTCOMPONENTS_BUILDENV
# in target/envbuildscripts/target_buildenv_run_command.sh — add XMOS_TOOL_PATH
# to that variable list if it is not already present.

FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive

# XTC Tools version — update this ARG and rebuild the image when upgrading.
# The COMPONENT_BASE_TECHNOLOGIES in config.mk must match this version.
ARG XTC_VERSION=15.3.0
ARG XTC_INSTALL_DIR=/opt/XMOS/XTC/${XTC_VERSION}

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
    wget \
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
    cmake \
    ninja \
    pyyaml \
    pyelftools

# Download and install XTC Tools
# The Linux package is a self-extracting installer or a .tgz archive.
# URL pattern: https://s3.xmos.io/downloads/xtc/XTC-Tools-${XTC_VERSION}-Linux64.tgz
# Verify the exact filename at https://github.com/xmos/install-xtc-tools before building.
RUN mkdir -p ${XTC_INSTALL_DIR} \
    && wget -q --show-progress \
        "https://s3.xmos.io/downloads/xtc/XTC-Tools-${XTC_VERSION}-Linux64.tgz" \
        -O /tmp/xtc-tools.tgz \
    && tar -xf /tmp/xtc-tools.tgz --strip-components=1 -C ${XTC_INSTALL_DIR} \
    && rm /tmp/xtc-tools.tgz

# Set XMOS_TOOL_PATH so xcc can locate device target files, builtins, and libraries.
# This ENV is inherited by all processes run in the container.
ENV XMOS_TOOL_PATH=${XTC_INSTALL_DIR}
ENV PATH="${XTC_INSTALL_DIR}/bin:${PATH}"

# Create the inxware build user matching the host uid/gid pattern
RUN groupadd --gid 1000 inxware \
    && useradd --uid 1000 --gid 1000 --create-home --shell /bin/bash inxware

USER inxware
WORKDIR /inxware/ert-components
