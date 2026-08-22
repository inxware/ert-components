#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2025 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# zephyr_build.sh
#
# Runs the Zephyr west build for the current TARGET.  Can be invoked:
#   (a) Inside Docker  — called by zephyr_build_docker.sh via Docker run.
#   (b) Natively       — called directly when ZEPHYR_BASE points to a local
#                        Zephyr installation (no Docker needed).
#
# Required environment:
#   TARGET           eRT platform target name.
#   ERT_ZEPHYR_BOARD     Zephyr board identifier (e.g. nrf5340dk/nrf5340/cpuapp).
#
# Optional environment:
#   ZEPHYR_BASE      Absolute path to a Zephyr installation.  When set the
#                    existing tree is used and west init/update are skipped.
#                    When unset, the west workspace in the staging directory
#                    is used — run 'make zephyr_prepdeps' beforehand to fetch
#                    it (needs internet; runs inside Docker with
#                    '--network host', not natively). This script never
#                    fetches it itself, so it never needs the network.
#   ERT_ZEPHYR_PRISTINE  Set to 'y' to force a pristine (from-scratch) build
#                    by passing -p always to west build.  Default: 'auto'
#                    (incremental when the build dir already exists).
#
# Paths inside Docker (mounted as /inxware):
#   /inxware/ert-components/    — this repo
#   /inxware/TARGET_TREES/      — output staging area

set -e

# -----------------------------------------------------------------------
# Resolve TARGET and ERT_ZEPHYR_BOARD
# -----------------------------------------------------------------------
if [ -z "${TARGET}" ]; then
    export TARGET="$1"
fi
if [ -z "${TARGET}" ]; then
    echo "ERROR: TARGET is not set." >&2; exit 1
fi

# If ERT_ZEPHYR_BOARD / ERT_ZEPHYR_VERSION are not already in the environment,
# read them from the generated make_vars.env so this script works without the
# full make context.
VARS_FILE="${PWD}/../TARGET_TREES/ehs_env-${TARGET}/zephyr-staging/make_vars.env"
if [ -z "${ERT_ZEPHYR_BOARD}" ]; then
    if [ -f "${VARS_FILE}" ]; then
        ERT_ZEPHYR_BOARD=$(grep '^ERT_ZEPHYR_BOARD=' "${VARS_FILE}" | cut -d= -f2-)
    fi
fi
if [ -z "${ERT_ZEPHYR_BOARD}" ]; then
    echo "ERROR: ERT_ZEPHYR_BOARD is not set and could not be read from make_vars.env." >&2
    exit 1
fi
if [ -z "${ERT_ZEPHYR_VERSION}" ]; then
    if [ -f "${VARS_FILE}" ]; then
        ERT_ZEPHYR_VERSION=$(grep '^ERT_ZEPHYR_VERSION=' "${VARS_FILE}" | cut -d= -f2-)
    fi
fi
# ERT_ZEPHYR_MANIFEST (west manifest repo URL) is empty for upstream-Zephyr
# targets and set for nRF Connect SDK targets (selects the west init manifest).
if [ -z "${ERT_ZEPHYR_MANIFEST}" ]; then
    if [ -f "${VARS_FILE}" ]; then
        ERT_ZEPHYR_MANIFEST=$(grep '^ERT_ZEPHYR_MANIFEST=' "${VARS_FILE}" | cut -d= -f2-)
    fi
fi
# ERT_ZEPHYR_SYSBUILD=y builds the MCUboot multi-image (needed for USB/MCUboot
# DFU); otherwise NCS targets build single-image (--no-sysbuild) for bring-up.
if [ -z "${ERT_ZEPHYR_SYSBUILD}" ]; then
    if [ -f "${VARS_FILE}" ]; then
        ERT_ZEPHYR_SYSBUILD=$(grep '^ERT_ZEPHYR_SYSBUILD=' "${VARS_FILE}" | cut -d= -f2-)
    fi
fi

STAGING_DIR="${PWD}/../TARGET_TREES/ehs_env-${TARGET}/zephyr-staging"
APP_DIR="${STAGING_DIR}/app"
BUILD_DIR="${STAGING_DIR}/build"
OUTPUT_BIN="${PWD}/../TARGET_TREES/ehs_env-${TARGET}/bin"

