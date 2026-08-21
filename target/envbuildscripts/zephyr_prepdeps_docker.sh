#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2025 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# zephyr_prepdeps_docker.sh
#
# Runs zephyr_prepdeps.sh (the 'west init'/'west update' fetch) inside the
# Zephyr Docker image, using the standard target_buildenv_run_command.sh
# infrastructure — mirrors zephyr_build_docker.sh. 'west' is never required
# on the host; the image already bakes it in.
#
# This is the one Zephyr build step that needs internet access. We pass
# '--network host' for this container only (via EHS_DOCKER_EXTRA_RUN_ARGS,
# read by target_buildenv_run_command.sh) — that uses the host's own network
# stack directly instead of Docker's default bridge network, which is enough
# to work around hosts that block bridge-network egress for containers by
# firewall policy while still allowing the host's own traffic through. The
# actual compile step (zephyr_build_docker.sh) does not set this — it never
# needs the network, so it keeps Docker's normal network isolation.
#
# If the container still can't reach the internet with '--network host' (a
# genuinely offline/air-gapped host), zephyr_prepdeps.sh detects that and
# prints an explicit error explaining what to check / how to work around it,
# rather than hanging or failing with a confusing partial-clone error.
#
# Usage:
#   make zephyr_prepdeps
#   TARGET=zephyr_arm-nrf5340_nrf5340dk make zephyr_prepdeps

set -e

export TARGET="${TARGET:-$1}"
if [ -z "${TARGET}" ]; then
    echo "ERROR: TARGET not set." >&2; exit 1
fi

export ERT_ZEPHYR_VERSION
export ERT_ZEPHYR_MANIFEST
if [ -n "${ZEPHYR_BASE}" ]; then
    export ZEPHYR_BASE
fi

VARS_FILE="${PWD}/../TARGET_TREES/ehs_env-${TARGET}/zephyr-staging/make_vars.env"
if [ -z "${ERT_ZEPHYR_MANIFEST}" ] && [ -f "${VARS_FILE}" ]; then
    ERT_ZEPHYR_MANIFEST=$(grep '^ERT_ZEPHYR_MANIFEST=' "${VARS_FILE}" | cut -d= -f2-)
fi

export EHS_DOCKER_EXTRA_RUN_ARGS="--network host"

echo "========================================================="
echo "  Zephyr dependency fetch (west init/update)"
echo "  TARGET : ${TARGET}"
echo "========================================================="

if [ -n "${ERT_ZEPHYR_MANIFEST}" ]; then
    # NCS toolchain image (ENTRYPOINT ["/bin/bash","-c"]): pass one command string.
    ./target/envbuildscripts/target_buildenv_run_command.sh \
        "bash ./target/envbuildscripts/zephyr_prepdeps.sh"
else
    # Upstream zephyrproject-rtos/ci image (no entrypoint): 'sh -c <script>' form.
    ./target/envbuildscripts/target_buildenv_run_command.sh \
        sh -c "./target/envbuildscripts/zephyr_prepdeps.sh"
fi
