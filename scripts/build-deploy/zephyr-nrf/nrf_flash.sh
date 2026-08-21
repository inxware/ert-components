#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2026 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# nrf_flash.sh — flash a built Zephyr/NCS nRF image with nRF Util.
#
# Run from the ert-components repo ROOT (reads TARGET.cfg), like the esp32
# scripts:
#     ./scripts/build-deploy/zephyr-nrf/nrf_flash.sh            # USB / MCUboot DFU (no probe)
#     ./scripts/build-deploy/zephyr-nrf/nrf_flash.sh --swd      # external SWD probe / J-Link
#     ./scripts/build-deploy/zephyr-nrf/nrf_flash.sh --swd --serial 1050XXXXXX
#
# Requires: nrfutil (+ 'nrfutil install device'); see install_nrfutil.sh.
# The --swd path additionally needs SEGGER J-Link installed.
#
# Overridable via env:
#   NRF_XFAMILY   nRF Util device family     (default: nrf91)
#   NRF_CORE      target core                (default: Application)
#   NRF_SERIAL    debug-probe / device SN    (default: auto — single device)
#
# NOTE (first-cut wrapper): family/core are wired for the Thingy:91 X nRF9151.
# Naming/flags are open to change — flash manually the first time to confirm.

set -e

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

if [ ! -f TARGET.cfg ]; then
    echo "ERROR: run this from the ert-components repo root (TARGET.cfg not found here)." >&2
    exit 1
fi
source TARGET.cfg
if [ -z "${TARGET}" ]; then echo "ERROR: TARGET not set in TARGET.cfg." >&2; exit 1; fi

BIN="../TARGET_TREES/ehs_env-${TARGET}/bin"
XFAMILY="${NRF_XFAMILY:-nrf91}"
CORE="${NRF_CORE:-Application}"

# --- parse args -------------------------------------------------------------
METHOD="usb"                 # usb (MCUboot DFU) | swd (J-Link)
SERIAL="${NRF_SERIAL:-}"
AUTO_MONITOR=false
while [ "$#" -gt 0 ]; do
    case "$1" in
        --swd)          METHOD="swd"; shift ;;
        --usb)          METHOD="usb"; shift ;;
        --serial)       SERIAL="$2"; shift 2 ;;
        --monitor)      AUTO_MONITOR=true; shift ;;
        -h|--help)      grep '^#' "$0" | sed 's/^# \{0,1\}//'; exit 0 ;;
        *) echo "Unknown arg: $1 (use --swd | --usb | --serial <SN> | --monitor)" >&2; exit 1 ;;
    esac
done

if ! command -v nrfutil >/dev/null 2>&1; then
    echo "ERROR: nrfutil not on PATH. Install it: ${SCRIPT_DIR}/install_nrfutil.sh" >&2
    exit 1
fi

SERIAL_ARGS=()
[ -n "${SERIAL}" ] && SERIAL_ARGS=(--serial-number "${SERIAL}")

# --- select firmware + transport -------------------------------------------
if [ "${METHOD}" = "usb" ]; then
    FW="${BIN}/dfu_application.zip"
    TRAITS="mcuBoot"
    echo "Method: USB / MCUboot serial-recovery DFU (no probe)."
    echo "  - Set SW1 -> ON. Do NOT open a serial terminal on the USB port during flashing."
    echo "  - Requires an MCUboot/sysbuild build (ERT_ZEPHYR_SYSBUILD=y)."
else
    FW="${BIN}/merged.hex"
    TRAITS="jlink"
    echo "Method: external SWD probe / J-Link (full flash: MCUboot + b0 + app)."
    echo "  - Connect the probe to P8, set SW2 -> nRF91, SW1 -> ON."
    echo "  - Do NOT full-'recover'/erase-all — it wipes the factory modem firmware."
fi

if [ ! -f "${FW}" ]; then
    echo "ERROR: firmware not found: ${FW}" >&2
    echo "       Build it first (make zephyr_cmake_gen && make zephyr_build_docker)." >&2
    [ "${METHOD}" = "usb" ] && echo "       USB DFU needs ERT_ZEPHYR_SYSBUILD=y (produces dfu_application.zip)." >&2
    exit 1
fi

echo "---------------------------------------------------------"
echo "  TARGET   : ${TARGET}"
echo "  Firmware : ${FW}"
echo "  Traits   : ${TRAITS}   family=${XFAMILY} core=${CORE}"
echo "---------------------------------------------------------"

nrfutil device program \
    --firmware "${FW}" \
    --traits "${TRAITS}" \
    --x-family "${XFAMILY}" \
    --core "${CORE}" \
    "${SERIAL_ARGS[@]}"

# SWD path: reset into the app. (Serial-recovery DFU resets on its own.)
if [ "${METHOD}" = "swd" ]; then
    nrfutil device reset "${SERIAL_ARGS[@]}" || true
fi

echo "Flash complete."

if [ "${AUTO_MONITOR}" = true ]; then
    echo "Launching serial monitor..."
    sleep 2
    "${SCRIPT_DIR}/nrf_monitor.sh"
else
    echo "Open a serial console with: ${SCRIPT_DIR}/nrf_monitor.sh"
fi
