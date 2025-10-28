FROM debian:11
RUN echo "Successfully entered and fetched debian:11"
RUN apt-get update 
RUN apt-get update --fix-missing
RUN echo "installing build essential and git"
RUN apt-get install -y build-essential libtinfo5 git git-lfs \
  clang llvm binutils-aarch64-linux-gnu libc6-arm64-cross crossbuild-essential-arm64 \
  sudo
#above is used while debugging and needs the bits at the end of ths file too
RUN dpkg --add-architecture arm64
RUN apt-get update
RUN apt-get install -y libarchive-dev:arm64 libcurl4-openssl-dev:arm64 libxml2-dev:arm64\
  libc6-dev:arm64 libz-dev:arm64 libidn2-dev:arm64 libexpat1-dev:arm64 \
  libsdl2-dev:arm64

#  The following may need to be added to this Docker file!!
#    2  dpkg --add-architecture arm64
#    3  apt-get install libgtk3-dev:arm64
#    4  apt-cache search libgtk3-dev:arm64
#    5  apt-cache search gtk3-dev
#    6  apt-cache search gtk-dev
#    7  apt-get install libcurl-dev:arm64
#    8  aptitude search '~n apache2-dev'
#    9  apt-cache search '~n apache2-dev'
#   10  cat /etc/apt/sources.list
#   11  apt-get install libarchive-dev:aarch64
#   12  apt-get install libarchive-dev:accrch64
#   13  apt-get update
#   14  apt-get install libarchive-dev:aarch64
#   15  apt-get install libarchive-dev:arm64
#   16  su docker
#   17  apt-cache search  lib*curl*
#   18  apt-get install libcurl4-openssl-dev:arm64
#   19  su docker
#   20  history


# We may want to sudo to experiment with a few things:
#RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo
RUN adduser docker && mkdir -p /etc/sudoers.d \
        && echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker \
        && chmod 0440 /etc/sudoers.d/docker
USER docker
WORKDIR /home/docker
RUN whoami
RUN sudo whoami

# You might also need to install libc6-arm64-cross, libc6-dev-arm64-cross, crossbuild-essential-arm64.
# libcurl4-openssl-dev libarchive-dev libexpat1-dev libexpat-dev libidn2-0-dev  zlib1g-dev git

