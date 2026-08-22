# inxware/ubuntu24.04-build-essential-mingw
#
# Build environment for the 64-bit Windows targets. The Ubuntu mingw-w64
# package supplies both cross toolchains, so this one image covers
# x86_64-w64-mingw32 (used here) and i686-w64-mingw32.
#
# The 32-bit targets deliberately stay on inxware/ubuntu22.04-build-essential-mingw
# (mingw gcc 10.3, NSIS 3.0.4.1). They link contrib libraries fetched from MSYS2
# in 2022/2023 that cannot be re-fetched -- MSYS2 has retired its 32-bit
# environment -- so that toolchain is frozen against the libraries it was proven
# with. New Windows work targets 64-bit, where MSYS2 remains maintained.
#
#   32-bit  ubuntu22.04 base   mingw gcc 10.3.0   NSIS 3.0.4.1 (2019 CVS)
#   64-bit  ubuntu24.04 base   mingw gcc 13.2.0   NSIS 3.09    (this file)
#
# Self-contained by choice: the 32-bit Dockerfile declares FROM inxware/ubuntu22.04,
# an image that is not published, and contributes only build-essential/git/git-lfs.
# Those are inlined below so one file describes the whole environment.
#
# NSIS comes from the distribution rather than an unpinned electron-builder
# tarball fetched over the network at build time. The installer script uses only
# MUI2, which is stock NSIS.
#
# An identical copy lives in ert-components; keep them in step.

FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        git \
        git-lfs \
        mingw-w64 \
        nsis \
        p7zip-full \
        wget \
        ca-certificates && \
    rm -rf /var/lib/apt/lists/*
