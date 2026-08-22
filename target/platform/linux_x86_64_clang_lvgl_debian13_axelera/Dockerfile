# inxware/inx-debian13-clang-lvgl-axelera
#
# x86_64 build image: Debian 13 + Clang + LVGL deps + Axelera Voyager SDK.
# Extends inxware/inx-debian13-clang19 (defined in ert-contrib-middleware).
#
# Axelera APT repository (public, no auth):
#   https://software.axelera.ai/artifactory/axelera-apt-source/
# GPG key:
#   https://software.axelera.ai/artifactory/api/security/keypair/axelera/public
# Distribution string: ubuntu24 main
#
# Packages added on top of the base image:
#   axelera-runtime-<VER>  — SDK headers + shared libs (headers needed at build time)
#   axelera-device-<VER>   — device firmware blobs
# NOT installed (only needed on target hardware at runtime):
#   metis-dkms             — PCIe kernel driver

FROM inxware/inx-debian13-clang19

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
    libsdl2-dev \
    libopencv-dev \
    libavcodec-dev \
    libavformat-dev \
    libavutil-dev \
    libswscale-dev \
    libswresample-dev \
    libabsl-dev \
    libtbb-dev \
    gnupg \
    && rm -rf /var/lib/apt/lists/*

#####################################################
# Axelera Voyager SDK (amd64 — headers needed at build time)
#####################################################

ARG AXELERA_SDK_VERSION=1.6.0

RUN mkdir -p /etc/apt/keyrings \
    && curl -fsSL "https://software.axelera.ai/artifactory/api/security/keypair/axelera/public" \
       | gpg --dearmor \
       | tee /etc/apt/keyrings/axelera.gpg > /dev/null \
    && echo "deb [signed-by=/etc/apt/keyrings/axelera.gpg] https://software.axelera.ai/artifactory/axelera-apt-source/ ubuntu24 main" \
       | tee /etc/apt/sources.list.d/axelera.list > /dev/null \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
       axelera-runtime-${AXELERA_SDK_VERSION} \
       axelera-device-${AXELERA_SDK_VERSION} \
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
