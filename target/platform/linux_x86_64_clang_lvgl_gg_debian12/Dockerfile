# inxware/inx-debian12-clang14-lvgl-gg
#
# x86_64 build image: Debian 12 + Clang 14 + LVGL deps + AWS Greengrass build deps.
# Extends inxware/inx-debian12-clang14 (defined in ert-contrib-middleware
# inx_build_scripts/Dockerfile.debian12-clang14).  The base image already has
# build-essential, cmake, ninja, clang/llvm/lld, git/git-lfs, rsync, wget, curl,
# pkg-config and libssl-dev.
#
# This image adds the Debian 12 -dev packages needed by ert-components when it
# links against system libraries (curl, archive, xml2, ffmpeg, opencv, sdl2,
# absl, tbb).  No NPU SDK — that's added by the Axelera variant
# (inxware/inx-debian12-clang14-lvgl-axelera).

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

######################################################################################
# Boilerplate for the build system

RUN adduser docker && mkdir -p /etc/sudoers.d \
    && echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker \
    && chmod 0440 /etc/sudoers.d/docker
USER docker
WORKDIR /home/docker
RUN whoami
RUN sudo whoami
