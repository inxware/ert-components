#!/bin/bash

EHS_BIN="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"

${EHS_BIN}/runOsInit.sh
${EHS_BIN}/ehs.exe

