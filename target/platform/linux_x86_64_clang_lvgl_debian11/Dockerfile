# inxware/inx-debian11-clang11-lvgl
From debian:11
RUN echo "Successfully entered and fetched debian:11"
RUN apt update
RUN echo "installing build essential and git"
RUN apt install -y build-essential libtinfo5 clang llvm lld \
	libcurl4-openssl-dev libarchive-dev libexpat1-dev libexpat-dev libidn2-0-dev  zlib1g-dev libxml2-dev \
	git git-lfs 
RUN apt install -y libgtk2.0-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libpng-dev libidn2-0-dev libsndfile-dev libasound2-dev liblo-dev libsdl2-dev

RUN apt install -y libopencv-dev
