From debian:9
RUN echo "Successfully entered and fetched debian:9"
RUN apt-get update
RUN echo "installing git"
RUN apt-get install -y git

#RUN mkdir -m 700 /root/.ssh; \
#  touch -m 600 /root/.ssh/known_hosts; \
#  ssh-keyscan github.com > /root/.ssh/known_hosts
RUN mkdir -p /root/.ssh
RUN cp /var/my-app/id_rsa /root/.ssh/id_rsa
RUN chmod 600 /root/.ssh/id_rsa
RUN ssh-keyscan github.com >> /root/.ssh/known_hosts

RUN echo "Host github.com\n\tStrictHostKeyChecking no\n" >> /root/.ssh/config
RUN ssh -v git@github.com

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
#https://{zuhaibasgher}:{just4fun1}@github.com/{zuhaibasgher}/project.git
RUN echo "ert components successfully done"
RUN cd ert-components
COPY configure /configure
ENTRYPOINT ["./configure"] CMD["linux_x86_gts_gst"]
RUN make
RUN make targetenv
RUN make targetenv_deb
