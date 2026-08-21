# inxware/inx-debian12-clang14-lvgl-axelera
#
# x86_64 build image: Debian 12 + Clang 14 + LVGL deps + Axelera Voyager SDK.
# Extends inxware/inx-debian12-clang14-lvgl-gg (the Debian 12 baseline image
# defined in target/platform/linux_x86_64_clang_lvgl_gg_debian12/Dockerfile).
#
# Axelera APT repository (public, no auth):
#   https://software.axelera.ai/artifactory/axelera-apt-source/
# GPG key:
#   https://software.axelera.ai/artifactory/api/security/keypair/axelera/public
# Distribution string: ubuntu24 main
#   (the same string is used for both Debian 12 and 13 — the runtime is
#    dlopen'd at use-time and the build-time headers are libc-version
#    tolerant; matches the existing arm64 raspi Debian 12 + Axelera image)
#
# Packages added on top of the base image:
#   axelera-runtime-<VER>  — SDK headers + shared libs (headers needed at build time)
#   axelera-device-<VER>   — device firmware blobs
# NOT installed (only needed on target hardware at runtime):
#   metis-dkms             — PCIe kernel driver

FROM inxware/inx-debian12-clang14-lvgl-gg

# The base image already ran adduser/sudo and switched to USER docker.
# Switch back to root for the package installation.
USER root

#####################################################
# Axelera Voyager SDK (amd64 — headers needed at build time)
#####################################################

ARG AXELERA_SDK_VERSION=1.6.0

RUN mkdir -p /etc/apt/keyrings \
    && curl -fsSL "https://software.axelera.ai/artifactory/api/security/keypair/axelera/public" \
       | gpg --dearmor \
       | tee /etc/apt/keyrings/axelera.gpg > /dev/null \
    && echo "deb [signed-by=/etc/apt/keyrings/axelera.gpg] https://software.axelera.ai/artifactory/axelera-apt-source/ ubuntu24 main" \
       | tee /etc/apt/sources.list.d/axelera.list > /dev/null \
    && apt-get update \
    && apt-get install -y --no-install-recommends \
       axelera-runtime-${AXELERA_SDK_VERSION} \
       axelera-device-${AXELERA_SDK_VERSION} \
    && rm -rf /var/lib/apt/lists/*

# Restore the unprivileged docker user from the base image.
USER docker
WORKDIR /home/docker
