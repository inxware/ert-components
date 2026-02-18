FROM debian:12
RUN echo "Successfully entered and fetched debian:12"
RUN apt-get update
RUN echo "installing build essential and git"
# above is used while debugging and needs the bits at the end of ths file too

RUN dpkg --add-architecture arm64
RUN apt-get update -y --fix-missing

# Core build tools
RUN apt-get install -y \
    build-essential \
    cmake \
    libtinfo5 \
    git \
    git-lfs \
    clang \
    llvm \
    binutils-aarch64-linux-gnu \
    libc6-arm64-cross \
    crossbuild-essential-arm64 \
    sudo

# Generic tools and dependencies
RUN apt-get install -y \
    libarchive-dev:arm64 \
    libcurl4-openssl-dev:arm64 \
    gcc-multilib \
    g++-multilib \
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
    libgstreamer-plugins-base1.0-dev:arm64

# Qt 5 dependencies
#Removing these now as supporting both can be confusing to builds
#RUN apt-get install -y \
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

# We want to sudo to experiment with a few things:
#RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo

RUN adduser docker && mkdir -p /etc/sudoers.d \
    && echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker \
    && chmod 0440 /etc/sudoers.d/docker
USER docker
WORKDIR /home/docker
RUN whoami
RUN sudo whoami
