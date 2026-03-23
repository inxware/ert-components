#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2025 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# targetenv_xmos_docker.sh
#
# Phase 2 of the xcore two-phase build: links the Phase 1 static library
# (.a) produced by 'make all' against the XMOS fwk_rtos SDK via xcommon_cmake
# inside the XMOS Docker container, producing the final .xe firmware image.
#
# Usage (from ert-components/):
#   make targetenv_xmos_docker
#
# Prerequisites:
#   1. 'make all' (or 'make all_docker') must have completed successfully,
#      producing ehs_$(TARGET).a in the ert-components working directory.
#   2. The XMOS Docker image must be built locally:
#        cd ../ert-components && make build_docker_local
#      (The image contains proprietary XTC Tools and cannot be pulled from
#       DockerHub.)
#   3. SDK source repos must be cloned on the host (done automatically by
#      ert-contrib-middleware/inx_build_scripts/build-xcore-freertos-ehs.sh).
#
# Output:
#   ../TARGET_TREES/ehs_env-$(TARGET)/bin/ehs.xe

set -e

export TARGET=$1
if [ -z "$TARGET" ]; then
    echo "ERROR: TARGET not specified. Run via 'make targetenv_xmos_docker'."
    exit 1
fi

PATH_TO_TARGET_DOCKER_IMAGE="${PWD}/target/platform/${TARGET}/Dockerimagename"
if [ ! -f "${PATH_TO_TARGET_DOCKER_IMAGE}" ]; then
    echo "ERROR: No Dockerimagename file at ${PATH_TO_TARGET_DOCKER_IMAGE}"
    echo "       Is TARGET=${TARGET} an xcore platform?"
    exit 1
fi

echo "=================================================="
echo "  XMOS Phase 2: linking .xe firmware for ${TARGET}"
echo "=================================================="

./target/envbuildscripts/target_buildenv_run_command.sh \
    sh -c "./target/envbuildscripts/targetenv_xmos.sh ${TARGET}"

echo "=================================================="
echo "  XMOS Phase 2 complete"
echo "=================================================="
