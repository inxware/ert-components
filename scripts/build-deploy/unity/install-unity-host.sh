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
# Host Unity SDK Installer
#
# Installs Unity Hub (if needed) then prints step-by-step instructions
# for installing Unity 2019.4.40f1 with Android support via the Hub GUI.
#
# The Unity Hub headless CLI is unreliable (hangs waiting for the Hub
# daemon), so the editor install is done manually through the GUI.
#
# Target install path expected by the build system:
#   /opt/unity3d/2019.4.40f1/Editor/Unity
#
# After following the steps below, make targetenv_unity_export will find
# Unity automatically.
#
# Usage:
#   ./scripts/build-deploy/unity/install-unity-host.sh
#
# Requirements:
#   - Ubuntu 20.04 / 22.04 (or compatible Debian-based distro)
#   - sudo access
#   - Internet access

set -e

UNITY_VERSION="2022.3.62f3"
UNITY_CHANGESET=""  # look up at https://unity.com/releases/editor/whats-new/2022.3.62
UNITY_INSTALL_ROOT="/opt/unity3d"
UNITY_EDITOR_PATH="${UNITY_INSTALL_ROOT}/${UNITY_VERSION}/Editor"

echo "======================================================================"
echo " Unity Host SDK Installer"
echo " Version  : ${UNITY_VERSION}"
echo " Install  : ${UNITY_INSTALL_ROOT}/${UNITY_VERSION}/"
echo "======================================================================"
echo ""

if [ -x "${UNITY_EDITOR_PATH}/Unity" ]; then
    echo "Unity ${UNITY_VERSION} is already installed at ${UNITY_EDITOR_PATH}."
    echo "Nothing to do."
    exit 0
fi

# ── Step 1: install Unity Hub via APT (if not already present) ───────────
if ! command -v unityhub &>/dev/null; then
    echo "Step 1: Installing Unity Hub ..."
    wget -qO - https://hub.unity3d.com/linux/keys/public | gpg --dearmor | sudo tee /usr/share/keyrings/Unity_Technologies_ApS.gpg >/dev/null
    echo "deb [signed-by=/usr/share/keyrings/Unity_Technologies_ApS.gpg] https://hub.unity3d.com/linux/repos/deb stable main" | sudo tee /etc/apt/sources.list.d/unityhub.list
    sudo apt-get update -qq
    sudo apt-get install -y unityhub
    echo "Unity Hub installed."
else
    echo "Step 1: Unity Hub already installed. Skipping."
fi

# ── Step 2: create /opt/unity3d with correct permissions ─────────────────
echo ""
echo "Step 2: Creating ${UNITY_INSTALL_ROOT} ..."
sudo mkdir -p "${UNITY_INSTALL_ROOT}"
sudo chown "$(id -u):$(id -g)" "${UNITY_INSTALL_ROOT}"
echo "Done — ${UNITY_INSTALL_ROOT} is writable by ${USER}."

# ── Step 3: manual instructions for the Hub GUI ──────────────────────────
echo ""
echo "======================================================================"
echo " Step 3 — Install Unity ${UNITY_VERSION} via Unity Hub GUI"
echo ""
echo " The Unity Hub headless CLI is unreliable so this step is manual."
echo ""
echo " 1. Launch Unity Hub:"
echo "      unityhub"
echo ""
echo " 2. Sign in (or create a free account at https://id.unity.com)"
echo ""
echo " 3. Activate a Personal license:"
echo "      Preferences -> Licenses -> Add -> Get a free Personal license"
echo ""
echo " 4. Install the editor:"
echo "      Installs -> Install Editor -> Archive tab"
echo "      -> find ${UNITY_VERSION} (LTS) -> Install"
echo ""
echo "    When prompted for modules, tick:"
echo "      [x] Android Build Support"
echo "            [x] Android SDK & NDK Tools"
echo "            [x] OpenJDK"
echo ""
echo " 5. IMPORTANT — change the install location to ${UNITY_INSTALL_ROOT}:"
echo "      Preferences -> Installs -> Default install location"
echo "      -> set to: ${UNITY_INSTALL_ROOT}"
echo "    (Do this BEFORE clicking Install in step 4)"
echo ""
echo " 6. Once installed, verify the build system can find it:"
echo "      ls ${UNITY_EDITOR_PATH}/Unity"
echo ""
echo " Then run make targetenv_unity_export as normal."
echo "======================================================================"
