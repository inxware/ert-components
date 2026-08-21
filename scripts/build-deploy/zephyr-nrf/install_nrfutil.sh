#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2026 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# install_nrfutil.sh
#
# Installs Nordic's nRF Util CLI ('nrfutil') and its 'device' command on a
# host Linux machine, for flashing / debugging Zephyr / nRF Connect SDK
# targets (e.g. Thingy:91 X, nRF9151/nRF5340 DKs).
#
# This is a HOST-NATIVE tool — flashing needs real USB / J-Link access, so it
# is installed on the developer machine, not inside a build Docker image.
#
# Usage:
#   ./install_nrfutil.sh                 # install to /usr/local/bin (sudo if needed)
#   INSTALL_DIR=~/.local/bin ./install_nrfutil.sh   # user-local, no sudo
#   NRFUTIL_URL=<url> ./install_nrfutil.sh          # override the download URL
#
# After install, both flashing paths for the Thingy:91 X are available via the
# same 'nrfutil device' command (see README.md):
#   - USB / MCUboot  (no probe; needs a signed dfu_application.zip)
#   - external SWD probe / J-Link (takes the raw zephyr.hex)

set -e

# Canonical Linux x86_64 nRF Util binary (Nordic Artifactory). Overridable.
NRFUTIL_URL="${NRFUTIL_URL:-https://files.nordicsemi.com/artifactory/swtools/external/nrfutil/executables/x86_64-unknown-linux-gnu/nrfutil}"
INSTALL_DIR="${INSTALL_DIR:-/usr/local/bin}"

echo "========================================================="
echo "  nRF Util install"
echo "  URL         : ${NRFUTIL_URL}"
echo "  INSTALL_DIR : ${INSTALL_DIR}"
echo "========================================================="

# Pick sudo only when the target dir is not writable by the current user.
SUDO=""
if [ ! -w "${INSTALL_DIR}" ]; then
    if command -v sudo >/dev/null 2>&1; then
        SUDO="sudo"
    else
        echo "ERROR: ${INSTALL_DIR} is not writable and sudo is unavailable." >&2
        echo "       Re-run with INSTALL_DIR=\$HOME/.local/bin ./install_nrfutil.sh" >&2
        exit 1
    fi
fi

TMP_BIN="$(mktemp)"
trap 'rm -f "${TMP_BIN}"' EXIT

echo "Downloading nrfutil ..."
if ! curl -fSL --retry 3 --max-time 180 "${NRFUTIL_URL}" -o "${TMP_BIN}"; then
    echo "ERROR: download failed. Check the URL or grab it manually from:" >&2
    echo "       https://www.nordicsemi.com/Products/Development-tools/nRF-Util/Download" >&2
    exit 1
fi

# Sanity-check we got an executable, not an HTML error page.
if ! head -c 4 "${TMP_BIN}" | grep -q $'\x7fELF'; then
    echo "ERROR: downloaded file is not an ELF executable (URL may have moved)." >&2
    exit 1
fi

chmod +x "${TMP_BIN}"
${SUDO} install -m 0755 "${TMP_BIN}" "${INSTALL_DIR}/nrfutil"
echo "Installed: ${INSTALL_DIR}/nrfutil"

if ! command -v nrfutil >/dev/null 2>&1; then
    echo "WARNING: ${INSTALL_DIR} is not on PATH. Add it, e.g.:" >&2
    echo "         export PATH=\"${INSTALL_DIR}:\$PATH\"" >&2
    NRFUTIL="${INSTALL_DIR}/nrfutil"
else
    NRFUTIL="nrfutil"
fi

echo "Updating nrfutil core ..."
"${NRFUTIL}" self-upgrade || true

echo "Installing the 'device' command (program / recover / list) ..."
"${NRFUTIL}" install device

echo "---------------------------------------------------------"
"${NRFUTIL}" --version || true
"${NRFUTIL}" device --version || true
echo "---------------------------------------------------------"
echo "nRF Util ready."
echo

echo "========================================================="
echo "  USB Permissions & Host Environment Checks (Linux)"
echo "========================================================="

# Check dialout / plugdev group membership
CURRENT_USER="${USER:-$(whoami)}"
if ! id -Gn "${CURRENT_USER}" | grep -qE '\b(dialout|plugdev)\b'; then
    echo "WARNING: User '${CURRENT_USER}' is not in dialout or plugdev group."
    echo "         To fix USB serial permission issues, run:"
    echo "           sudo usermod -aG dialout,plugdev ${CURRENT_USER}"
    echo "         (Log out and back in for changes to take effect)."
fi

# Check for Nordic udev rules
if [ ! -f /etc/udev/rules.d/71-nrf.rules ] && [ ! -f /lib/udev/rules.d/71-nrf.rules ]; then
    echo "WARNING: Nordic udev rules (71-nrf.rules) not detected in /etc/udev/rules.d/."
    echo "         To install udev rules for nRF USB devices:"
    echo "           git clone https://github.com/NordicSemiconductor/nrf-udev"
    echo "           sudo cp nrf-udev/nrf-udev_1.0.1-all/lib/udev/rules.d/*.rules /etc/udev/rules.d/"
    echo "           sudo udevadm control --reload-rules && sudo udevadm trigger"
    echo "           rm -rf nrf-udev"
fi

# Check for ModemManager (cellular targets e.g. Thingy:91 X nRF9151)
if systemctl is-active --quiet ModemManager 2>/dev/null; then
    echo "NOTE: ModemManager is currently running on host."
    echo "      ModemManager can lock /dev/ttyACM* ports and cause MCUboot DFU timeouts."
    echo "      If USB flashing hangs at 0%, temporarily stop ModemManager:"
    echo "        sudo systemctl stop ModemManager"
fi

echo
echo "NOTE: the external-probe (SWD / J-Link) flashing path also needs SEGGER"
echo "      J-Link software installed separately:"
echo "      https://www.segger.com/downloads/jlink/"
echo "      The USB / MCUboot path (no probe) needs no J-Link — see docs/ert-porting-guide.md."

