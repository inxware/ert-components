FROM debian:12
RUN echo "Successfully entered and fetched debian:12"
RUN apt update
RUN echo "installing build essential and git"
# above is used while debugging and needs the bits at the end of ths file too

RUN dpkg --add-architecture arm64
RUN apt update -y --fix-missing
# Not sure always want to do this, but...
#RUN apt upgrade -y

#######################################################
# Core build tools
#######################################################

# NOTE: crossbuild-essential-arm64 was removed here (2026-08).  It pulls
# gcc-aarch64-linux-gnu / g++-12-aarch64-linux-gnu, which pin
# gcc-12-aarch64-linux-gnu (= 12.2.0-14cross1).  A bookworm point release moved the
# arm64 runtime to 12.2.0-14+deb12u1 but the amd64 *cross* packages were never
# rebuilt, so that metapackage is now uninstallable and made the whole image
# unbuildable ("held broken packages").
# We compile with clang --target=aarch64-linux-gnu (see target/os-arch/linux-arm64/
# toolchain.mk), not the GCC cross driver, so all clang needs is the aarch64
# assembler/linker from binutils plus the arm64 crt/libgcc/libstdc++ bits, which
# come from the multiarch :arm64 dev packages requested explicitly below.
RUN apt install -y \
    build-essential \
    cmake \
    libtinfo5 \
    git \
    git-lfs \
    clang \
    llvm \
    lld \
    gcc-multilib \
    g++-multilib \
    binutils-aarch64-linux-gnu \
    libc6-arm64-cross \
    libgcc-12-dev:arm64 \
    libstdc++-12-dev:arm64 \
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

# Qt 5 dependencies
#Removing these now as supporting both can be confusing to builds
#RUN apt install -y \
#    qtbase5-dev:arm64 \
#    qtdeclarative5-dev:arm64 \
#    qtbase5-dev-tools \
#    qtdeclarative5-dev-tools
#QT 6 Dependencies
RUN apt install -y \
     qt6-base-dev:arm64 \
     qt6-base-dev-tools \
     qt6-declarative-dev:arm64 \
     qt6-declarative-dev-tools 
#     libqt6quickwidgets6 \


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

#Add the hailo headers and library for linking
#
# OPTIONAL.  This target does not enable EHS_ML_HARDWARE_ACCELERATION=hailo
# (see config.mk - it, EHS_ML_SUPPORT and EHS_MV_SUPPORT=opencv are all commented
# out), and the .deb is not committed anywhere in the repo.  An unconditional COPY
# of a file that isn't in the build context makes `make build_docker_local` fail
# outright, which is what stopped this image being rebuilt and left it without
# wiringPi/pigpio.  So it is now off by default:
#
#   make build_docker_local                          -> no HailoRT (default)
#   ERT_DOCKER_BUILD_ARGS=INSTALL_HAILO=1 make build_docker_local
#                                                    -> installs HailoRT
#
# For the ON case, drop hailort_${HAILO_SDK_VERSION}_arm64.deb into <repo-root>/temp/
# first.  COPY itself cannot be made conditional, so temp/ is committed with a
# .gitkeep and the whole directory is copied; the conditional lives in the RUN.
#TODO -We need to be ablt to copy tis from the ert-contrib middleware as a prebuild step?
# from ./ert-contrib-middleware/contrib/hailort/hailort_4.22.0_arm64.deb
ARG HAILO_SDK_VERSION=4.22.0
ARG INSTALL_HAILO=0
COPY temp/ /tmp/hailo/
RUN if [ "${INSTALL_HAILO}" = "1" ]; then \
        dpkg -i /tmp/hailo/hailort_${HAILO_SDK_VERSION}_arm64.deb ; \
    else \
        echo "Skipping HailoRT install (INSTALL_HAILO=${INSTALL_HAILO})" ; \
    fi ; \
    rm -rf /tmp/hailo

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
