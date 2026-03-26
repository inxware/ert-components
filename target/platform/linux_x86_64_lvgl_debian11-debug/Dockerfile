# inxware/inx-debian11-lvgl
From debian:11
RUN echo "Successfully entered and fetched debian:11"
RUN apt update
RUN echo "installing build essential and git"
RUN apt install -y build-essential libtinfo5 clang llvm \
	libcurl4-openssl-dev libarchive-dev libexpat1-dev libexpat-dev libidn2-0-dev  zlib1g-dev libxml2-dev \
	git git-lfs 
RUN apt install -y libgtk2.0-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libpng-dev libidn2-0-dev libsndfile-dev libasound2-dev liblo-dev libsdl2-dev

#    libcurl4-openssl-dev:arm64 \
#    libarchive-dev:arm64 \
#    libc6-dev:arm64 \
#    libz-dev:arm64 \
#    zlib1g-dev:arm64 \
#    libexpat-dev:arm64 \
#    libexpat-dev:arm64 \
#    libexpat1-dev:arm64 \
#    libidn2-dev:arm64 \
#    libidn2-0-dev:arm64  \
#    libxml2-dev:arm64 \
#    libgtk-3-dev:arm64 \
 #   libsdl2-dev:arm64 \
  #  libgstreamer1.0-dev:arm64 \
  #  libgstreamer-plugins-base1.0-dev:arm64 \

RUN apt install -y \
    libopencv-dev 
#    libcamera-dev 