# Zephyr/NCS builds use ccache when present. In Docker we run as the mapped host
# user, for whom the toolchain image's default ccache dir (/opt/.cache/ccache)
# is not writable → "Permission denied". Redirect ccache to the host-mounted,
# writable staging dir so it works and persists across builds.
export CCACHE_DIR="${STAGING_DIR}/.ccache"
mkdir -p "${CCACHE_DIR}" 2>/dev/null || true

if [ ! -f "${APP_DIR}/CMakeLists.txt" ]; then
    echo "ERROR: ${APP_DIR}/CMakeLists.txt not found." >&2
    echo "       Run 'make zephyr_cmake_gen' first." >&2
    exit 1
fi

echo "========================================================="
echo "  eRT Zephyr build"
echo "  TARGET      : ${TARGET}"
echo "  BOARD       : ${ERT_ZEPHYR_BOARD}"
echo "  APP         : ${APP_DIR}"
echo "  BUILD DIR   : ${BUILD_DIR}"
echo "========================================================="

# -----------------------------------------------------------------------
# Zephyr workspace resolution (skipped when ZEPHYR_BASE is provided).
#
# The fetch itself (west init/update) does NOT happen here — it runs in its
# own Docker invocation via zephyr_prepdeps.sh (see zephyr_prepdeps_docker.sh),
# so this script only ever needs a tree that is already present on disk and
# never touches the network itself.
# -----------------------------------------------------------------------
if [ -n "${ZEPHYR_BASE}" ]; then
    echo "Using provided ZEPHYR_BASE: ${ZEPHYR_BASE}"
else
    # Shared SDK source, one tree per (manifest, version) — see
    # zephyr_sdk_paths.sh for why it is arch-free and lives outside TARGET_TREES.
    # shellcheck source=zephyr_sdk_paths.sh
    . "$(dirname "$0")/zephyr_sdk_paths.sh"
    ZEPHYR_BASE="${WEST_WORKSPACE}/zephyr"

    # Gate on the completion stamp zephyr_prepdeps.sh writes, not just '.west'
    # existing — '.west' is created before the actual clone happens, so a
    # workspace interrupted mid-fetch would otherwise look ready when it isn't.
    if [ ! -f "${WEST_WORKSPACE}/.zephyr_prepdeps_complete" ]; then
        echo "ERROR: SDK source not found (or incomplete) at ${WEST_WORKSPACE}." >&2
        echo "       That is the shared ${EHS_ZEPHYR_SDK_ID} tree." >&2
        echo "       Run 'make zephyr_prepdeps' first (needs internet)." >&2
        exit 1
    fi
    echo "Using shared SDK source ${EHS_ZEPHYR_SDK_ID} at ${WEST_WORKSPACE}"
    # Optionally run west update to keep modules current:
    # pushd "${WEST_WORKSPACE}" >/dev/null && west update && popd >/dev/null
fi
export ZEPHYR_BASE

echo "ZEPHYR_BASE   : ${ZEPHYR_BASE}"

# -----------------------------------------------------------------------
# EHS_ROOT: path to ert-components inside the current environment.
# Inside Docker this is /inxware/ert-components; natively it is PWD.
# EHS_BUILD_SUPPORT: path to ert-build-support (one level up from ert-components).
# EHS_KERNELS: path to ert-kernels, which is where the pre-built kernel archives
# live since they moved out of ert-build-support into their own repo.
# -----------------------------------------------------------------------
EHS_ROOT="${PWD}"
EHS_BUILD_SUPPORT="${EHS_ROOT%/ert-components}/ert-build-support"
EHS_KERNELS="${EHS_ROOT%/ert-components}/ert-kernels"
# Exported (not only passed as -D) so the generated app CMakeLists can pick them
# up from the environment under sysbuild, where the top-level -D is not forwarded
# to the app sub-image (see scripts/zephyr_cmake_gen.py).
export EHS_ROOT EHS_BUILD_SUPPORT EHS_KERNELS
echo "EHS_ROOT         : ${EHS_ROOT}"
echo "EHS_BUILD_SUPPORT: ${EHS_BUILD_SUPPORT}"
echo "EHS_KERNELS      : ${EHS_KERNELS}"

