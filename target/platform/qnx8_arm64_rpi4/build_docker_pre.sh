#!/bin/bash
# Platform pre-build hook for qnx8_arm64_rpi4.
# Sourced by target/envbuildscripts/build_docker_local.sh before docker build.
#
# This script gathers QNX SDP credentials interactively (if not already set
# as environment variables) and exports ERT_DOCKER_SECRETS for the generic
# build script to forward them to docker build as BuildKit --secret flags.
# Secrets are never stored in the image or visible in docker history.
#
# Usage (via make):
#   ./configure qnx8_arm64_rpi4
#   make build_docker_local
#
# To avoid re-entering credentials: export QNX_USER, QNX_PASS, QNX_LICENSE
# in your shell before running make, or add them to ~/.qnx_credentials and
# source that file (keep it out of version control).

# ---- Preflight: check the QNX Software Center installer is present ----

INSTALLER="target/platform/qnx8_arm64_rpi4/qnxsoftwarecenter.run"
if [ ! -f "${INSTALLER}" ]; then
    echo ""
    echo "ERROR: QNX Software Center installer not found."
    echo ""
    echo "  Expected: ${INSTALLER}  (relative to repo root)"
    echo ""
    echo "To obtain it:"
    echo "  1. Log in to your myQNX account at https://www.qnx.com/account/"
    echo "  2. Go to Downloads → QNX Software Center"
    echo "  3. Download the Linux x86-64 installer"
    echo "     (filename: qnxsoftwarecenter-linux64-x.x.x.run or similar)"
    echo "  4. Copy it to the expected path:"
    echo "     cp ~/Downloads/qnxsoftwarecenter-linux64-*.run \\"
    echo "        target/platform/qnx8_arm64_rpi4/qnxsoftwarecenter.run"
    echo ""
    echo "Then re-run:  make build_docker_local"
    echo ""
    exit 1
fi

# ---- Gather credentials ----

if [ -z "${QNX_USER}" ]; then
    read -r -p "myQNX account email: " QNX_USER
fi

if [ -z "${QNX_PASS}" ]; then
    read -r -s -p "myQNX password: " QNX_PASS
    echo
fi

if [ -z "${QNX_LICENSE}" ]; then
    read -r -p "QNX licence key (XXXX-XXXX-XXXX-XXXX, or press Enter to skip): " QNX_LICENSE
fi

export QNX_USER QNX_PASS QNX_LICENSE

# ---- Tell the generic script which --build-arg values to forward ----
# Format: space-separated NAME=VALUE pairs (passed as --build-arg NAME=VALUE)
ERT_DOCKER_BUILD_ARGS="QNX_USER=${QNX_USER} QNX_PASS=${QNX_PASS}"
if [ -n "${QNX_LICENSE}" ]; then
    ERT_DOCKER_BUILD_ARGS="${ERT_DOCKER_BUILD_ARGS} QNX_LICENSE=${QNX_LICENSE}"
fi
export ERT_DOCKER_BUILD_ARGS
