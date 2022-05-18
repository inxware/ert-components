From debian:9
RUN echo "Successfully entered and fetched debian:9"
RUN apt-get update
RUN apt-get install -y build-essential libcurl4-openssl-dev libssl-dev libarchive-dev libexpat1-dev libexpat-dev libidn2-0-dev  zlib1g-dev tree git unzip wget make gettext
RUN cd /usr/src/
RUN wget https://github.com/git/git/archive/v2.18.0.tar.gz -O git.tar.gz
RUN tar -xf git.tar.gz
WORKDIR git-2.18.0
RUN make prefix=/usr/local all
RUN make prefix=/usr/local install
RUN git --version 
RUN apt install -y curl
RUN curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | bash
RUN apt-get install git-lfs
