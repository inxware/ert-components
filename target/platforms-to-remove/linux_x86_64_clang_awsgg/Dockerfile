From debian:10
RUN echo "Successfully entered and fetched debian:10"
RUN apt-get update
RUN echo "installing build essential and git git-lfs"
RUN apt-get install -y \
	build-essential automake cmake libtool libtinfo5 clang llvm \
	libcurl4-openssl-dev libarchive-dev libexpat1-dev libexpat-dev libidn2-0-dev  zlib1g-dev libxml2-dev \
	git git-lfs 
