#!/bin/bash

set -e

TARGET=$1
TARGET_ROOT=../TARGET_TREES/ehs_env-${TARGET}

echo "###############################################################"
echo "######################## ESP32 HACK ###########################"
echo "###############################################################"


DEFAULT_APP_DIR=${TARGET_ROOT}/appdata

# remove any font files for this target
if find ${DEFAULT_APP_DIR} -type f -name "*.bdf" | grep -q .; then
    find ${DEFAULT_APP_DIR} -type f -name "*.bdf" -exec rm -f {} +
fi

echo "DONE!"

echo "###############################################################"