# inxware/inx-debian11-clang-arm-gtk
FROM debian:11
RUN echo "Successfully entered and fetched debian:11"
RUN apt-get update
RUN echo "installing build essential and git"
#above is used while debugging and needs the bits at the end of ths file too
RUN dpkg --add-architecture arm64
RUN apt-get update --fix-missing
RUN apt-get install -y build-essential cmake libtinfo5 git git-lfs \
  clang llvm binutils-aarch64-linux-gnu libc6-arm64-cross crossbuild-essential-arm64 \
  sudo
RUN apt-get install -y libarchive-dev:arm64 libcurl4-openssl-dev:arm64 \
	gcc-multilib g++-multilib \
  	libc6-dev:arm64 libz-dev:arm64 libidn2-dev:arm64 libexpat1-dev:arm64

RUN apt-get install -y \
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
	libgstreamer-plugins-base1.0-dev:arm64

#	libgtk2.0-dev:arm64 \

# We awnt to sudo to experiment with a few things:
#RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo
RUN adduser docker && mkdir -p /etc/sudoers.d \
        && echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker \
        && chmod 0440 /etc/sudoers.d/docker
USER docker
WORKDIR /home/docker
RUN whoami
RUN sudo whoami
