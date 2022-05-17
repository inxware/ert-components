From debian:9
RUN echo "Successfully entered and fetched debian:9"
RUN apt-get update
RUN echo "installing git"
RUN apt-get install -y build-essential libcurl4-openssl-dev libarchive-dev libexpat1-dev libexpat-dev libidn2-0-dev  zlib1g-dev git unzip wget
RUN cd /usr/src/
RUN wget https://github.com/git/git/archive/v2.18.0.tar.gz -O git.tar.gz
RUN sudo tar -xf git.tar.gz
RUN cd git-*
RUN make prefix=/usr/local all
RUN make prefix=/usr/local install
RUN git --version 
