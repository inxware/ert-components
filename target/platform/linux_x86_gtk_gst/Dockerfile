From debian:9
RUN echo "Successfully entered and fetched debian:9"
RUN apt-get update
RUN echo "installing git"
RUN apt-get install -y git

RUN mkdir /root/.ssh/
ARG SSH_PRIVATE_KEY
RUN echo "${SSH_PRIVATE_KEY}" > ~/.ssh/id_ed25519
RUN chmod 600 ~/.ssh/id_ed25519
RUN ssh-keyscan github.com >> ~/.ssh/known_hosts

RUN echo "cloning to build support repos"
RUN git clone git@github.com:inxware/ert-build-support.git

RUN echo "cloning to build contrib middleware"
RUN git clone git@github.com:inxware/ert-contrib-middleware.git

RUN git clone git@github.com:inxware/ert-components.git
RUN cd ert-components
COPY configure /configure
ENTRYPOINT ["./configure"] CMD["linux_x86_gts_gst"]
RUN make
RUN make targetenv
RUN make targetenv_deb
