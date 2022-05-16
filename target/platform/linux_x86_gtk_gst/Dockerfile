From debian:9
RUN echo "Successfully entered and fetched debian:9"
RUN apt-get update
RUN echo "installing git"
RUN apt-get install -y git
RUN apt install -y curl
RUN curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | bash
RUN apt-get -y install openssh-client && apt-get install -y make && apt-get -y install git-lfs
RUN apt-get install -y build-essential libcurl4-openssl-dev libarchive-dev libexpat1-dev libidn2-0-dev  zlib1g-dev
#ENTRYPOINT ./configure



RUN echo "cloning to build support repos"
RUN git lfs clone https://github.com/inxware/ert-build-support.git
WORKDIR "/ert-build-support"
RUN git lfs pull
RUN echo "build support successfully done"

WORKDIR "/"
RUN echo "cloning to build contrib middleware"
RUN git lfs clone https://github.com/inxware/ert-contrib-middleware.git
WORKDIR "/ert-contrib-middleware"
RUN git lfs pull
RUN echo "middleware contri successfully done"
RUN ls -l

WORKDIR "/"
RUN echo "cloning to build ert-components"
RUN git lfs clone https://github.com/inxware/ert-components.git
RUN echo "ert components successfully done"

WORKDIR "/ert-components"
ENTRYPOINT ./configure
##ENTRYPOINT #exec #['/bin/sh'] #["./configure"] CMD["linux_x86_gts_gst"]
RUN make prepdeps
RUN make
#RUN make targetenv
#RUN make targetenv_deb
