FROM debian:13
RUN echo "Successfully entered and fetched debian:13 for x86"
RUN apt update
RUN echo "Installing build essential and git"

# Core build tools
RUN apt install -y \
    build-essential \
    cmake \
    git \
    git-lfs \
    clang \
    llvm \
    lld \
    sudo

# Generic tools and dependencies
RUN apt install -y \
    libarchive-dev \
    libcurl4-openssl-dev \
    libc6-dev \
    zlib1g-dev \
    libexpat-dev \
    libexpat1-dev \
    libidn2-dev \
    libxml2-dev

RUN apt install -y \
    libgtk2.0-dev \
    libgstreamer1.0-dev \
    libgstreamer-plugins-base1.0-dev \
    libpng-dev libidn2-0-dev \
    libsndfile-dev \
    libasound2-dev \
    liblo-dev \
    libsdl2-dev

# See comment on QT 5 or 6 below: GTK2 dependencies (used by Qt platform for some image/graphics support)
#RUN apt install -y \
#    libgtk2.0-dev \
#    libgdk-pixbuf2.0-dev \
#    libcairo2-dev \
#    libpango1.0-dev \
#    libatk1.0-dev \
#    libglib2.0-dev

# Qt 4 - switch to 6!!!  dependencies
#RUN apt install -y \
#    qtbase5-dev \
#    qtdeclarative5-dev \
#    qtbase5-dev-tools \
#    qtdeclarative5-dev-tools

# Do W still need this? X11 and display dependencies
#RUN apt-get install -y \
#    libx11-dev \
#    libxext-dev \
#    libxrender-dev \
#    libxcomposite-dev \
#    libxfixes-dev \
#    libfontconfig1-dev \
#    libfreetype6-dev

RUN apt install -y libopencv-dev

# Create docker user with sudo access
#RUN adduser docker && mkdir -p /etc/sudoers.d \
#    && echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker \
#    && chmod 0440 /etc/sudoers.d/docker
#USER docker
#WORKDIR /home/docker
#RUN whoami
#RUN sudo whoami
