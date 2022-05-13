From debian:9
RUN echo "Successfully entered and fetched debian:9"
RUN apt-get update
RUN echo "installing git"
RUN apt-get install -y git
RUN apt install -y curl
RUN curl -s https://packagecloud.io/install/repositories/github/git-lfs/script.deb.sh | bash
RUN apt-get -y install openssh-client && apt-get install -y make && apt-get -y install git-lfs


#now using public repo @pierre make it public for now
#RUN chmod 666 /dev/tty
#RUN mkdir -p /.ssh && chmod 700 /.ssh
#RUN ssh-keygen -q -t rsa -N '' -f /.ssh/id_rsa
##COPY /root/.ssh/id_rsa > /root/.ssh/id_rsa
#RUN chmod 600 /.ssh/id_rsa
#RUN touch /.ssh/known_hosts
#RUN ssh-keyscan -t rsa -H gitlab.com >> /.ssh/known_hosts
##RUN ssh-keyscan github.com >> /.ssh/known_hosts
#RUN touch /.ssh/config && chmod 666 /.ssh/config
#RUN echo "Host github.com\n\tStrictHostKeyChecking no\n" >> /.ssh/config
#RUN ssh -v git@github.com

#RUN mkdir /root/.ssh/
#ARG SSH_PRIVATE_KEY
#RUN echo "${SSH_PRIVATE_KEY}" > ~/.ssh/id_ed25519
#RUN chmod 600 ~/.ssh/id_ed25519
#RUN ssh-keyscan github.com >> ~/.ssh/known_hosts

RUN cd home && mkdir inxware
WORKDIR inxware

RUN echo "cloning to build support repos"
RUN git lfs clone https://github.com/inxware/ert-build-support.git
WORKDIR "inxware/ert-build-support/"
RUN git lfs pull
RUN echo "build support successfully done"

WORKDIR "/home/inxware/"
RUN echo "cloning to build contrib middleware"
RUN git lfs clone https://github.com/inxware/ert-contrib-middleware.git
WORKDIR "/home/inxware/ert-contrib-middleware"
RUN git lfs pull
RUN echo "middleware contri successfully done"
RUN ls -l

WORKDIR "/home/inxware/"
RUN echo "cloning to build ert-components"
RUN git lfs clone https://github.com/inxware/ert-components.git
RUN echo "ert components successfully done"

WORKDIR "/home/inxware/ert-components"
RUN ls -l
#COPY configure /configure
ENV CC=gcc-11
ENV CXX=g++-11
ENTRYPOINT ["./configure"] CMD["linux_x86_gts_gst"]
RUN make
RUN make targetenv
RUN make targetenv_deb
