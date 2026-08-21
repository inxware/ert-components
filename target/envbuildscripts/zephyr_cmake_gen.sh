#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2025 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# zephyr_cmake_gen.sh
#
# Shell wrapper called by 'make zephyr_cmake_gen'.
# The Makefile recipe has already written all required make variables into
# <staging>/make_vars.env using GNU make's $(file ...) function.
# This script validates the environment then invokes the Python generator.
#
# Usage (called by Makefile, not directly):
#   ./target/envbuildscripts/zephyr_cmake_gen.sh <TARGET>

# TODO: Review the Makefile's Zephyr specific recipe - this should really call a GNU make procedure 
# defined in  ./target/os-arch/zephyr/toochain.mk (or target.mk?) 

set -e

TARGET="${1:-${TARGET}}"
if [ -z "${TARGET}" ]; then
    echo "ERROR: TARGET not specified." >&2
    exit 1
fi

STAGING_DIR="${PWD}/../TARGET_TREES/ehs_env-${TARGET}/zephyr-staging"
VARS_FILE="${STAGING_DIR}/make_vars.env"
OUTPUT_DIR="${STAGING_DIR}/app"

if [ ! -f "${VARS_FILE}" ]; then
    echo "ERROR: make_vars.env not found at ${VARS_FILE}" >&2
    echo "       The Makefile recipe should have written it before calling this script." >&2
    exit 1
fi

echo "=================================================="
echo "  Zephyr CMake generation for TARGET=${TARGET}"
echo "  Staging: ${STAGING_DIR}"
echo "=================================================="

python3 "${PWD}/scripts/zephyr_cmake_gen.py" \
    --vars  "${VARS_FILE}" \
    --output "${OUTPUT_DIR}"

echo "=================================================="
echo "  CMake generation complete."
echo "  Next step: make zephyr_build_docker"
echo "=================================================="
