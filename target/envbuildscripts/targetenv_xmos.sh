#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2025 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# targetenv_xmos.sh
#
# Runs INSIDE the XMOS Docker container (invoked by targetenv_xmos_docker.sh
# via target_buildenv_run_command.sh).
#
# Takes the static library produced by Phase 1 (make all) and links it against
# the XMOS fwk_rtos SDK using xcommon_cmake to produce the final .xe firmware.
#
# Paths inside the Docker container:
#   /inxware/ert-components/          — ert-components working directory
#   /inxware/ert-contrib-middleware/  — ert-contrib-middleware repo
#   /inxware/TARGET_TREES/            — output staging tree

set -e

TARGET=$1
if [ -z "$TARGET" ]; then
    echo "ERROR: TARGET not specified."
    exit 1
fi

EHS_ROOT="/inxware/ert-components"
MIDDLEWARE_ROOT="/inxware/ert-contrib-middleware"
XMOS_SDK_DIR="${MIDDLEWARE_ROOT}/contrib/xmos-sdk"
EHS_LIB="${EHS_ROOT}/ehs_${TARGET}.a"
OUTPUT_DIR="/inxware/TARGET_TREES/ehs_env-${TARGET}/bin"
APP_DIR="${XMOS_SDK_DIR}/xcore-ehs-app"
BUILD_DIR="${APP_DIR}/_build_${TARGET}"

# --- Validate prerequisites ---

if [ -z "${XMOS_TOOL_PATH}" ]; then
    echo "ERROR: XMOS_TOOL_PATH is not set. This script must run inside the XMOS Docker container."
    exit 1
fi

if ! command -v xcc &>/dev/null; then
    echo "ERROR: xcc not found on PATH. This script must run inside the XMOS Docker container."
    exit 1
fi

if [ ! -f "${EHS_LIB}" ]; then
    echo "ERROR: Phase 1 library not found: ${EHS_LIB}"
    echo "       Run 'make all' or 'make all_docker' first to produce the static library."
    exit 1
fi

for REQUIRED_REPO in xcommon_cmake fwk_rtos fwk_io fwk_core; do
    if [ ! -d "${XMOS_SDK_DIR}/${REQUIRED_REPO}" ]; then
        echo "ERROR: ${REQUIRED_REPO} not found at ${XMOS_SDK_DIR}/${REQUIRED_REPO}"
        echo "       Run build-xcore-freertos-ehs.sh on the host to clone SDK sources."
        exit 1
    fi
done

echo "--- XMOS Phase 2: final .xe link ---"
echo "    EHS library : ${EHS_LIB}"
echo "    SDK dir     : ${XMOS_SDK_DIR}"
echo "    Output      : ${OUTPUT_DIR}/ehs.xe"
echo ""

# Read board target from platform config (XMOS_BOARD_TARGET may be in env or config.mk)
if [ -z "${XMOS_BOARD_TARGET}" ]; then
    XMOS_BOARD_TARGET="XCORE-AI-EXPLORER"
    echo "WARNING: XMOS_BOARD_TARGET not set; defaulting to ${XMOS_BOARD_TARGET}"
fi

# --- Run cmake via xcommon_cmake wrapper app ---
# XMOS_CMAKE_PATH is used by xcore-ehs-app/CMakeLists.txt to locate
# xcommon.cmake (which sets up the xcc toolchain and XCORE platform).
cmake -S "${APP_DIR}" -B "${BUILD_DIR}" \
    -DXMOS_CMAKE_PATH="${XMOS_SDK_DIR}/xcommon_cmake" \
    -DXMOS_BOARD_TARGET="${XMOS_BOARD_TARGET}" \
    -DEHS_LIB="${EHS_LIB}"

cmake --build "${BUILD_DIR}" --parallel

# --- Install output ---
mkdir -p "${OUTPUT_DIR}"
XE_FILE=$(find "${BUILD_DIR}" -name "*.xe" | head -1)
if [ -z "${XE_FILE}" ]; then
    echo "ERROR: No .xe file found in ${BUILD_DIR} after build."
    exit 1
fi
cp -f "${XE_FILE}" "${OUTPUT_DIR}/ehs.xe"

echo ""
echo "--- Phase 2 complete ---"
echo "    Firmware: ${OUTPUT_DIR}/ehs.xe"
