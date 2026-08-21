#!/bin/bash
#---------------------------------------------------------------
# Copyright (C) 2026 inx limited, UK - All Rights Reserved
# LGPLv3 license — see https://www.gnu.org/licenses/lgpl-3.0.txt
#---------------------------------------------------------------
#
# nrf_monitor.sh — open a serial console on the nRF USB-CDC port for eRT logs.
#
# Mirrors esp32_monitor_console.sh:
#     ./scripts/build-deploy/zephyr-nrf/nrf_monitor.sh                 # minicom (default)
#     ./scripts/build-deploy/zephyr-nrf/nrf_monitor.sh /dev/ttyACM1    # explicit port
#     ./scripts/build-deploy/zephyr-nrf/nrf_monitor.sh --use-screen    # screen + logfile out.txt
#
# Baud is 115200 (matches the board .conf / prj.conf UART speed).
#
# NOTE: the USB/MCUboot flashing method uses this same port — don't run the
# monitor while flashing over USB (see nrf_flash.sh --usb).

BAUD="${NRF_BAUD:-115200}"

if [ "$1" == "--use-screen" ]; then
    rm -f out.txt
    date '+%d/%m/%Y %H:%M:%S' > out.txt
    TTYTERMINAL="screen -L -Logfile out.txt"
    echo "Using screen. <CTRL>+a k to quit, <CTRL>+a Esc to scroll."
    [ -n "$2" ] && NRF_TTY="$2"
else
    [ -n "$1" ] && NRF_TTY="$1"
    TTYTERMINAL="minicom -b ${BAUD} -D"
fi

# Autodetect the USB-CDC port (nRF enumerates as /dev/ttyACM*).
if [ -z "${NRF_TTY}" ]; then
    if   [ -c /dev/ttyACM0 ]; then NRF_TTY=/dev/ttyACM0
    elif [ -c /dev/ttyACM1 ]; then NRF_TTY=/dev/ttyACM1
    elif [ -c /dev/ttyUSB0 ]; then NRF_TTY=/dev/ttyUSB0
    else
        echo "Could not find /dev/ttyACM0 (or ACM1 / ttyUSB0)."
        echo "If you are running in WSL, install usbipd: https://github.com/dorssel/usbipd-win"
        echo "  usbipd list ; usbipd bind --busid=<b> ; usbipd attach --wsl --busid=<b>"
        exit 1
    fi
fi

echo "Serial console on ${NRF_TTY} @ ${BAUD} 8N1"
if [ "$1" == "--use-screen" ]; then
    ${TTYTERMINAL} "${NRF_TTY}" "${BAUD}"
else
    ${TTYTERMINAL} "${NRF_TTY}"
fi