# -----------------------------------------------------------------------
# Pristine / incremental build selection
# -----------------------------------------------------------------------
# ERT_ZEPHYR_PRISTINE accepts 'y'/'yes'/'always' for a clean build, or
# 'n'/'no'/'never' to force incremental; anything else (or unset) → 'auto'.
case "${ERT_ZEPHYR_PRISTINE:-auto}" in
    y|yes|always) PRISTINE=always ;;
    n|no|never)   PRISTINE=never  ;;
    *)            PRISTINE=auto   ;;
esac

# Sysbuild selection.
#   ERT_ZEPHYR_SYSBUILD=y  -> --sysbuild: MCUboot multi-image (mcuboot + b0 + app),
#                            signs the app and emits merged.hex + dfu_application.zip
#                            (needed for USB/MCUboot DFU). The app CMakeLists reads
#                            EHS_ROOT/EHS_BUILD_SUPPORT from the exported env, since
#                            sysbuild does not forward the top-level -D to it.
#   otherwise, NCS targets -> --no-sysbuild: single-image bring-up (zephyr.hex).
#   upstream-Zephyr targets -> no flag (Zephyr default).
case "${ERT_ZEPHYR_SYSBUILD}" in
    y|yes|true) SYSBUILD_ARG="--sysbuild" ;;
    *) if [ -n "${ERT_ZEPHYR_MANIFEST}" ]; then SYSBUILD_ARG="--no-sysbuild"; else SYSBUILD_ARG=""; fi ;;
esac

# Force the NCS partition manager to use our static layout (pm_static.yml copied
# into the app dir by zephyr_cmake_gen.py) instead of the board default — the
# board's variant-specific file otherwise wins. This is what carves the
# littlefs_storage partition (external flash) for the SODL filesystem.
PM_STATIC_ARG=""
if [ -f "${APP_DIR}/pm_static.yml" ]; then
    PM_STATIC_ARG="-DPM_STATIC_YML_FILE=${APP_DIR}/pm_static.yml"
    echo "  Static partition layout: ${APP_DIR}/pm_static.yml"
fi

echo "---------------------------------------------------------"
echo "Running west build (pristine=${PRISTINE}${SYSBUILD_ARG:+, ${SYSBUILD_ARG}}) ..."

west build \
    ${SYSBUILD_ARG} \
    -b "${ERT_ZEPHYR_BOARD}" \
    -p "${PRISTINE}" \
    -d "${BUILD_DIR}" \
    "${APP_DIR}" \
    -- -DEHS_ROOT="${EHS_ROOT}" \
       -DEHS_BUILD_SUPPORT="${EHS_BUILD_SUPPORT}" \
       -DEHS_KERNELS="${EHS_KERNELS}" \
       ${PM_STATIC_ARG}

echo "---------------------------------------------------------"

# -----------------------------------------------------------------------
# Copy output firmware to bin/
# -----------------------------------------------------------------------
mkdir -p "${OUTPUT_BIN}"

# Collect output artefacts if they exist. First set = single-image (--no-sysbuild)
# layout; second set = sysbuild layout (app image under app/, plus the
# sysbuild-level merged.hex and the USB/MCUboot dfu_application.zip).
COPIED=0
for artefact in \
    "${BUILD_DIR}/zephyr/zephyr.hex" \
    "${BUILD_DIR}/zephyr/zephyr.elf" \
    "${BUILD_DIR}/zephyr/zephyr.bin" \
    "${BUILD_DIR}/app/zephyr/zephyr.elf" \
    "${BUILD_DIR}/app/zephyr/zephyr.signed.hex" \
    "${BUILD_DIR}/app/zephyr/zephyr.signed.bin" \
    "${BUILD_DIR}/merged.hex" \
    "${BUILD_DIR}/dfu_application.zip"
do
    if [ -f "${artefact}" ]; then
        cp -f "${artefact}" "${OUTPUT_BIN}/"
        echo "Copied: $(basename ${artefact}) → ${OUTPUT_BIN}/"
        COPIED=$((COPIED + 1))
    fi
done

if [ "${COPIED}" -eq 0 ]; then
    echo "WARNING: no firmware artefacts found in ${BUILD_DIR}/zephyr/" >&2
    exit 1
fi

echo "========================================================="
echo "  Zephyr build complete."
echo "  Firmware in: ${OUTPUT_BIN}"
echo "========================================================="
