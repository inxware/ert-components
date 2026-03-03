FROM debian:13
RUN echo "Successfully entered and fetched debian:13"
RUN apt update
RUN echo "installing build essential and git"
#above is used while debugging and needs the bits at the end of ths file too
# Add raspberry pi repos
RUN dpkg --add-architecture arm64
RUN apt update --fix-missing
RUN apt upgrade -y
RUN apt install -y build-essential cmake libtinfo6 git git-lfs \
  clang llvm binutils-aarch64-linux-gnu libc6-arm64-cross crossbuild-essential-arm64 \
  sudo wget
RUN ln -s /usr/lib/aarch64-linux-gnu /lib/aarch64-linux-gnu
RUN apt install -y libarchive-dev:arm64 libcurl4-openssl-dev:arm64 \
	gcc-multilib g++-multilib \
  	libc6-dev:arm64 libz-dev:arm64 libidn2-dev:arm64 libexpat1-dev:arm64

RUN apt install -y \
 libcurl4-openssl-dev:arm64 \
 libarchive-dev:arm64 \
 libexpat1-dev:arm64 \
 libexpat-dev:arm64 \
 libidn2-0-dev:arm64  \
 zlib1g-dev:arm64 \
 libexpat-dev:arm64 \
 libxml2-dev:arm64 \
 libgtk-3-dev:arm64 \
 libsdl2-dev:arm64 \
 libgstreamer1.0-dev:arm64 \
 libgstreamer-plugins-base1.0-dev:arm64 \
 libopencv-dev:arm64 \
 libcamera-dev:arm64 \
 g++-aarch64-linux-gnu gcc-aarch64-linux-gnu

#	libgtk2.0-dev:arm64 \

RUN echo "deb [ trusted=yes ] http://archive.raspberrypi.com/debian/ trixie main" | tee /etc/apt/sources.list.d/raspi.list

RUN apt update
RUN apt install -y hailort

# We awnt to sudo to experiment with a few things:
#RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo
RUN adduser docker && mkdir -p /etc/sudoers.d \
        && echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker \
        && chmod 0440 /etc/sudoers.d/docker
USER docker
WORKDIR /home/docker
RUN whoami
RUN sudo whoami
