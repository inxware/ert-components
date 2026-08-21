#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2025 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# targetenv_xmos.sh — XMOS xcore.ai Phase 2 link
#
# Runs INSIDE the XMOS Docker container (invoked by targetenv_xmos_docker.sh
# via target_buildenv_run_command.sh). Takes the static library produced by
# Phase 1 (make all_docker) and links it against the XMOS fwk_rtos SDK to
# produce the final ehs.xe firmware image.
#
# *** STATUS: Phase 2 is incomplete. *** This script currently runs to the
# cmake-configure step and fails because xcore-ehs-app/CMakeLists.txt uses
# xcommon's XMOS_REGISTER_APP() macro, which expects xcommon-style modules
# with lib_build_info.cmake — fwk_rtos / fwk_core / fwk_io are not that
# shape. See docs/llm-dev-contexts/CLAUDE-xcore.md for the full Phase 2
# status, the architectural decision (BSP or no BSP), known landmines,
# and the todo list for finishing the .xe link.
#
# Stages, in order:
#   1. Validate that we're inside the XMOS container (XMOS_TOOL_PATH set, xcc
#      on PATH).
#   2. Source XTC Tools SetEnv to populate the XCC_* env vars (target / device
#      / library paths). Without this xcc cannot find board XN files.
#   3. Validate that Phase 1 produced its .a archive and that the SDK source
#      repos were cloned by build-xcore-freertos-ehs.sh on the host.
#   4. Resolve XMOS_BOARD_TARGET (from env, fallback to XCORE-AI-EXPLORER).
#   5. Configure xcore-ehs-app via cmake — this is the wrapper CMake project
#      that consumes xcommon_cmake's XMOS_REGISTER_APP() macro.
#   6. Build (cmake --build) — produces a .xe somewhere under the build dir.
#   7. Locate the .xe and copy it to TARGET_TREES.
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

# -----------------------------------------------------------------------------
# Stage 1 — Container sanity check
# -----------------------------------------------------------------------------
# XMOS_TOOL_PATH is set by the Dockerfile's ENV directive; xcc is provided by
# extracting the XTC Tools tarball into /opt/XMOS. If either is missing we
# almost certainly aren't running inside the xcore docker image.
# -----------------------------------------------------------------------------

if [ -z "${XMOS_TOOL_PATH}" ]; then
    echo "ERROR: XMOS_TOOL_PATH is not set. This script must run inside the XMOS Docker container."
    exit 1
fi

if ! command -v xcc &>/dev/null; then
    echo "ERROR: xcc not found on PATH. This script must run inside the XMOS Docker container."
    exit 1
fi

# -----------------------------------------------------------------------------
# Stage 2 — Source XTC Tools SetEnv
# -----------------------------------------------------------------------------
# The Dockerfile only sets XMOS_TOOL_PATH + PATH. SetEnv populates a much
# larger set of env vars that xcc/xcommon_cmake actually need at runtime:
#   XCC_TARGET_PATH         — search path for board .xn files. Without this
#                             xcc fails with `unknown target 'XCORE-AI-EXPLORER'`
#                             even though the .xn file exists at
#                             $XMOS_TOOL_PATH/targets/XCORE-AI-EXPLORER/.
#   XCC_DEVICE_PATH         — silicon device configs.
#   XCC_LIBRARY_PATH        — runtime library archive search path.
#   XCC_*_INCLUDE_PATH      — C / C++ / XC / asm header search paths.
#   XMOS_HOME, XMOS_CACHE_PATH, XMOS_REPO_PATH — paths under $HOME/.xmos.
#   XMOS_CMAKE_PATH         — set to the XTC-bundled xcommon_cmake (we override
#                             this in stage 5 to use the cloned SDK copy).
# SetEnv uses $PWD to discover its own location — cd in before sourcing.
# -----------------------------------------------------------------------------

pushd "${XMOS_TOOL_PATH}" > /dev/null
# shellcheck disable=SC1091
. ./SetEnv
popd > /dev/null

# -----------------------------------------------------------------------------
# Stage 3 — Validate inputs
# -----------------------------------------------------------------------------
# The Phase 1 archive must already exist. The SDK repos (cloned outside Docker
# by build-xcore-freertos-ehs.sh on the host into contrib/xmos-sdk/) must be
# present too — xcommon_cmake reads them in-tree, no in-container fetch.
# -----------------------------------------------------------------------------

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

# -----------------------------------------------------------------------------
# Stage 4 — Board target
# -----------------------------------------------------------------------------
# XMOS_BOARD_TARGET is the board name xcc resolves against XCC_TARGET_PATH.
# The default (XCORE-AI-EXPLORER) matches XMOS_BOARD_TARGET in the platform
# config.mk. The platform config value is currently NOT propagated through
# the Makefile → docker invocation, so a default fallback is used here.
# TODO: thread XMOS_BOARD_TARGET from platform config.mk through
#       targetenv_xmos_docker.sh as a -e env var so this fallback only fires
#       for genuinely unconfigured platforms.
# -----------------------------------------------------------------------------

if [ -z "${XMOS_BOARD_TARGET}" ]; then
    XMOS_BOARD_TARGET="XCORE-AI-EXPLORER"
    echo "WARNING: XMOS_BOARD_TARGET not set; defaulting to ${XMOS_BOARD_TARGET}"
fi

# -----------------------------------------------------------------------------
# Stage 5 — Configure (cmake)
# -----------------------------------------------------------------------------
# XMOS_CMAKE_PATH is read in TWO different ways and BOTH forms must agree:
#   - xcore-ehs-app/CMakeLists.txt:24 uses ${XMOS_CMAKE_PATH} — a CMake variable,
#     populated by -DXMOS_CMAKE_PATH=… on the command line below. Used to
#     locate xcommon.cmake itself.
#   - xcommon.cmake:16 uses $ENV{XMOS_CMAKE_PATH} — an environment variable —
#     to include xcore_xs.cmake (the actual xcc toolchain file). If unset,
#     CMake silently includes "/xcore_xs.cmake" (just slash + name); the
#     toolchain doesn't load and the build later fails with
#     "No sources present to determine architecture".
# Setting only the -D form gets past CMakeLists.txt but breaks inside
# xcommon.cmake. Setting only the env var works at runtime but breaks the
# initial include() of xcommon.cmake itself. Hence: both.
# We override the value SetEnv set (XTC-bundled xcommon_cmake) with the cloned
# SDK's copy under contrib/xmos-sdk/xcommon_cmake/ so the CMake macros and the
# fwk_* repo versions stay consistent.
# -----------------------------------------------------------------------------

export XMOS_CMAKE_PATH="${XMOS_SDK_DIR}/xcommon_cmake"
cmake -S "${APP_DIR}" -B "${BUILD_DIR}" \
    -DXMOS_CMAKE_PATH="${XMOS_CMAKE_PATH}" \
    -DXMOS_BOARD_TARGET="${XMOS_BOARD_TARGET}" \
    -DEHS_LIB="${EHS_LIB}"

# -----------------------------------------------------------------------------
# Stage 6 — Build (cmake --build)
# -----------------------------------------------------------------------------
# xcommon_cmake hides the actual link command behind its XMOS_REGISTER_APP()
# macro. Output is something like ${BUILD_DIR}/bin/<app_name>.xe — the exact
# filename depends on how the macro names targets internally, so stage 7
# discovers it by glob rather than by hard-coded path.
# -----------------------------------------------------------------------------

cmake --build "${BUILD_DIR}" --parallel

# -----------------------------------------------------------------------------
# Stage 7 — Install output to TARGET_TREES
# -----------------------------------------------------------------------------

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
