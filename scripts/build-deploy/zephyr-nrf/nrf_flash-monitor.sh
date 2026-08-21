#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2026 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# nrf_flash-monitor.sh — flash a built Zephyr/NCS nRF image and immediately open serial monitor.
#
# Usage (run from repo root):
#     ./scripts/build-deploy/zephyr-nrf/nrf_flash-monitor.sh
#     ./scripts/build-deploy/zephyr-nrf/nrf_flash-monitor.sh --swd
#     ./scripts/build-deploy/zephyr-nrf/nrf_flash-monitor.sh --serial <SN>

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

"${SCRIPT_DIR}/nrf_flash.sh" "$@" --monitor
