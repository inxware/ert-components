#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2025 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# zephyr_prepdeps.sh
#
# Fetches the Zephyr/NCS west workspace for TARGET by running 'west
# init'/'west update'. Always runs INSIDE the Zephyr Docker image (via
# zephyr_prepdeps_docker.sh) so 'west' never needs to be installed on the
# host — the image already bakes it in, same as for the compile step.
#
# Split out as its own step (rather than living inside zephyr_build.sh) so
# it only has to run once and is easy to retry in isolation; the actual
# compile in zephyr_build.sh never touches the network.
#
# Required environment:
#   TARGET    eRT platform target name.
#
# Optional environment (else read from the generated make_vars.env):
#   ERT_ZEPHYR_VERSION   Zephyr/NCS tag passed to 'west init --mr'.
#   ERT_ZEPHYR_MANIFEST  West manifest repo URL (set only for NCS targets).
#
# If ZEPHYR_BASE is already set (pointing at a local Zephyr/NCS install),
# this script is a no-op — that tree is used as-is, nothing to fetch.

set -e

if [ -z "${TARGET}" ]; then
    export TARGET="$1"
fi
if [ -z "${TARGET}" ]; then
    echo "ERROR: TARGET is not set." >&2; exit 1
fi

if [ -n "${ZEPHYR_BASE}" ]; then
    echo "ZEPHYR_BASE is set (${ZEPHYR_BASE}) — nothing to fetch."
    exit 0
fi

VARS_FILE="${PWD}/../TARGET_TREES/ehs_env-${TARGET}/zephyr-staging/make_vars.env"
if [ -z "${ERT_ZEPHYR_VERSION}" ] && [ -f "${VARS_FILE}" ]; then
    ERT_ZEPHYR_VERSION=$(grep '^ERT_ZEPHYR_VERSION=' "${VARS_FILE}" | cut -d= -f2-)
fi
if [ -z "${ERT_ZEPHYR_MANIFEST}" ] && [ -f "${VARS_FILE}" ]; then
    ERT_ZEPHYR_MANIFEST=$(grep '^ERT_ZEPHYR_MANIFEST=' "${VARS_FILE}" | cut -d= -f2-)
fi

STAGING_DIR="${PWD}/../TARGET_TREES/ehs_env-${TARGET}/zephyr-staging"
WEST_WORKSPACE="${STAGING_DIR}/west-workspace"
# Written only after 'west update' succeeds. 'west init' alone creates
# '.west/' before any real data transfer happens, so checking for '.west'
# is not sufficient — a network drop during 'west init's manifest clone or
# during 'west update's project clones would leave '.west' present but the
# workspace incomplete, and a naive check would then skip the fetch forever
# on every retry. Gating on this stamp instead means a failed attempt is
# always retried from scratch (see cleanup_on_failure below).
STAMP="${WEST_WORKSPACE}/.zephyr_prepdeps_complete"

if [ -f "${STAMP}" ]; then
    echo "West workspace already complete at ${WEST_WORKSPACE} — nothing to fetch."
    echo "(To refresh: cd ${WEST_WORKSPACE} && west update)"
    exit 0
fi

if [ -d "${WEST_WORKSPACE}" ]; then
    echo "Found an incomplete west workspace at ${WEST_WORKSPACE} (from an interrupted or failed previous fetch attempt) — removing it and starting a clean fetch."
    rm -rf "${WEST_WORKSPACE}"
fi

MANIFEST_URL="${ERT_ZEPHYR_MANIFEST:-https://github.com/zephyrproject-rtos/zephyr}"

# Fail fast with a clear message instead of letting 'west init' hang or spew
# a confusing partial-clone error if the container has no route out at all.
echo "Checking network access to ${MANIFEST_URL} ..."
if ! timeout 15 git ls-remote --exit-code "${MANIFEST_URL}" HEAD >/dev/null 2>&1; then
    echo "-------------------------------------------------------------------" >&2
    echo "ERROR: cannot reach ${MANIFEST_URL} from inside this Docker container." >&2
    echo "" >&2
    echo "This step (zephyr_prepdeps) needs to fetch the Zephyr/NCS source via" >&2
    echo "'west init'/'west update', which requires internet access from the" >&2
    echo "build container. We already run this container with '--network host'" >&2
    echo "to work around Docker bridge-network restrictions, so this looks like" >&2
    echo "the host itself (or its firewall/proxy) is blocking Docker containers" >&2
    echo "from reaching the internet entirely." >&2
    echo "" >&2
    echo "To fix this, on the HOST machine:" >&2
    echo "  - Confirm the host itself has internet access (e.g. curl -I ${MANIFEST_URL})" >&2
    echo "  - Check for a firewall/proxy rule that blocks Docker container traffic" >&2
    echo "    specifically (Docker containers use the host's network with" >&2
    echo "    --network host, so if the host can reach GitHub but this still" >&2
    echo "    fails, something is filtering traffic from the container process/user)" >&2
    echo "  - If this host is intentionally offline/air-gapped, pre-fetch the" >&2
    echo "    workspace on a machine with internet access and either copy it to" >&2
    echo "    ${WEST_WORKSPACE} (then re-run to pick it up), or set ZEPHYR_BASE" >&2
    echo "    to point at a Zephyr/NCS tree you already have available." >&2
    echo "-------------------------------------------------------------------" >&2
    exit 1
fi

WEST_MANIFEST_ARG=""
if [ -n "${ERT_ZEPHYR_MANIFEST}" ]; then
    WEST_MANIFEST_ARG="-m ${ERT_ZEPHYR_MANIFEST}"
    echo "  Using west manifest: ${ERT_ZEPHYR_MANIFEST}"
fi

# On any failure (network drop, Ctrl-C, ...) remove whatever was partially
# fetched so the workspace is never left in a state that looks fetched but
# isn't — the next run of this script gets a guaranteed-clean retry instead
# of tripping over a half-cloned manifest repo or 'west init' refusing to
# re-run because '.west' already exists.
cleanup_on_failure() {
    echo "ERROR: west fetch failed or was interrupted — removing incomplete workspace so the next attempt starts clean." >&2
    rm -rf "${WEST_WORKSPACE}"
}
trap cleanup_on_failure ERR INT TERM

echo "Initialising west workspace at ${WEST_WORKSPACE} ..."
mkdir -p "${WEST_WORKSPACE}"
if [ -n "${ERT_ZEPHYR_VERSION}" ]; then
    echo "  Pinning to revision ${ERT_ZEPHYR_VERSION}"
    west init ${WEST_MANIFEST_ARG} --mr "${ERT_ZEPHYR_VERSION}" "${WEST_WORKSPACE}"
else
    echo "  WARNING: ERT_ZEPHYR_VERSION not set — fetching latest (may break)"
    west init ${WEST_MANIFEST_ARG} "${WEST_WORKSPACE}"
fi
pushd "${WEST_WORKSPACE}" >/dev/null
west update
popd >/dev/null

trap - ERR INT TERM
touch "${STAMP}"
echo "West workspace ready at ${WEST_WORKSPACE}."
