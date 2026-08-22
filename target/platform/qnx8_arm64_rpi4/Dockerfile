# Dockerfile for QNX SDP 8.0 ARM64 (AArch64) build environment
# Target: QNX Neutrino 8.0 on Raspberry Pi 4 (Cortex-A72)
#
# !! PRIVATE IMAGE — DO NOT PUSH TO A PUBLIC REGISTRY !!
# This image installs the QNX SDP which is licensed software.
# Store only in a private container registry (e.g. Azure Container Registry).
#
# -----------------------------------------------------------------------
# PREREQUISITE: A myQNX account with QNX Everywhere or a commercial licence.
#   Register / log in at: https://www.qnx.com/account/
#
# Method A — QNX Software Center installer (default, recommended):
#   The Docker build downloads QNX SDP automatically.  You only need to
#   supply the QNX Software Center CLI installer once and your myQNX credentials.
#   Credentials are passed as BuildKit secrets — they are NEVER baked into
#   the image or visible in docker history.
#
#   1. Download the QNX Software Center for Linux from your myQNX account:
#        https://www.qnx.com/account/  →  Downloads  →  QNX Software Center
#        File: qnxsoftwarecenter-linux64-x.x.x.run  (or similar)
#   2. Place it alongside this Dockerfile:
#        cp ~/Downloads/qnxsoftwarecenter-linux64-*.run \
#             target/platform/qnx8_arm64_rpi4/qnxsoftwarecenter.run
#        (path is relative to repo root — that is where Docker's COPY looks)
#   3. Build via make — the build_docker_pre.sh hook prompts for credentials
#      interactively if they are not already set as environment variables:
#        ./configure qnx8_arm64_rpi4
#        make build_docker_local
#
# Method B — offline pre-packaged tarball:
#   Use this when the build host has no internet access, or when you want
#   a fully reproducible image that does not reach out to qnx.com.
#   Requires exporting an already-installed SDP:
#        tar -czf qnx800.tar.gz -C /opt qnx800
#        cp qnx800.tar.gz target/platform/qnx8_arm64_rpi4/
#   Comment out the Method A block below and uncomment Method B.
# -----------------------------------------------------------------------

FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

#######################################################
# Core build tools
#######################################################
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    git \
    git-lfs \
    wget \
    curl \
    less \
    openjdk-17-jre-headless \
    python3 \
    python3-pip \
    sudo \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

#######################################################
# QNX SDP 8.0 — Method A: QNX Software Center CLI install (default)
#
# The installer file is COPY'd from the build context (one-time download
# by the developer from myQNX).  Credentials are passed as --build-arg
# values by build_docker_pre.sh / build_docker_local.sh.
#
# !! SECURITY NOTE !!
# ARG values are visible in `docker history`.  This image is a PRIVATE
# local-only build — never push it to a public registry.
# If your organisation requires secret-safe builds, install Docker buildx
# and switch to the --secret / RUN --mount=type=secret pattern instead.
#######################################################
# Baseline package ID: com.qnx.qnx800  (verified via -offline -list)
ARG QNX_SDP_BASELINE=com.qnx.qnx800

COPY target/platform/qnx8_arm64_rpi4/qnxsoftwarecenter.run /tmp/qnxsoftwarecenter.run

# Step 1: Install the QNX Software Center application itself.
# Cached after first successful run — credentials not needed here.
# The .run file installs the qnxsoftwarecenter_clt tool only;
# the SDP download happens in Step 2 below.
#
# Use printf rather than 'yes' so we answer only the EULA prompt ("y")
# and let the installer use its default directory for everything else.
RUN chmod +x /tmp/qnxsoftwarecenter.run && \
    printf 'y\n' | /tmp/qnxsoftwarecenter.run --nox11 && \
    rm /tmp/qnxsoftwarecenter.run

# Credentials declared here so Step 1 cache is unaffected by credential changes.
ARG QNX_USER
ARG QNX_PASS
ARG QNX_LICENSE

# Step 2a: Mirror SDP 8.0 metadata.
RUN /root/qnx/qnxsoftwarecenter/qnxsoftwarecenter_clt \
      -myqnx.user    "$QNX_USER" \
      -myqnx.password "$QNX_PASS" \
      -mirrorBaseline "${QNX_SDP_BASELINE}"

# Step 2b: Install QNX SDP.
RUN LICENSE_ARG="" && \
    [ -n "$QNX_LICENSE" ] && LICENSE_ARG="-addLicenseKey $QNX_LICENSE" ; \
    /root/qnx/qnxsoftwarecenter/qnxsoftwarecenter_clt \
      -myqnx.user    "$QNX_USER" \
      -myqnx.password "$QNX_PASS" \
      $LICENSE_ARG \
      -installBaseline "${QNX_SDP_BASELINE}" \
      -destination /opt/qnx800

# --- Method B: offline pre-packaged tarball ---
# Comment out the ARG/COPY/RUN block above and uncomment below.
# Place qnx800.tar.gz (tar -czf qnx800.tar.gz -C /opt qnx800) alongside
# this Dockerfile before running docker build.  No credentials needed.
#
# COPY target/platform/qnx8_arm64_rpi4/qnx800.tar.gz /tmp/qnx800.tar.gz
# RUN tar -xzf /tmp/qnx800.tar.gz -C /opt && rm /tmp/qnx800.tar.gz

#######################################################
# QNX SDP environment variables
# The cross-compiler uses QNX_HOST and QNX_TARGET to locate
# its sysroot at $QNX_TARGET/aarch64le — no explicit --sysroot needed.
#######################################################
ENV QNX_HOST=/opt/qnx800/host/linux/x86_64
ENV QNX_TARGET=/opt/qnx800/target/qnx
ENV PATH="${QNX_HOST}/usr/bin:${PATH}"

# Sanity-check: verify the AArch64 cross-compiler is reachable
RUN aarch64-unknown-nto-qnx8.0.0-gcc --version

######################################################################################
# Boilerplate for the inxware build system (matches other inxware Docker images)

RUN adduser docker && mkdir -p /etc/sudoers.d \
    && echo "docker ALL=(ALL) NOPASSWD: ALL" > /etc/sudoers.d/docker \
    && chmod 0440 /etc/sudoers.d/docker
USER docker
WORKDIR /home/docker
RUN whoami
RUN sudo whoami
