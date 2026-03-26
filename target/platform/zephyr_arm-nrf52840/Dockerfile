# inxware/zephyr-sdk-arm-nrf52840
# Dockerfile for Zephyr SDK build environment targeting nRF52840
# Based on Ubuntu 22.04 with Zephyr SDK and west tools installed

FROM ubuntu:22.04

ARG DEBIAN_FRONTEND=noninteractive

# Zephyr SDK version
ARG ZEPHYR_SDK_VERSION=0.16.8
ARG ZEPHYR_SDK_INSTALL_DIR=/opt/zephyr-sdk

# Install system dependencies required by Zephyr
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    ninja-build \
    ccache \
    git \
    git-lfs \
    python3 \
    python3-pip \
    python3-venv \
    python3-setuptools \
    python3-wheel \
    wget \
    curl \
    file \
    gperf \
    xz-utils \
    device-tree-compiler \
    libsdl2-dev \
    libffi-dev \
    libssl-dev \
    libyaml-dev \
    && rm -rf /var/lib/apt/lists/*

# Install west and other Python dependencies
RUN pip3 install --no-cache-dir \
    west \
    pyelftools \
    pyyaml \
    pykwalify \
    canopen \
    packaging \
    progress \
    psutil \
    pylink-square \
    pyocd \
    && pip3 install --no-cache-dir 'intelhex>=2.6.1'

# Download and install Zephyr SDK (ARM toolchain only to save space)
RUN mkdir -p ${ZEPHYR_SDK_INSTALL_DIR} \
    && wget -q https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v${ZEPHYR_SDK_VERSION}/zephyr-sdk-${ZEPHYR_SDK_VERSION}_linux-x86_64_minimal.tar.xz \
    && tar xf zephyr-sdk-${ZEPHYR_SDK_VERSION}_linux-x86_64_minimal.tar.xz -C /opt/ \
    && rm zephyr-sdk-${ZEPHYR_SDK_VERSION}_linux-x86_64_minimal.tar.xz \
    && cd /opt/zephyr-sdk-${ZEPHYR_SDK_VERSION} \
    && ./setup.sh -t arm-zephyr-eabi -c

# Set Zephyr SDK environment
ENV ZEPHYR_SDK_INSTALL_DIR=/opt/zephyr-sdk-${ZEPHYR_SDK_VERSION}
ENV ZEPHYR_TOOLCHAIN_VARIANT=zephyr

# Add Zephyr SDK toolchain to PATH
ENV PATH="${ZEPHYR_SDK_INSTALL_DIR}/arm-zephyr-eabi/bin:${PATH}"

# Create build user (matching host uid/gid pattern from other Dockerfiles)
RUN groupadd -g 1000 inxware \
    && useradd -m -u 1000 -g 1000 -s /bin/bash inxware \
    && echo 'inxware ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers

USER inxware
WORKDIR /home/inxware
