From debian:9
RUN echo "Successfully entered and fetched debian:9"
#RUN apt-get update --fix-missing
RUN apt-get -y update
RUN echo "installing build essential and git"
RUN apt-get install -y build-essential libtinfo5 git git-lfs \
             clang llvm binutils-arm-linux-gnueabi libc6-armhf-cross crossbuild-essential-armhf \
             sudo libidn2-0-dev 
#libcurl4-openssl-dev libarchive-dev libexpat1-dev libexpat-dev libidn2-0-dev  zlib1g-dev git

RUN dpkg --add-architecture armhf
#RUN apt-get -y update --fix-missing
RUN apt-get -y update
RUN apt-get install -y libarchive-dev:armhf libcurl4-openssl-dev:armhf \
  libc6-dev:armhf libz-dev:armhf libidn11-dev:armhf libexpat1-dev:armhf libxml2-dev:armhf

RUN apt-get install -y \
	zlib1g-dev:armhf \
	libxml2-dev:armhf \
	libgtk-3-dev:armhf \
	libgstreamer1.0-dev:armhf \
	libgstreamer-plugins-base1.0-dev:armhf




# We may want to sudo to experiment with a few things:
#RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo
RUN adduser docker && mkdir -p /etc/sudoers.d \
        && echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker \
        && chmod 0440 /etc/sudoers.d/docker
USER docker
WORKDIR /home/docker
RUN whoami
RUN sudo whoami
