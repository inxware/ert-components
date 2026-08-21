#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2025 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# zephyr_build_docker.sh
#
# Runs zephyr_build.sh inside the Zephyr CI Docker image using
# the standard target_buildenv_run_command.sh infrastructure.
#
# Does NOT fetch the Zephyr/NCS west workspace itself — that happens in a
# separate, earlier Docker invocation via zephyr_prepdeps_docker.sh (run
# automatically as a Make prerequisite; see that script for why the fetch
# needs its own container run with '--network host'). This script's
# container never touches the network — it only compiles against a tree
# that already exists on disk, so it keeps Docker's normal network isolation.
#
# Optional environment overrides:
#   ZEPHYR_BASE        Path to a pre-installed Zephyr tree.  When set
#                      the ZEPHYR_BASE is exported into Docker; west
#                      init/update are skipped.
#   ERT_ZEPHYR_PRISTINE  Set to 'y' to force a pristine build.
#
# Usage:
#   make zephyr_build_docker
#   TARGET=zephyr_arm-nrf5340_nrf5340dk make zephyr_build_docker

# TODO!!: Message to CLAUDE!!  - is this really necessary? Why don't we just run 
#./target/envbuildscripts/target_buildenv_run_command.sh \
#    sh -c "./target/envbuildscripts/zephyr_build.sh"
# Like we do elswhere? The exports can be done in the .mk files?

set -e

export TARGET="${TARGET:-$1}"
if [ -z "${TARGET}" ]; then
    echo "ERROR: TARGET not set." >&2; exit 1
fi

# Forward Zephyr-specific variables so target_buildenv_run_command.sh
# includes them in the docker run -e list via INX_ERTCOMPONENTS_BUILDENV.
export ERT_ZEPHYR_BOARD
export ERT_ZEPHYR_VERSION
export ERT_ZEPHYR_PRISTINE
export ERT_ZEPHYR_MANIFEST
if [ -n "${ZEPHYR_BASE}" ]; then
    export ZEPHYR_BASE
fi

# Detect an nRF Connect SDK target: a non-empty ERT_ZEPHYR_MANIFEST (read from
# the generated make_vars.env when not already in the environment). The NCS
# sdk-nrf-toolchain image has ENTRYPOINT ["/bin/bash","-c"], so the container
# command must be a single string (not 'sh -c <script>', which that entrypoint
# would mis-split). The upstream zephyrproject-rtos/ci image has no entrypoint
# and takes the 'sh -c' form.
VARS_FILE="${PWD}/../TARGET_TREES/ehs_env-${TARGET}/zephyr-staging/make_vars.env"
if [ -z "${ERT_ZEPHYR_MANIFEST}" ] && [ -f "${VARS_FILE}" ]; then
    ERT_ZEPHYR_MANIFEST=$(grep '^ERT_ZEPHYR_MANIFEST=' "${VARS_FILE}" | cut -d= -f2-)
fi

echo "========================================================="
echo "  Zephyr Docker build"
echo "  TARGET : ${TARGET}"
echo "  IMAGE  : $(cat ${PWD}/target/platform/${TARGET}/Dockerimagename 2>/dev/null || echo '(unknown)')"
echo "========================================================="

if [ -n "${ERT_ZEPHYR_MANIFEST}" ]; then
    # NCS toolchain image (ENTRYPOINT ["/bin/bash","-c"]): pass one command string.
    ./target/envbuildscripts/target_buildenv_run_command.sh \
        "bash ./target/envbuildscripts/zephyr_build.sh"
else
    # Upstream zephyrproject-rtos/ci image (no entrypoint): 'sh -c <script>' form.
    ./target/envbuildscripts/target_buildenv_run_command.sh \
        sh -c "./target/envbuildscripts/zephyr_build.sh"
fi
