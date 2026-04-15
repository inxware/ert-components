#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2025, inx limited, UK.
# All Rights Reserved.
# You may use, distribute and modify this code under the terms
# of the LGPLv3 license. You should have received a copy of the
# LGPLv3 (GNU LESSER GENERAL PUBLIC LICENSE Version 3) license
# with this file. If not, please visit
#	<https://www.gnu.org/licenses/lgpl-3.0.txt>
#---------------------------------------------------------------
#
# Unity License Setup — GameCI Docker method
#
# Activates a Unity Personal license online using your Unity account
# credentials, then saves the resulting license file so that
# make targetenv_unity_export_docker can run headless builds using the
# unityci/editor Docker image (no local Unity Hub install needed).
#
# Run this ONCE on any machine that will build Unity targets.
# Add the export line printed at the end to ~/.bashrc (or your CI
# secrets store).
#
# Usage:
#   ./scripts/build-deploy/unity/setup-unity-license.sh
#
# Or with credentials inline (non-interactive, e.g. for CI setup):
#   UNITY_EMAIL=you@example.com UNITY_PASSWORD=secret \
#       ./scripts/build-deploy/unity/setup-unity-license.sh
#
# Requirements:
#   - Docker installed and the current user in the docker group
#   - A free Unity account at https://id.unity.com
#   - Internet access from the Docker container for online activation
#   - 2FA must be disabled on the Unity account used here
#
# Unity version targeted by this repo:
UNITY_VERSION="2022.3.62f3"
UNITY_DOCKER_IMAGE="unityci/editor:ubuntu-${UNITY_VERSION}-android-3.1.0"
ACTIVATION_DIR="/tmp/unity-activation"
CONTAINER_NAME="unity-license-activation-$$"

set -e

echo "======================================================================"
echo " Unity License Setup for eRT Unity builds"
echo " Unity version : $UNITY_VERSION"
echo " Docker image  : $UNITY_DOCKER_IMAGE"
echo "======================================================================"
echo ""
echo " NOTE: Unity no longer supports manual offline activation for Personal"
echo " licenses. This script activates online using your Unity account."
echo " 2FA must be disabled on the account used here."
echo ""

# ── Prompt for credentials if not already set ────────────────────────────
if [ -z "$UNITY_EMAIL" ]; then
    read -rp "Unity account email: " UNITY_EMAIL
fi
if [ -z "$UNITY_PASSWORD" ]; then
    read -rsp "Unity account password: " UNITY_PASSWORD
    echo ""
fi

if [ -z "$UNITY_EMAIL" ] || [ -z "$UNITY_PASSWORD" ]; then
    echo "ERROR: UNITY_EMAIL and UNITY_PASSWORD must be set."
    exit 1
fi

mkdir -p "$ACTIVATION_DIR"

# Clean up any leftover container from a previous failed run
docker rm "$CONTAINER_NAME" 2>/dev/null || true

# ── Activate online — use a named container so we can docker cp the result ──
# We do NOT use --rm here so the container's filesystem survives for inspection.
# -quit tells Unity to exit after activation rather than hanging.
echo ""
echo "Activating Unity Personal license online ..."

docker run --name "$CONTAINER_NAME" --privileged --network host \
    "${UNITY_DOCKER_IMAGE}" \
    /opt/unity/Editor/Unity \
        -quit -batchmode -nographics \
        -username "${UNITY_EMAIL}" \
        -password "${UNITY_PASSWORD}" \
        -logFile /dev/stdout || true

# ── Copy the license file out of the stopped container ───────────────────
echo ""
echo "Searching for license file in stopped container ..."

# Try the standard Linux path first, then search broadly if not found
docker cp "${CONTAINER_NAME}:/root/.local/share/unity3d/Unity/." \
    "${ACTIVATION_DIR}/" 2>/dev/null || true

ULF_FILE=$(ls "${ACTIVATION_DIR}"/Unity_lic.ulf \
              "${ACTIVATION_DIR}"/Unity_v*.ulf 2>/dev/null | head -1)

if [ -z "$ULF_FILE" ]; then
    # Broad search: dump all .ulf files from anywhere in the container
    echo "Not found at standard path — searching entire container filesystem ..."
    FOUND_IN_CONTAINER=$(docker exec "$CONTAINER_NAME" \
        find / -name "*.ulf" 2>/dev/null | head -5 || true)
    if [ -n "$FOUND_IN_CONTAINER" ]; then
        echo "Found .ulf file(s) in container at:"
        echo "$FOUND_IN_CONTAINER"
        FIRST=$(echo "$FOUND_IN_CONTAINER" | head -1)
        docker cp "${CONTAINER_NAME}:${FIRST}" "${ACTIVATION_DIR}/"
        ULF_FILE="${ACTIVATION_DIR}/$(basename "$FIRST")"
    fi
fi

docker rm "$CONTAINER_NAME" 2>/dev/null || true

if [ -z "$ULF_FILE" ]; then
    echo ""
    echo "======================================================================"
    echo "ERROR: No license file found."
    echo ""
    echo "Possible causes:"
    echo "  - Wrong email or password"
    echo "  - 2FA is enabled on the Unity account (disable it and retry)"
    echo "  - Network not reachable from Docker container"
    echo "  - Unity activation API changed for this version"
    echo ""
    echo "Check the Docker log output above for 'license' or 'error' messages."
    echo "======================================================================"
    exit 1
fi

echo ""
echo "License file saved to: $ULF_FILE"

# ── Print the export instruction ─────────────────────────────────────────
echo ""
echo "======================================================================"
echo " License activation complete."
echo ""
echo " Add the following line to your ~/.bashrc so that"
echo " make targetenv_unity_export_docker can use this license:"
echo ""
echo "   export UNITY_LICENSE=\$(cat '${ULF_FILE}')"
echo ""
echo " To apply it in this shell session right now, run:"
echo "   export UNITY_LICENSE=\$(cat '${ULF_FILE}')"
echo "======================================================================"
