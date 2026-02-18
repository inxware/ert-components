FROM debian:12
RUN echo "Successfully entered and fetched debian:12"
RUN apt-get update
RUN echo "Installing build essential and git"

# Core build tools
RUN apt-get install -y \
    build-essential \
    cmake \
    git \
    git-lfs \
    clang \
    llvm \
    lld \
    sudo

# Generic tools and dependencies
RUN apt-get install -y \
    libarchive-dev \
    libcurl4-openssl-dev \
    libc6-dev \
    zlib1g-dev \
    libexpat-dev \
    libexpat1-dev \
    libidn2-dev \
    libxml2-dev

# GTK2 dependencies (used by Qt platform for some image/graphics support)
RUN apt-get install -y \
    libgtk2.0-dev \
    libgdk-pixbuf2.0-dev \
    libcairo2-dev \
    libpango1.0-dev \
    libatk1.0-dev \
    libglib2.0-dev

# Qt 5 dependencies
RUN apt-get install -y \
    qtbase5-dev \
    qtdeclarative5-dev \
    qtbase5-dev-tools \
    qtdeclarative5-dev-tools

# X11 and display dependencies
RUN apt-get install -y \
    libx11-dev \
    libxext-dev \
    libxrender-dev \
    libxcomposite-dev \
    libxfixes-dev \
    libfontconfig1-dev \
    libfreetype6-dev

# Claude guesswork
# Create docker user with sudo access
#RUN adduser docker && mkdir -p /etc/sudoers.d \
#    && echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker \
#    && chmod 0440 /etc/sudoers.d/docker
#USER docker
#WORKDIR /home/docker
#RUN whoami
#RUN sudo whoami
