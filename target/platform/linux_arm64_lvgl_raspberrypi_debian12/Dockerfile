FROM debian:12
RUN echo "Successfully entered and fetched debian:12"
RUN apt update
RUN echo "installing build essential and git"
# above is used while debugging and needs the bits at the end of ths file too

RUN dpkg --add-architecture arm64
# Not sure we always need to do this:
RUN apt update --fix-missing
# Or this
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
    gcc-multilib \
    g++-multilib \
    binutils-aarch64-linux-gnu \
    libc6-arm64-cross \
    crossbuild-essential-arm64 \
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


#####################################################
# Paho MQTT C — cross-compiled from source for arm64
# libpaho-mqtt-c-dev:arm64 is not available as a cross-arch apt package.
# Not used by the default config (aws_green_grass) but available if
# EHS_MQTT_SUPPORT=paho is selected.
#####################################################
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

#####################################################
# Other Debian packages directly from vendors
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

#Add the hailo headers and library for linking

RUN pwd
ARG HAILO_SDK_VERSION=4.22.0
#TODO -We need to be ablt to copy tis from the ert-contrib middleware as a prebuild step?
# from ./ert-contrib-middleware/contrib/hailort/hailort_4.22.0_arm64.deb
# COPY ../ert-contrib-middleware/contrib/hailort/hailort_${HAILO_SDK_VERSION}_arm64.deb /tmp/hailort_${HAILO_SDK_VERSION}_arm64.deb
COPY temp/hailort_${HAILO_SDK_VERSION}_arm64.deb /tmp/hailort_${HAILO_SDK_VERSION}_arm64.deb
#COPY ../target/platform/linux_arm64_lvgl_raspberrypi_demo/hailort_${HAILO_SDK_VERSION}_arm64.deb /tmp/hailort_${HAILO_SDK_VERSION}_arm64.deb
RUN dpkg -i /tmp/hailort_${HAILO_SDK_VERSION}_arm64.deb ; rm /tmp/hailort_*_arm64.deb

# We awnt to sudo to experiment with a few things:
#RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo

######################################################################################
# Boiler plate fr the build system

RUN adduser docker && mkdir -p /etc/sudoers.d \
    && echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker \
    && chmod 0440 /etc/sudoers.d/docker
USER docker
WORKDIR /home/docker
RUN whoami
RUN sudo whoami
