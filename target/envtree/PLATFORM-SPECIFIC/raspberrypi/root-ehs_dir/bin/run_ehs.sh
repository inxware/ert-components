#!/bin/bash
########################################################################
# Copyright : inx limited, Sheffield, UK, 2008
# License   : wwww.inx-systems.com/inxware_license.html
# Type      : Target deployed unencrypted code
# Support   : support.inx-systems.com
# Description:
# Raspberry Pi platform-specific ehs launcher.
# Configures OS file systems, starts EHS using the host's runtime
# libraries (LIB_HOST mode), and handles restart behaviour.
#
# Supported Arguments: (Use "" as placeholders).
# 1 : NO_RESTART - stops EHS being restarted on exit  [default]
# 2 : LIB_HOST   - use host's runtime libraries       [default]
# 3 : GDB        - start EHS with GDB for debugging
########################################################################

export RUNMODE=${1:-NO_RESTART}
export LIBMODE=${2:-LIB_HOST}
export DEBUGMODE=$3

EHS_BIN="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

${EHS_BIN}/runOsInit.sh

while [ 1 ]
do
    export LD_LIBRARY_PATH="${EHS_BIN}/cslib"
    if [ "${DEBUGMODE}" == "GDB" ]; then
        gdb "${EHS_BIN}/ehs.exe"
    else
        "${EHS_BIN}/ehs.exe" || echo "Exiting ehs.exe"
    fi
    if [ "${RUNMODE}" != "YES_RESTART" ]; then
        echo "Not restarting ehs.exe"
        break
    fi
    echo "ehs: restarting!"
done
