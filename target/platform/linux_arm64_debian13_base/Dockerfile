FROM debian:13
RUN echo "Successfully entered and fetched debian:13"
RUN apt update
RUN echo "installing Pacakges from Debian"
# above is used while debugging and needs the bits at the end of ths file too

RUN dpkg --add-architecture arm64
# Not sure we always need to donthis:
RUN apt update --fix-missing
# Or this
RUN apt upgrade -y

#######################################################
# Core build tools
#######################################################

RUN apt install -y \
    build-essential \
    cmake \
    libtinfo6 \
    git \
    git-lfs \
    clang \
    llvm \
    binutils-aarch64-linux-gnu \
    libc6-arm64-cross \
    crossbuild-essential-arm64 \
    sudo \
    wget

#    gcc-multilib \
#    g++-multilib \

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

#   g++-aarch64-linux-gnu gcc-aarch64-linux-gnu 

# Pre-install mesa/GL packages explicitly to satisfy Qt6 EGL/GLX dependencies
# (libegl1 and libglx0 depend on mesa, which needs explicit arm64 cross-arch install)
RUN apt install -y --no-install-recommends \
     libglvnd0:arm64 \
     libglvnd-dev:arm64 \
     libegl-mesa0:arm64 \
     libglx-mesa0:arm64 \
     libegl1:arm64 \
     libglx0:arm64 \
     libgl1:arm64

# An the basic QT6 libraries needed for compilation
RUN apt install -y \
     qt6-base-dev:arm64 \
     qt6-base-dev-tools \
     qt6-declarative-dev:arm64 


#####################################################
# Othe Debian packages directly from vendors
#####################################################

ARG WIRINGPI_VERSION=3.16
RUN wget https://github.com/WiringPi/WiringPi/releases/download/${WIRINGPI_VERSION}/wiringpi_${WIRINGPI_VERSION}_arm64.deb && dpkg -i wiringpi_${WIRINGPI_VERSION}_arm64.deb 
# Tidy uo and make some soft links for the cross build
RUN rm wiringpi_*_arm64.deb ; \
    rm /lib/libwiringPi*.so ; \
    cd /lib/ ; ln -s libwiringPi.so.${WIRINGPI_VERSION} libwiringPi.so ; \
    ln -s libwiringPiDev.so.${WIRINGPI_VERSION} libwiringPiDev.so
# Adding this would cause problem.
# Only update, do not upgrade.
# pigpio:arm64 package directly installs to `/usr/lib/` instead of `/usr/lib/aarch64-linux-gnu/` and it does not install libc6:arm64 while looking for it.
## So it's neccesary to install the other proper arm64 package first to get the libc6:arm64 properly installed
RUN echo "deb [ trusted=yes ] http://archive.raspberrypi.com/debian/ bookworm main" | tee /etc/apt/sources.list.d/raspi.list
RUN apt update && apt install -y pigpio:arm64

# We don't really need to install hailo for non RPI, but below is how it can be done from 
# packages manually downloaded from the Hailo site for Hailo 8 Debian 13 version

ARG HAILO_SDK_VERSION=4.22.0
# We would want something like this:
# COPY ../ert-contrib-middleware/contrib/hailort/hailort_${HAILO_SDK_VERSION}_arm64.deb /tmp/hailort_${HAILO_SDK_VERSION}_arm64.deb
# But hacjed this because we can't reference ../ : COPY temp/hailort_${HAILO_SDK_VERSION}_arm64.deb /tmp/hailort_${HAILO_SDK_VERSION}_arm64.deb
#RUN dpkg -i /tmp/hailort_${HAILO_SDK_VERSION}_arm64.deb ; rm /tmp/hailort_*_arm64.deb


# No Hailo support for other Debian builds currently
# Alternatively we need to download from Hailo web site as Debian packages into ../contrib-middleware 
# This is done for Hailo 8 and Debian 12 (v4.22) - for Debian 13 we need (4.23) and for other Hailo hardware versions we need others too

######################################################################################
# Boilerplate for the build system
# We awnt to sudo to experiment with a few things:
# RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo
RUN adduser docker && mkdir -p /etc/sudoers.d \
    && echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker \
    && chmod 0440 /etc/sudoers.d/docker
USER docker
WORKDIR /home/docker
RUN whoami
RUN sudo whoami
