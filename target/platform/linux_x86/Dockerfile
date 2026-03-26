# inxware/ubuntu18-build-essential
FROM ubuntu:18.04
#Note: We don't bother installing git-lfs on older ubuntu docker images as this is only needed in Docker environments on github.
#      We don't build this on github, but can be built locally using the make prepdeps.
RUN apt-get update && \
    apt-get install -y build-essential git

