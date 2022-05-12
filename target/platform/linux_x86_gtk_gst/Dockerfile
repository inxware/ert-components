From debian:9
RUN echo "Successfully entered and fetched debian:9"
RUN apt-get update
RUN echo "installing git"
RUN apt-get install -y git
RUN apt-get -y install openssh-client


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

RUN echo "cloning to build support repos"
RUN git clone https://github.com/inxware/ert-build-support.git
RUN echo "build support successfully done"
RUN echo "cloning to build contrib middleware"
RUN git clone https://github.com/inxware/ert-contrib-middleware.git
RUN echo "middleware contri successfully done"
RUN git clone https://github.com/inxware/ert-components.git
RUN echo "ert components successfully done"
WORKDIR "/ert-components"
RUN ls -l
#COPY configure /configure
ENTRYPOINT ["./configure"] CMD["linux_x86_gts_gst"]
RUN make
RUN make targetenv
RUN make targetenv_deb
