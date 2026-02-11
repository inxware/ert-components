From debian:9
#NOTE - This does not install git-lfs as this does not work properly on Debian 9. 
# Builds cn be done locally using the make prepdeps on a host linux machine that does support git-lfs instead.
RUN echo "Successfully entered and fetched debian:9"
RUN apt-get update 
RUN echo "installing build essential and git"
RUN apt-get install -y build-essential libcurl4-openssl-dev libarchive-dev libexpat1-dev libexpat-dev libidn2-0-dev  zlib1g-dev libxml2-dev git got-lfs
RUN apt-get install -y linux-image-4.19-amd64 libgtk2.0-dev libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libpng-dev libidn2-0-dev libsndfile-dev libasound2-dev liblo-dev

