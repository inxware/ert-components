From debian:9
RUN echo "Successfully entered and fetched debian:9"
RUN apt-get update
RUN echo "installing git"
RUN apt-get install -y git

RUN mkdir /root/.ssh/

# Copy over private key, and set permissions
# Warning! Anyone who gets their hands on this image will be able
# to retrieve this private key file from the corresponding image layer
ADD id_rsa /root/.ssh/id_rsa

# Create known_hosts
RUN touch /root/.ssh/known_hosts
# Add bitbuckets key
RUN ssh-keyscan bitbucket.org >> /root/.ssh/known_hosts

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
