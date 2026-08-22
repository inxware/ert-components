# Axelera AI Metis NPU cross-compilation image for Raspberry Pi (arm64)
#
# Built from the same debian:12 baseline as linux_arm64_lvgl_raspberrypi_demo,
# with the Axelera Voyager SDK installed instead of HailoRT.
#
# Axelera APT repository (public, no auth required):
#   https://software.axelera.ai/artifactory/axelera-apt-source/
# GPG key URL:
#   https://software.axelera.ai/artifactory/api/security/keypair/axelera/public
# Distribution string: ubuntu24 main  (Debian 12 bookworm shares the same ABI base)
#
# Packages installed for arm64 cross-compilation:
#   axelera-runtime-<VER>:arm64  — runtime headers, shared libs, pkg-config
#   axelera-device-<VER>:arm64   — device firmware blobs
# NOT installed (only needed on target hardware at runtime):
#   metis-dkms              — PCIe kernel driver
#   axelera-multi-device    — multi-card daemon

FROM debian:12
RUN echo "Successfully entered and fetched debian:12"
RUN apt update
RUN echo "installing build essential and git"

RUN dpkg --add-architecture arm64
RUN apt update -y --fix-missing
RUN apt upgrade -y

#######################################################
# Core build tools
#######################################################

RUN apt install -y \
    build-essential \
    cmake \
    libtinfo5 \
    git \
    git-lfs \
    clang \
    llvm \
    binutils-aarch64-linux-gnu \
    libc6-arm64-cross \
    libgcc-12-dev-arm64-cross \
    libstdc++-12-dev-arm64-cross \
    wget \
    sudo

########################################################
# Target Platform Component Dependencies
########################################################

RUN apt install -y \
    libcurl4-openssl-dev:arm64 \
    libarchive-dev:arm64 \
    libc6-dev:arm64 \
    libz-dev:arm64 \
    zlib1g-dev:arm64 \
    libexpat-dev:arm64 \
    libexpat-dev:arm64 \
    libexpat1-dev:arm64 \
    libidn2-dev:arm64 \
    libidn2-0-dev:arm64  \
    libxml2-dev:arm64 \
    libgtk-3-dev:arm64 \
    libsdl2-dev:arm64 \
    libgstreamer1.0-dev:arm64 \
    libgstreamer-plugins-base1.0-dev:arm64 \
    libopencv-dev:arm64 \
    libcamera-dev:arm64

#####################################################
# Other Debian packages directly from vendors
#####################################################

ARG WIRINGPI_VERSION=3.16
RUN wget https://github.com/WiringPi/WiringPi/releases/download/${WIRINGPI_VERSION}/wiringpi_${WIRINGPI_VERSION}_arm64.deb && dpkg -i wiringpi_${WIRINGPI_VERSION}_arm64.deb
RUN rm wiringpi_*_arm64.deb ; \
    rm /lib/libwiringPi*.so ; \
    cd /lib/ ; ln -s libwiringPi.so.${WIRINGPI_VERSION} libwiringPi.so ; \
    ln -s libwiringPiDev.so.${WIRINGPI_VERSION} libwiringPiDev.so
RUN echo "deb [ trusted=yes ] http://archive.raspberrypi.com/debian/ bookworm main" | tee /etc/apt/sources.list.d/raspi.list
RUN apt update && apt install -y pigpio:arm64

#####################################################
# Axelera Voyager SDK (arm64 cross-compile headers + libs)
#####################################################

ARG AXELERA_SDK_VERSION=1.6.0

RUN apt install -y curl gnupg \
    && mkdir -p /etc/apt/keyrings \
    && curl -fsSL "https://software.axelera.ai/artifactory/api/security/keypair/axelera/public" \
       | gpg --dearmor \
       | tee /etc/apt/keyrings/axelera.gpg > /dev/null \
    && echo "deb [signed-by=/etc/apt/keyrings/axelera.gpg] https://software.axelera.ai/artifactory/axelera-apt-source/ ubuntu24 main" \
       | tee /etc/apt/sources.list.d/axelera.list > /dev/null \
    && apt update

RUN apt install -y --no-install-recommends \
    axelera-runtime-${AXELERA_SDK_VERSION}:arm64 \
    axelera-device-${AXELERA_SDK_VERSION}:arm64 \
    && rm -rf /var/lib/apt/lists/*

######################################################################################
# Boiler plate for the build system

RUN adduser docker && mkdir -p /etc/sudoers.d \
    && echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker \
    && chmod 0440 /etc/sudoers.d/docker
USER docker
WORKDIR /home/docker
RUN whoami
RUN sudo whoami
