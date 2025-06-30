From debian:11
RUN echo "Successfully entered and fetched debian:11"
RUN apt-get update --fix-missing
RUN echo "installing build essential and git (not git-lfs for debian 10)"
RUN apt-get install -y \
	build-essential automake cmake libtool libtinfo5 clang llvm lld\
	libcurl4-openssl-dev libarchive-dev libexpat1-dev libexpat-dev libidn2-0-dev  zlib1g-dev libxml2-dev \
	git sudo

RUN apt-get install -y \
	libcurl4-openssl-dev \
	libarchive-dev \
	libexpat1-dev \
	libexpat-dev \
	libidn2-0-dev  \
	zlib1g-dev \
	libxml2-dev \
	libgtk2.0-dev \
	libgstreamer1.0-dev \
	libgstreamer-plugins-base1.0-dev


# We may want to sudo to experiment with a few things:
#RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo
RUN adduser docker && mkdir -p /etc/sudoers.d \
        && echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker \
        && chmod 0440 /etc/sudoers.d/docker
USER docker
WORKDIR /home/docker
RUN whoami
RUN sudo whoami
